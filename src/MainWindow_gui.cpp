#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "HttpApi/MySocketServer.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    last_request_ = "";

    multivideo_people_detector_ = new MultiView_People_Detector();
    multivideo_people_detector_thread_ = new QThread();
    multivideo_people_detector_->moveToThread(multivideo_people_detector_thread_);
    multivideo_people_detector_thread_->start();
    connect(this, SIGNAL(signal_updateCameraParam()), multivideo_people_detector_, SLOT(slot_updateCameraParam()));
    connect(this, SIGNAL(signal_startDetect()), multivideo_people_detector_, SLOT(slot_startDetect()));
    connect(multivideo_people_detector_, SIGNAL(signal_findPeople(double, double)), this, SLOT(slot_findPeople(double, double)));
    connect(multivideo_people_detector_, SIGNAL(signal_update()), this, SLOT(slot_update()));
    show_image_ = &multivideo_people_detector_->show_image;

    socket_server_ = new MySocketServer(nullptr,4400);//SimpleServer(nullptr, 4400);
    simple_server_thread_ = new QThread();
    socket_server_->moveToThread(simple_server_thread_);
    simple_server_thread_->start();
    connect(socket_server_, SIGNAL(signalReceiveText(QString)), this, SLOT(slot_requestReceived(QString)));
    connect(this, SIGNAL(signalSendTextMessage(QString)), socket_server_, SLOT(sendTextMessage(QString)));

    multivideo_people_detector_->init_darknet_detector("../data/coco80/yolov3.cfg", "../data/coco80/yolov3.weights");

    multivideo_people_detector_->openMultiCamera(false);

    updateCameraParam();

//    startDetect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCameraParam()
{
    emit signal_updateCameraParam();
}

void MainWindow::startDetect()
{
    emit signal_startDetect();
}

void MainWindow::stopDetect()
{
    multivideo_people_detector_->slot_stopDetect();
}

void MainWindow::slot_requestReceived(QString msg)
{
    QJsonDocument doc=QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject msg_obj=doc.object();
    QString method;
    QJsonObject content;
    if(msg_obj.contains("method")){
        method=msg_obj["method"].toString();
    }
    qDebug() << "get request :";
    qDebug() << "method :" << method;
    qDebug() << "content :" << content;

    if(method == "/updateCameraParam")
    {
        updateCameraParam();
    }
    else if(method == "/startDetect")
    {
        startDetect();
    }
    else if(method == "/stopDetect")
    {
        stopDetect();
    }

//    if(last_request_ == method)
//    {
//        ++last_request_call_times_;

//        if(last_request_call_times_ > 3)
//        {
//            last_request_call_times_ = 0;
//        }

//        if(last_request_call_times_ == 0)
//        {
//            if(method == "/updateCameraParam")
//            {
//                updateCameraParam();
//            }
//            else if(method == "/startDetect")
//            {
//                startDetect();
//            }
//            else if(method == "/stopDetect")
//            {
//                stopDetect();
//            }
//        }
//    }
//    else
//    {
//        last_request_ = method;
//        last_request_call_times_ = 1;

//        if(method == "/updateCameraParam")
//        {
//            updateCameraParam();
//        }
//        else if(method == "/startDetect")
//        {
//            startDetect();
//        }
//        else if(method == "/stopDetect")
//        {
//            stopDetect();
//        }
//    }

    emit signalSendTextMessage("OK");
}

void MainWindow::slot_findPeople(double x, double y)
{
    cout << "MainWindow::slot_findPeople : find people : " << x << " , " << y << endl;
}

void MainWindow::slot_update()
{
    this->ui->label_showImage->setPixmap(QPixmap::fromImage(*show_image_));
    this->update();
}
