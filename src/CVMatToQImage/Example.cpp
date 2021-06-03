#include "Example.h"

Example::Example()
{

}

Example::~Example()
{

}

void Example::run_example()
{
    Mat mat = imread("../20201222164429.jpg");

    QImage qimage = CVMatToQImage::cvMat2QImage(mat);

    Mat new_mat = CVMatToQImage::QImage2cvMat(qimage);

    imshow("trans result", new_mat);

    waitKey(0);
}
