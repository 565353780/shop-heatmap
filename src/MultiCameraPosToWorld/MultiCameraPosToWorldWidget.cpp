#include "MultiCameraPosToWorldWidget.h"
#include "ui_MultiCameraPosToWorldWidget.h"

MultiCameraPosToWorldWidget::MultiCameraPosToWorldWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultiCameraPosToWorldWidget)
{
    ui->setupUi(this);
}

MultiCameraPosToWorldWidget::~MultiCameraPosToWorldWidget()
{
    delete ui;
}

void MultiCameraPosToWorldWidget::addTransMatrixWithID(MatrixXd trans_matrix, int id)
{
    if(opened_video_id_vec_.size() > 0)
    {
        for(int i = 0; i < opened_video_id_vec_.size(); ++i)
        {
            if(opened_video_id_vec_[i] == id)
            {
                cout << "MultiCameraToWorldWidget::addTransMatrixWithID : this id already exist." << endl;
                return;
            }
        }
    }

    opened_video_id_vec_.emplace_back(id);
    trans_matrix_vec_.emplace_back(trans_matrix);
}

void MultiCameraPosToWorldWidget::setTransMatrixWithID(MatrixXd trans_matrix, int id)
{
    if(opened_video_id_vec_.size() == 0)
    {
        cout << "MultiCameraToWorldWidget::setTransMatrixWithID : no video is opened." << endl;
        return;
    }

    for(int i = 0; i < opened_video_id_vec_.size(); ++i)
    {
        if(opened_video_id_vec_[i] == id)
        {
            trans_matrix_vec_[i] = trans_matrix;
            return;
        }
    }

    cout << "MultiCameraToWorldWidget::setTransMatrixWithID : this id not exist." << endl;
}

vector<double> MultiCameraPosToWorldWidget::getWorldPosWithID(double x, double y, int id)
{
    vector<double> world_pos;
    world_pos.resize(2);
    world_pos[0] = -1;
    world_pos[1] = -1;

    for(int i = 0; i < opened_video_id_vec_.size(); ++i)
    {
        if(opened_video_id_vec_[i] == id)
        {
            MatrixXd camera_pos(3, 1);
            camera_pos(0, 0) = x;
            camera_pos(1, 0) = y;
            camera_pos(2, 0) = 1.0;

            MatrixXd world_pos_matrix = trans_matrix_vec_[i] * camera_pos;

            world_pos[0] = world_pos_matrix(0, 0) / world_pos_matrix(2, 0);
            world_pos[1] = world_pos_matrix(1, 0) / world_pos_matrix(2, 0);

            return world_pos;
        }
    }

    cout << "MultiCameraToWorldWidget::getWorldPosWithID : this id is not exist." << endl;
    return world_pos;
}

void MultiCameraPosToWorldWidget::mouseMoveEvent(QMouseEvent *event)
{
    mouse_x_ = event->x();
    mouse_y_ = event->y();

    this->update();
}

void MultiCameraPosToWorldWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    int radius = 5;

    QPen pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(2 * radius);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::red);
    painter.setPen(pen);

    if(opened_video_id_vec_.size() > 0)
    {
        for(int i = 0; i < opened_video_id_vec_.size(); ++i)
        {
            vector<double> world_pos = getWorldPosWithID(mouse_x_ / this->width(), mouse_y_ / this->height(), opened_video_id_vec_[i]);

            painter.drawPoint(world_pos[0] * this->width(), world_pos[1] * this->height());
            painter.drawText(world_pos[0] * this->width() + 2 * radius, world_pos[1] * this->height(), QString::number(opened_video_id_vec_[i]));
        }
    }
}
