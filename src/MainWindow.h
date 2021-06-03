#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MultiView_People_Detector.h"

#include <QProcess>

class MySocketServer;
class MainWindow : public QObject
{
    Q_OBJECT

public:
    explicit MainWindow(QObject *parent = nullptr);
    ~MainWindow();

    void updateCameraParam();

    void startDetect();
    void stopDetect();

private slots:
    void slot_requestReceived(QString msg);

    void slot_findPeople(double x, double y);

    void slot_update();

signals:
    void signal_updateCameraParam();
    void signal_startDetect();
    void signalSendTextMessage(QString);

private:
    MultiView_People_Detector* multivideo_people_detector_;
    QThread* multivideo_people_detector_thread_;

    MySocketServer* socket_server_;
    QThread* simple_server_thread_;

//    QString last_request_;
//    int last_request_call_times_;

    QImage* show_image_;
};

#endif // MAINWINDOW_H
