#include "OpenCV_GetImage.h"

OpenCV_GetImage::OpenCV_GetImage()
{

}

OpenCV_GetImage::~OpenCV_GetImage()
{
    if(video_capture_.isOpened())
    {
        video_capture_.release();
    }
}

bool OpenCV_GetImage::openVideo(string video_path)
{
    if(video_capture_.isOpened())
    {
        video_capture_.release();
    }

    video_capture_.open(video_path);
    if(!video_capture_.isOpened())
    {
        cout << "OpenCV_GetImage::openVideo : Cannot open the video." << endl;
        return false;
    }

    return true;
}

bool OpenCV_GetImage::isVideoOpened()
{
    return video_capture_.isOpened();
}

Mat OpenCV_GetImage::getImageFromFile(string filepath)
{
    Mat image = imread(filepath);

    return image;
}

Mat OpenCV_GetImage::getImageFromVideo()
{
    Mat image;

    if(!video_capture_.isOpened())
    {
        cout << "OpenCV_GetImage::getImageFromVideo : Cannot get image from video." << endl;
        return image;
    }

    video_capture_ >> image;

    return image;
}

double OpenCV_GetImage::getProp(int prop_id)
{
    return video_capture_.get(prop_id);
}
