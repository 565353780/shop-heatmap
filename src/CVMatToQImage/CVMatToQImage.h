#ifndef CVMATTOQIMAGE_H
#define CVMATTOQIMAGE_H

#include <iostream>
#include <fstream>

#include <QImage>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class CVMatToQImage
{
public:
    CVMatToQImage();
    ~CVMatToQImage();

    QImage static cvMat2QImage(const Mat& mat);

    Mat static QImage2cvMat(QImage image);
};

#endif // CVMATTOQIMAGE_H
