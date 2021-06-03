#ifndef OPENCV_VIDEOSERVER_H
#define OPENCV_VIDEOSERVER_H

#include <QThread>
#include <QMutex>
#include <QQueue>

#include "MultiVideo_Manager/OpenCV_GetImage.h"

class OpenCV_VideoServer : public QThread
{
    Q_OBJECT
public:
    OpenCV_VideoServer(QObject* parent=nullptr);
    ~OpenCV_VideoServer();

    bool openVideo(string video_path, bool all_frame_used);

    bool get_frame(Mat &frame);

    double getProp(int prop_id);

    void stopRun();

private:
    void put_frame(Mat frame);

    void run() override;

private:
    string current_video_path_;

    bool all_frame_used_;

    int max_retry_times_;
    int save_frame_num_;

    //缓存容器
    QQueue<Mat> frame_queue_;
    //互斥锁
    QMutex *mutex_;

    OpenCV_GetImage opencv_get_image_;

    bool stop_run_;
};

#endif // OPENCV_VIDEOSERVER_H
