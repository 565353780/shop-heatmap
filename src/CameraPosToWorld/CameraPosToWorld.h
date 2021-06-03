#ifndef CAMERAPOSTOWORLD_H
#define CAMERAPOSTOWORLD_H

#include <iostream>
#include <fstream>
#include <cmath>

#include <Eigen/Eigen>
#include <Eigen/Dense>
#include <Eigen/LU>
#include <Eigen/SVD>

using namespace std;
using namespace Eigen;

class CameraPosToWorld
{
public:
    CameraPosToWorld();
    ~CameraPosToWorld();

    void setCameraStartPos(double x_min, double y_min);
    void setCameraStartPos(vector<double> camera_start_pos);

    void setCameraEndPos(double x_max, double y_max);
    void setCameraEndPos(vector<double> camera_end_pos);

    void setWorldStartPos(double x_min, double y_min);
    void setWorldStartPos(vector<double> world_start_pos);

    void setWorldEndPos(double x_max, double y_max);
    void setWorldEndPos(vector<double> world_end_pos);

    void setCameraBoundingBox(double x_min, double y_min, double x_max, double y_max);
    void setCameraBoundingBox(vector<double> camera_bbox);

    void setWorldBoundingBox(double x_min, double y_min, double x_max, double y_max);
    void setWorldBoundingBox(vector<double> world_bbox);

    void setCameraAndWorldBoundingBox(vector<double> camera_bbox, vector<double> world_bbox);

    bool matchPosPair(vector<vector<double>> pos_pair_camera, vector<vector<double>> pos_pair_world);

    vector<double> getWorldPosFromCamera(double x, double y);
    vector<double> getWorldPosFromCamera(vector<double> camera_pos);

    vector<double> getCameraPosFromWorld(double x, double y);
    vector<double> getCameraPosFromWorld(vector<double> world_pos);

    MatrixXd getTransMatrix();
    MatrixXd getTransMatrixInverse();

private:
    bool checkPosAvalible();

    vector<double> toUnitCameraPos(double x, double y);
    vector<double> toUnitCameraPos(vector<double> camera_pos);

    vector<double> toSourceCameraPos(double unit_x, double unit_y);
    vector<double> toSourceCameraPos(vector<double> unit_camera_pos);

    vector<double> toUnitWorldPos(double x, double y);
    vector<double> toUnitWorldPos(vector<double> camera_pos);

    vector<double> toSourceWorldPos(double unit_x, double unit_y);
    vector<double> toSourceWorldPos(vector<double> unit_camera_pos);

    void setPosPair(vector<vector<double>> pos_pair_camera, vector<vector<double>> pos_pair_world);

    void normal(MatrixXd& P, Matrix3d& T);

    VectorXd solveHomographyDLT(MatrixXd& srcNormal, MatrixXd& dstNormal);

    bool updateTransMatrix();

private:
    vector<double> camera_start_pos_;
    vector<double> camera_end_pos_;
    vector<double> world_start_pos_;
    vector<double> world_end_pos_;

    vector<vector<double>> unit_pos_pair_;

    bool camera_start_pos_inited_;
    bool camera_end_pos_inited_;
    bool world_start_pos_inited_;
    bool world_end_pos_inited_;
    bool pos_pair_inited_;

    MatrixXd trans_matrix_;
    MatrixXd trans_matrix_inverse_;
};

#endif // CAMERAPOSTOWORLD_H
