#include "MultiCameraPosToWorld.h"

MultiCameraPosToWorld::MultiCameraPosToWorld()
{
    max_target_dist_ = 0.1;
}

MultiCameraPosToWorld::~MultiCameraPosToWorld()
{

}

void MultiCameraPosToWorld::init()
{
    opened_video_id_vector_.clear();
    camera_pos_to_world_vector_.clear();
}

void MultiCameraPosToWorld::addCameraPosToWorldWithtID(int id)
{
    bool have_this_id = false;

    if(opened_video_id_vector_.size() > 0)
    {
        for(int i = 0; i < opened_video_id_vector_.size(); ++i)
        {
            if(opened_video_id_vector_[i] == id)
            {
                have_this_id = true;
                break;
            }
        }
    }

    if(have_this_id)
    {
        cout << "MultiCameraPosToWorld::addCameraPosToWorldWithtID : this id already exist :" << id << "." << endl;
        return;
    }

    opened_video_id_vector_.emplace_back(id);

    CameraPosToWorld camera_pos_to_world;
    camera_pos_to_world.setCameraBoundingBox(0.0, 0.0, 1.0, 1.0);
    camera_pos_to_world.setWorldBoundingBox(0.0, 0.0, 1.0, 1.0);
    camera_pos_to_world_vector_.emplace_back(camera_pos_to_world);
}

void MultiCameraPosToWorld::setCameraBoundingBoxWithID(double x_min, double y_min, double x_max, double y_max, int id)
{
    if(opened_video_id_vector_.size() == 0)
    {
        cout << "MultiCameraPosToWorld::setCameraBoundingBoxWithID : No video opened." << endl;
        return;
    }

    for(int i = 0; i < opened_video_id_vector_.size(); ++i)
    {
        if(opened_video_id_vector_[i] == id)
        {
            camera_pos_to_world_vector_[i].setCameraBoundingBox(x_min, y_min, x_max, y_max);
            return;
        }
    }

    cout << "MultiCameraPosToWorld::setCameraBoundingBoxWithID : This ID not found." << endl;
}

void MultiCameraPosToWorld::setWorldBoundingBoxWithID(double x_min, double y_min, double x_max, double y_max, int id)
{
    if(opened_video_id_vector_.size() == 0)
    {
        cout << "MultiCameraPosToWorld::setWorldBoundingBoxWithID : No video opened." << endl;
        return;
    }

    for(int i = 0; i < opened_video_id_vector_.size(); ++i)
    {
        if(opened_video_id_vector_[i] == id)
        {
            camera_pos_to_world_vector_[i].setWorldBoundingBox(x_min, y_min, x_max, y_max);
            return;
        }
    }

    cout << "MultiCameraPosToWorld::setWorldBoundingBoxWithID : This ID not found." << endl;
}

bool MultiCameraPosToWorld::matchPosPairWithID(vector<vector<double>> pos_pair_camera, vector<vector<double>> pos_pair_world, int id)
{
    if(opened_video_id_vector_.size() == 0)
    {
        cout << "MultiCameraPosToWorld::matchPosPairWithID : No video opened." << endl;
        return false;
    }

    for(int i = 0; i < opened_video_id_vector_.size(); ++i)
    {
        if(opened_video_id_vector_[i] == id)
        {
            camera_pos_to_world_vector_[i].matchPosPair(pos_pair_camera, pos_pair_world);
            return true;
        }
    }

    cout << "MultiCameraPosToWorld::matchPosPairWithID : This ID not found." << endl;
    return false;
}

int MultiCameraPosToWorld::getOpenedVideoNum()
{
    return opened_video_id_vector_.size();
}

