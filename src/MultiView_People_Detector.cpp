#include "MultiView_People_Detector.h"

MultiView_People_Detector::MultiView_People_Detector(QObject *parent) :
    QObject(parent)
{
    open_camera_finished_ = true;
    camera_ready_ = false;
    keep_detect_ = false;

    data_manager_ = new DataManager();
    data_manager_thread_ = new QThread();

    QString database_type ="QMYSQL";
    QString connection_name ="Shop_Heat_Map";
    QString database_filename = "ShopHeatMap";
    QString host_name = "127.0.0.1";
    QString port = "3306";
    QString username = "root";
    QString password = "123456";

    data_manager_->init(database_type, connection_name, database_filename, host_name, port, username, password);
    data_manager_->moveToThread(data_manager_thread_);
    data_manager_thread_->start();

//    multi_camera_pos_to_world_widget_ = new MultiCameraPosToWorldWidget();
//    multi_camera_pos_to_world_widget_->show();
}

MultiView_People_Detector::~MultiView_People_Detector()
{

}

void MultiView_People_Detector::init_darknet_detector(string cfg_file, string weights_file)
{
    darknet_detector_.init_model(cfg_file, weights_file);
}

void MultiView_People_Detector::openMultiCamera(bool all_frame_used)
{
    open_camera_finished_ = false;
    camera_ready_ = false;

    multivideo_manager_.init();
    multi_camera_pos_to_world_.init();

    QJsonObject query_Obj;
    query_Obj.insert("CameraID", "-1");
    query_Obj.insert("CameraIDOperator", ">");

    bool success = data_manager_->queryData("CameraAddrs", query_Obj);

    if(!success)
    {
        cout << "MultiView_People_Detector::openMultiCamera : query camera ip failed." << endl;
        return;
    }

    QList<QJsonObject> result = data_manager_->getQueryResultList();

    if(result.size() == 0)
    {
        cout << "MultiView_People_Detector::openMultiCamera : camera ip not exist." << endl;
        return;
    }

    for(int i = 0; i < result.size(); ++i)
    {
        QString current_camera_ip = result[i].value("CameraIP").toString();
        int current_camera_id = result[i].value("CameraID").toString().toInt();

        open_video_with_id(current_camera_ip.toStdString(), current_camera_id, all_frame_used);
    }

    if(multi_camera_pos_to_world_.getOpenedVideoNum() == result.size())
    {
        camera_ready_ = true;
    }

    open_camera_finished_ = true;
}

void MultiView_People_Detector::open_video_with_id(string video_path, int id, bool all_frame_used)
{
    if(multivideo_manager_.openVideoWithID(video_path, id, all_frame_used))
    {
        multi_camera_pos_to_world_.addCameraPosToWorldWithtID(id);
    }
}

void MultiView_People_Detector::detect()
{
    int detect_num = 0;

    while(keep_detect_ && camera_ready_)
    {
        QTime time;
        time.start();

        ++detect_num;
        cout << detect_num << ":" << endl;
        vector<Mat> video_images;
        video_images.resize(multi_camera_pos_to_world_.getOpenedVideoNum());

        multivideo_manager_.updateImage();
        for(int i = 0; i < multi_camera_pos_to_world_.getOpenedVideoNum(); ++i)
        {
            video_images[i] = multivideo_manager_.getImageWithID(multi_camera_pos_to_world_.getOpenedVideoID(i));
        }

        vector<vector<vector<double>>> all_target_pos_vec;
        all_target_pos_vec.resize(multi_camera_pos_to_world_.getOpenedVideoNum());

        for(int i = 0; i < multi_camera_pos_to_world_.getOpenedVideoNum(); ++i)
        {
            bool detect_success = darknet_detector_.detect_image(video_images[i]);

            if(!detect_success)
            {
                cout << "MultiView_People_Detector::detect : detect failed for camera " << multi_camera_pos_to_world_.getOpenedVideoID(i) << "." << endl;
                return;
            }

            vector<vector<int>> result = darknet_detector_.get_result_trans();

            if(result.size() > 0)
            {
                for(int j = 0; j < result.size(); ++j)
                {
                    if(result[j][4] == 0)
                    {
                        double x = (result[j][0] + result[j][2]) / 2.0;
                        double y = 1.0 * result[j][3];

                        x /= video_images[i].cols;
                        y /= video_images[i].rows;

                        cout << "MultiView_People_Detector::detect : people pos : " << x << "," << y << " -->> ";

                        vector<double> people_pos_on_world = multi_camera_pos_to_world_.getWorldPosFromCameraWithID(x, y, multi_camera_pos_to_world_.getOpenedVideoID(i));

                        if(people_pos_on_world[0] >= 0 && people_pos_on_world[0] <= 1)
                        {
                            people_pos_on_world[1] = 1.0 - people_pos_on_world[1];

                            cout << people_pos_on_world[0] << "," << people_pos_on_world[1] << endl;

                            all_target_pos_vec[i].emplace_back(people_pos_on_world);
                        }
                        else
                        {
                            cout << "not used." << endl;
                        }
                    }
                }
            }
        }

        vector<vector<double>> world_target_pos_vec = multi_camera_pos_to_world_.getWorldTargetPos(all_target_pos_vec);

        Mat show_mat = multi_camera_pos_to_world_.getWorldTargetPosMap(1000, 1000);

        show_image = cv_mat_to_qimage_.cvMat2QImage(show_mat);

        emit signal_update();

        if(world_target_pos_vec.size() > 0)
        {
            QString date = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");

            for(int i = 0; i < world_target_pos_vec.size(); ++i)
            {
                QJsonArray position_array;
                position_array.append(world_target_pos_vec[i][0]);
                position_array.append(world_target_pos_vec[i][1]);
                position_array.append(1);

                QJsonObject position_Obj;
                position_Obj.insert("position", position_array);

                QJsonDocument doc(position_Obj);

                QJsonObject insert_Obj;
                insert_Obj.insert("ID", "5");
                insert_Obj.insert("Position", doc.toJson().toStdString().c_str());
                insert_Obj.insert("Time", date);

                bool success = data_manager_->insertData("PeoplePosition", insert_Obj);
                if(!success)
                {
                    cout << "MultiView_People_Detector::detect : insert failed." << endl;
                    return;
                }
                emit signal_findPeople(world_target_pos_vec[i][0], world_target_pos_vec[i][1]);
            }
        }

        while(time.elapsed() < 1000)
        {
            QCoreApplication::processEvents();
        }
    }
}

