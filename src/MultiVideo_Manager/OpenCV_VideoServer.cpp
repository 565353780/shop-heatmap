#include "OpenCV_VideoServer.h"

OpenCV_VideoServer::OpenCV_VideoServer(QObject* parent)
{
    mutex_ = new QMutex();

    max_retry_times_ = 1000;
    save_frame_num_ = 5;

    stop_run_ = false;
}

OpenCV_VideoServer::~OpenCV_VideoServer()
{

}

bool OpenCV_VideoServer::openVideo(string video_path, bool all_frame_used)
{
    current_video_path_ = video_path;

    all_frame_used_ = all_frame_used;

    return opencv_get_image_.openVideo(video_path);
}

bool OpenCV_VideoServer::get_frame(Mat &frame)
{
    int retry_time = 0;

    while(frame_queue_.size() < 1)
    {
        msleep(1);
        ++retry_time;

        if(max_retry_times_ != -1)
        {
            if(retry_time > max_retry_times_)
            {
                cout << "OpenCV_VideoServer::get_frame : Can't get frame for " << max_retry_times_ << " times." << endl;
                return false;
            }
        }
    }

    if(frame_queue_.size() < 1)
    {
        cout << "OpenCV_VideoServer::get_frame : frame_queue_ is empty." << endl;
        return false;
    }

    frame = frame_queue_.dequeue();
    return true;
}

double OpenCV_VideoServer::getProp(int prop_id)
{
    return opencv_get_image_.getProp(prop_id);
}

void OpenCV_VideoServer::stopRun()
{
    stop_run_ = true;
}

void OpenCV_VideoServer::put_frame(Mat frame)
{
    if(all_frame_used_)
    {
        if(frame_queue_.size() > 0)
        {
            return;
        }

        Mat current_frame = opencv_get_image_.getImageFromVideo();

        frame_queue_.enqueue(current_frame);
    }
    else
    {
        if(frame_queue_.size() > save_frame_num_)
        {
            frame_queue_.dequeue();
        }

        frame_queue_.enqueue(frame);
    }
}

void OpenCV_VideoServer::run()
{
    int retry_times = 0;

    while(!opencv_get_image_.isVideoOpened())
    {
        if(stop_run_)
        {
            stop_run_ = false;
            return;
        }

        ++retry_times;
        cout << "OpenCV_VideoServer::run : Cannot open the videocapture. Retrying " << retry_times << " ..." << endl;
        opencv_get_image_.openVideo(current_video_path_);

        if(max_retry_times_ != -1)
        {
            if(retry_times >= max_retry_times_)
            {
                cout << "OpenCV_VideoServer::run : Cannot open the videocapture." << endl;
                return;
            }
        }
    }

    Mat current_frame;
    while(true)
    {
        if(stop_run_)
        {
            stop_run_ = false;
            return;
        }

        while(!opencv_get_image_.isVideoOpened())
        {
            if(stop_run_)
            {
                stop_run_ = false;
                return;
            }

            ++retry_times;
            cout << "OpenCV_VideoServer::run : Cannot open the videocapture. Retrying " << retry_times << " ..." << endl;
            opencv_get_image_.openVideo(current_video_path_);

            if(max_retry_times_ != -1)
            {
                if(retry_times >= max_retry_times_)
                {
                    cout << "OpenCV_VideoServer::run : Cannot open the videocapture." << endl;
                    return;
                }
            }
        }

        if(all_frame_used_)
        {
            put_frame(current_frame);
        }
        else
        {
            current_frame = opencv_get_image_.getImageFromVideo();

            if(current_frame.empty())
            {
                ++retry_times;
                cout << "OpenCV_VideoServer::run : frame empty. Retrying " << retry_times << " ..." << endl;

                if(max_retry_times_ != -1)
                {
                    if(retry_times >= max_retry_times_)
                    {
                        cout << "OpenCV_VideoServer::run : frame is empty." << endl;
                        return;
                    }
                }
            }

            put_frame(current_frame);
        }
    }
}