int MultiCameraPosToWorld::getOpenedVideoID(int video_idx)
{
    if(opened_video_id_vector_.size() == 0)
    {
        cout << "MultiCameraPosToWorld::getOpenedVideoID : this id not exist." << endl;
        return -1;
    }
    if(video_idx < 0 || video_idx >= opened_video_id_vector_.size())
    {
        cout << "MultiCameraPosToWorld::getOpenedVideoID : video_idx out of range." << endl;
        return -1;
    }

    return opened_video_id_vector_[video_idx];
}

vector<double> MultiCameraPosToWorld::getWorldPosFromCameraWithID(double x, double y, int id)
{
    vector<double> world_pos;
    world_pos.resize(2);
    world_pos[0] = -1;
    world_pos[1] = -1;

    if(opened_video_id_vector_.size() == 0)
    {
        cout << "MultiCameraPosToWorld::getWorldPosFromCameraWithID : this id not exist." << endl;
        return world_pos;
    }

    for(int i = 0; i < opened_video_id_vector_.size(); ++i)
    {
        if(opened_video_id_vector_[i] == id)
        {
            return camera_pos_to_world_vector_[i].getWorldPosFromCamera(x, y);
        }
    }

    cout << "MultiCameraPosToWorld::getWorldPosFromCameraWithID : this id not exist." << endl;
    return world_pos;
}

MatrixXd MultiCameraPosToWorld::getTransMatrixWithID(int id)
{
    for(int i = 0; i < opened_video_id_vector_.size(); ++i)
    {
        if(opened_video_id_vector_[i] == id)
        {
            return camera_pos_to_world_vector_[i].getTransMatrix();
        }
    }

    MatrixXd trans_matrix(3, 3);
    cout << "MultiCameraPosToWorld::getTransMatrixWithID : this id is not exist." << endl;
    return trans_matrix;
}

bool MultiCameraPosToWorld::haveThisCameraID(int id)
{
    if(opened_video_id_vector_.size() == 0)
    {
        return false;
    }

    for(int i = 0; i < opened_video_id_vector_.size(); ++i)
    {
        if(opened_video_id_vector_[i] == id)
        {
            return true;
        }
    }

    return false;
}

vector<vector<double>> MultiCameraPosToWorld::getWorldTargetPos(vector<vector<vector<double>>> all_target_pos_vec)
{
    vector<vector<double>> world_target_pos_vec;

    if(all_target_pos_vec.size() == 0)
    {
//        cout << "MultiCameraPosToWorld::getWorldTargetPos : no people." << endl;
        return world_target_pos_vec;
    }

    matchMultiTargetPos(all_target_pos_vec);

    world_target_pos_vec.resize(target_list_.size());
    for(int i = 0; i < world_target_pos_vec.size(); ++i)
    {
        world_target_pos_vec[i].resize(2);
    }

    int current_target_idx = 0;
    for(auto itr = target_list_.begin(); itr != target_list_.end(); ++itr)
    {
        world_target_pos_vec[current_target_idx][0] = itr->x;
        world_target_pos_vec[current_target_idx][1] = itr->y;
        ++current_target_idx;
    }

//    showWorldTargetPos();

    return world_target_pos_vec;
}

Mat MultiCameraPosToWorld::getWorldTargetPosMap(int image_width, int image_height)
{
    Mat show_image = Mat(image_width, image_height, CV_8UC3, Scalar(255, 255, 255));

    for(auto itr = target_list_.begin(); itr != target_list_.end(); ++itr)
    {
        circle(show_image, Point(itr->x * show_image.cols, itr->y * show_image.rows), 3, Scalar(0, 0, 255), 3);
    }

    return show_image;
}

bool MultiCameraPosToWorld::haveThisTargetID(int id)
{
    if(target_list_.empty())
    {
        return false;
    }

    for (auto itr = target_list_.begin(); itr != target_list_.end(); ++itr)
    {
        if(itr->id == id)
        {
            return true;
        }
    }

    return false;
}