void MultiView_People_Detector::slot_updateCameraParam()
{
    if(!open_camera_finished_)
    {
        cout << "MultiView_People_Detector::slot_updateCameraParam : camera is updating, please try again." << endl;
        return;
    }

    openMultiCamera(false);

    QJsonObject query_camera_Obj;
    query_camera_Obj.insert("CameraID", "-1");
    query_camera_Obj.insert("CameraIDOperator", ">");

    bool query_success = data_manager_->queryData("CameraAddrs", query_camera_Obj);

    if(!query_success)
    {
        cout << "MultiView_People_Detector::openMultiCamera : query camera ip failed." << endl;
        return;
    }

    QList<QJsonObject> query_result = data_manager_->getQueryResultList();

    if(query_result.size() == 0)
    {
        cout << "MultiView_People_Detector::openMultiCamera : camera ip not exist." << endl;
        return;
    }

    for(int i = 0; i < query_result.size(); ++i)
    {
        int current_camera_id = query_result[i].value("CameraID").toString().toInt();

        if(!multi_camera_pos_to_world_.haveThisCameraID(current_camera_id))
        {
            continue;
        }

        QJsonObject query_Obj;
        query_Obj.insert("CameraNo", QString::number(current_camera_id));

        bool success = data_manager_->queryData("CameraCoord", query_Obj);

        if(!success)
        {
            cout << "MultiView_People_Detector::slot_updateCameraParam : query failed." << endl;
            return;
        }

        QList<QJsonObject> result = data_manager_->getQueryResultList();

        if(result.size() == 0)
        {
            cout << "MultiView_People_Detector::slot_updateCameraParam : camera data not exist." << endl;
            return;
        }
        else if(result.size() < 4)
        {
            cout << "MultiView_People_Detector::slot_updateCameraParam : camera data not enough." << endl;
            return;
        }

        vector<vector<double>> pos_pair_camera;
        pos_pair_camera.resize(result.size());
        for(int j = 0; j < pos_pair_camera.size(); ++j)
        {
            pos_pair_camera[j].resize(2);

            QString json_string = result[j].value("CameraCoord").toString();

            QJsonDocument doc = QJsonDocument::fromJson(json_string.toUtf8());

            QJsonObject position_Obj = doc.object();

            QJsonArray current_position_array = position_Obj.value("position").toArray();

            pos_pair_camera[j][0] = current_position_array[0].toDouble();
            pos_pair_camera[j][1] = 1.0 - current_position_array[1].toDouble();
        }

        vector<vector<double>> pos_pair_world;
        pos_pair_world.resize(result.size());
        for(int j = 0; j < pos_pair_world.size(); ++j)
        {
            pos_pair_world[j].resize(2);

            QString json_string = result[j].value("MapCoord").toString();

            QJsonDocument doc = QJsonDocument::fromJson(json_string.toUtf8());

            QJsonObject position_Obj = doc.object();

            QJsonArray current_position_array = position_Obj.value("position").toArray();

            pos_pair_world[j][0] = current_position_array[0].toDouble();
            pos_pair_world[j][1] = 1.0 - current_position_array[1].toDouble();
        }

        success = multi_camera_pos_to_world_.matchPosPairWithID(pos_pair_camera, pos_pair_world, current_camera_id);

        if(success)
        {
//            multi_camera_pos_to_world_widget_->addTransMatrixWithID(multi_camera_pos_to_world_.getTransMatrixWithID(current_camera_id), current_camera_id);
        }
    }
}

void MultiView_People_Detector::slot_startDetect()
{
    keep_detect_ = true;

    detect();
}

void MultiView_People_Detector::slot_stopDetect()
{
    keep_detect_ = false;
}
