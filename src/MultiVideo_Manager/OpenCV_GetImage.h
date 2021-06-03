#ifndef OPENCV_GETIMAGE_H
#define OPENCV_GETIMAGE_H

#include <iostream>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>
#include <string.h>

using namespace std;
using namespace cv;

class OpenCV_GetImage
{
public:
    OpenCV_GetImage();
    ~OpenCV_GetImage();

    bool openVideo(string video_path);
    bool isVideoOpened();

    Mat getImageFromFile(string file_path);
    Mat getImageFromVideo();

    double getProp(int prop_id);

private:
    VideoCapture video_capture_;
};

#endif // OPENCV_GETIMAGE_H