void MultiCameraPosToWorld::addTarget(double x, double y, int first_find_camera_idx)
{
    if(first_find_camera_idx < 0 || first_find_camera_idx >= opened_video_id_vector_.size())
    {
        cout << "MultiCameraPosToWorld::addTarget : this first_find_camera_idx out of range." << endl;
        return;
    }

    int add_id = 0;

    bool have_this_id = haveThisTargetID(add_id);

    while(have_this_id)
    {
        ++add_id;
        have_this_id = haveThisTargetID(add_id);
    }

    Target add_target;
    add_target.id = add_id;
    add_target.x = x;
    add_target.y = y;
    add_target.visiable = true;
    add_target.x_in_multicamera.resize(opened_video_id_vector_.size());
    add_target.y_in_multicamera.resize(opened_video_id_vector_.size());
    add_target.visiable_in_multicamera.resize(opened_video_id_vector_.size());
    for(int i = 0; i < opened_video_id_vector_.size(); ++i)
    {
        add_target.x_in_multicamera[i] = -1;
        add_target.y_in_multicamera[i] = -1;
        add_target.visiable_in_multicamera[i] = false;
    }

    add_target.x_in_multicamera[first_find_camera_idx] = x;
    add_target.y_in_multicamera[first_find_camera_idx] = y;
    add_target.visiable_in_multicamera[first_find_camera_idx] = true;

    target_list_.push_back(add_target);
}

void MultiCameraPosToWorld::removeTargetWithId(int target_id)
{
    if(target_list_.empty())
    {
        return;
    }

    for (auto itr = target_list_.begin(); itr != target_list_.end(); ++itr)
    {
        if(itr->id == target_id)
        {
            target_list_.erase(itr);
            return;
        }
    }
}

double MultiCameraPosToWorld::getDist(vector<double> target_pos_1, vector<double> target_pos_2)
{
    double delta_x = target_pos_1[0] - target_pos_2[0];
    double delta_y = target_pos_1[1] - target_pos_2[1];

    double dist = sqrt(delta_x * delta_x + delta_y * delta_y);

    return dist;
}

void MultiCameraPosToWorld::initTargetList()
{
    target_list_.clear();
}

