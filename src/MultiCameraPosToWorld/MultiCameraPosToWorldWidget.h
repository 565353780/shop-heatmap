#ifndef MULTICAMERAPOSTOWORLDWIDGET_H
#define MULTICAMERAPOSTOWORLDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QEvent>
#include <QPaintEvent>
#include <QPen>

#include <iostream>
#include <fstream>
#include <vector>

#include <opencv2/opencv.hpp>

#include <Eigen/Core>

using namespace std;
using namespace cv;
using namespace Eigen;

namespace Ui {
class MultiCameraPosToWorldWidget;
}

class MultiCameraPosToWorldWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MultiCameraPosToWorldWidget(QWidget *parent = nullptr);
    ~MultiCameraPosToWorldWidget();

    void addTransMatrixWithID(MatrixXd trans_matrix, int id);

    void setTransMatrixWithID(MatrixXd trans_matrix, int id);

    vector<double> getWorldPosWithID(double x, double y, int id);

    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::MultiCameraPosToWorldWidget *ui;

    double mouse_x_;
    double mouse_y_;

    vector<int> opened_video_id_vec_;
    vector<MatrixXd> trans_matrix_vec_;
};

#endif // MULTICAMERAPOSTOWORLDWIDGET_H