void MultiCameraPosToWorld::mergeMultiTargetWithCameraIdx(vector<vector<double>> target_pos_vec, int camera_idx)
{
    if(target_pos_vec.size() == 0)
    {
//        cout << "MultiCameraPosToWorld::mergeMultiTargetWithCameraIdx : this target_pos_vec is empty." << endl;
        return;
    }
    if(camera_idx < 0 || camera_idx >= opened_video_id_vector_.size())
    {
        cout << "MultiCameraPosToWorld::mergeMultiTargetWithCameraIdx : this camera_idx out of range." << endl;
        return;
    }

    if(target_list_.empty())
    {
        for(int i = 0; i < target_pos_vec.size(); ++i)
        {
            addTarget(target_pos_vec[i][0], target_pos_vec[i][1], camera_idx);
        }

        return;
    }

    vector<vector<double>> dists_to_all_target_vec;
    dists_to_all_target_vec.resize(target_pos_vec.size());
    for(int i = 0; i < dists_to_all_target_vec.size(); ++i)
    {
        dists_to_all_target_vec[i].resize(target_list_.size());
    }
    int current_target_idx = 0;
    for (auto itr = target_list_.begin(); itr != target_list_.end(); ++itr)
    {
        vector<double> current_target_position;
        current_target_position.resize(2);
        current_target_position[0] = itr->x;
        current_target_position[1] = itr->y;

        for(int i = 0; i < target_pos_vec.size(); ++i)
        {
            dists_to_all_target_vec[i][current_target_idx] = getDist(target_pos_vec[i], current_target_position);
        }
        ++current_target_idx;
    }

    vector<int> merge_to_target_idx_vec;
    vector<bool> target_list_merged_vec;
    merge_to_target_idx_vec.resize(target_pos_vec.size());
    for(int i = 0; i < merge_to_target_idx_vec.size(); ++i)
    {
        merge_to_target_idx_vec[i] = -1;
    }
    target_list_merged_vec.resize(target_list_.size());
    for(int i = 0; i < target_list_merged_vec.size(); ++i)
    {
        target_list_merged_vec[i] = false;
    }

    bool merge_result_changed  =true;

    while(merge_result_changed)
    {
        merge_result_changed = false;

        double min_dist = -1;
        int target_pos_vec_idx = -1;
        int target_list_idx = -1;

        for(int i = 0; i < target_pos_vec.size(); ++i)
        {
            if(merge_to_target_idx_vec[i] == -1)
            {
                for(int j = 0; j < target_list_.size(); ++j)
                {
                    if(target_list_merged_vec[j])
                    {
                        continue;
                    }

                    if(dists_to_all_target_vec[i][j] < min_dist || min_dist == -1)
                    {
                        min_dist = dists_to_all_target_vec[i][j];
                        target_pos_vec_idx = i;
                        target_list_idx = j;
                    }
                }
            }
        }

        if(min_dist != -1 && min_dist < max_target_dist_)
        {
            merge_to_target_idx_vec[target_pos_vec_idx] = target_list_idx;
            target_list_merged_vec[target_list_idx] = true;

            int current_target_idx = 0;
            for(auto itr = target_list_.begin(); itr != target_list_.end(); ++itr)
            {
                if(current_target_idx == target_list_idx)
                {
                    itr->x_in_multicamera[camera_idx] = target_pos_vec[target_pos_vec_idx][0];
                    itr->y_in_multicamera[camera_idx] = target_pos_vec[target_pos_vec_idx][1];
                    itr->visiable_in_multicamera[camera_idx] = true;

                    double x_sum = 0;
                    double y_sum = 0;
                    int visiable_camera_num = 0;
                    for(int i = 0; i < itr->visiable_in_multicamera.size(); ++i)
                    {
                        if(itr->visiable_in_multicamera[i])
                        {
                            x_sum += itr->x_in_multicamera[i];
                            y_sum += itr->y_in_multicamera[i];
                            ++visiable_camera_num;
                        }
                    }

                    itr->x = x_sum / visiable_camera_num;
                    itr->y = y_sum / visiable_camera_num;

                    break;
                }

                ++current_target_idx;
            }

            merge_result_changed = true;
        }
    }

    for(int i = 0; i < merge_to_target_idx_vec.size(); ++i)
    {
        if(merge_to_target_idx_vec[i] == -1)
        {
            addTarget(target_pos_vec[i][0], target_pos_vec[i][1], camera_idx);
        }
    }
}

void MultiCameraPosToWorld::matchMultiTargetPos(vector<vector<vector<double>>> all_target_pos_vec)
{
    initTargetList();

    if(opened_video_id_vector_.size() == 0)
    {
        cout << "MultiCameraPosToWorld::matchMultiTargetPos : no video opened." << endl;
        return;
    }
    if(opened_video_id_vector_.size() != all_target_pos_vec.size())
    {
        cout << "MultiCameraPosToWorld::matchMultiTargetPos : opened video num and target_pos_vec.size not match." << endl;
        return;
    }

    for(int i = 0; i < opened_video_id_vector_.size(); ++i)
    {
        mergeMultiTargetWithCameraIdx(all_target_pos_vec[i], i);
    }
}

void MultiCameraPosToWorld::showWorldTargetPos(int image_width, int image_height)
{
    Mat show_image = Mat(image_width, image_height, CV_8UC3, Scalar(255, 255, 255));

    for(auto itr = target_list_.begin(); itr != target_list_.end(); ++itr)
    {
        circle(show_image, Point(itr->x * show_image.cols, itr->y * show_image.rows), 3, Scalar(0, 0, 255), 3);
    }

    imshow("world target pos", show_image);
    waitKey(1);
}
