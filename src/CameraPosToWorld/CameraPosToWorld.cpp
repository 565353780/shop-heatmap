#include "CameraPosToWorld.h"

CameraPosToWorld::CameraPosToWorld()
{
    camera_start_pos_.resize(2);
    camera_end_pos_.resize(2);
    world_start_pos_.resize(2);
    world_end_pos_.resize(2);

    camera_start_pos_inited_ = false;
    camera_end_pos_inited_ = false;
    world_start_pos_inited_ = false;
    world_end_pos_inited_ = false;
    pos_pair_inited_ = false;

    trans_matrix_.resize(3, 3);
    trans_matrix_inverse_.resize(3, 3);
}

CameraPosToWorld::~CameraPosToWorld()
{

}

void CameraPosToWorld::setCameraStartPos(double x_min, double y_min)
{
    camera_start_pos_[0] = x_min;
    camera_start_pos_[1] = y_min;

    camera_start_pos_inited_ = true;
}

void CameraPosToWorld::setCameraStartPos(vector<double> camera_start_pos)
{
    camera_start_pos_inited_ = false;

    if(camera_start_pos.size() != 2)
    {
        cout << "CameraPosToWorld::setCameraStartPos : Can't set camera_start_pos;" << endl;
        return;
    }

    setCameraStartPos(camera_start_pos[0], camera_start_pos[1]);
}

void CameraPosToWorld::setCameraEndPos(double x_max, double y_max)
{
    camera_end_pos_[0] = x_max;
    camera_end_pos_[1] = y_max;

    camera_end_pos_inited_ = true;
}

void CameraPosToWorld::setCameraEndPos(vector<double> camera_end_pos)
{
    camera_end_pos_inited_ = false;

    if(camera_end_pos.size() != 2)
    {
        cout << "CameraPosToWorld::setCameraEndPos : Can't set camera_end_pos;" << endl;
        return;
    }

    setCameraEndPos(camera_end_pos[0], camera_end_pos[1]);
}

void CameraPosToWorld::setWorldStartPos(double x_min, double y_min)
{
    world_start_pos_[0] = x_min;
    world_start_pos_[1] = y_min;

    world_start_pos_inited_ = true;
}

void CameraPosToWorld::setWorldStartPos(vector<double> world_start_pos)
{
    world_start_pos_inited_ = false;

    if(world_start_pos.size() != 2)
    {
        cout << "CameraPosToWorld::setWorldStartPos : Can't set world_start_pos;" << endl;
        return;
    }

    setWorldStartPos(world_start_pos[0], world_start_pos[1]);
}

void CameraPosToWorld::setWorldEndPos(double x_max, double y_max)
{
    world_end_pos_[0] = x_max;
    world_end_pos_[1] = y_max;

    world_end_pos_inited_ = true;
}

void CameraPosToWorld::setWorldEndPos(vector<double> world_end_pos)
{
    world_end_pos_inited_ = false;

    if(world_end_pos.size() != 2)
    {
        cout << "CameraPosToWorld::setWorldEndPos : Can't set world_end_pos;" << endl;
        return;
    }

    setWorldEndPos(world_end_pos[0], world_end_pos[1]);
}

void CameraPosToWorld::setCameraBoundingBox(double x_min, double y_min, double x_max, double y_max)
{
    setCameraStartPos(x_min, y_min);
    setCameraEndPos(x_max, y_max);
}

void CameraPosToWorld::setCameraBoundingBox(vector<double> camera_bbox)
{
    if(camera_bbox.size() != 4)
    {
        cout << "CameraPosToWorld::setCameraBoundingBox : input must be x_min,y_min,x_max,y_max." << endl;
        return;
    }

    setCameraStartPos(camera_bbox[0], camera_bbox[1]);
    setCameraEndPos(camera_bbox[2], camera_bbox[3]);
}

void CameraPosToWorld::setWorldBoundingBox(double x_min, double y_min, double x_max, double y_max)
{
    setWorldStartPos(x_min, y_min);
    setWorldEndPos(x_max, y_max);
}

void CameraPosToWorld::setWorldBoundingBox(vector<double> world_bbox)
{
    if(world_bbox.size() != 4)
    {
        cout << "CameraPosToWorld::setWorldBoundingBox : input must be x_min,y_min,x_max,y_max." << endl;
        return;
    }

    setWorldStartPos(world_bbox[0], world_bbox[1]);
    setWorldEndPos(world_bbox[2], world_bbox[3]);
}

void CameraPosToWorld::setCameraAndWorldBoundingBox(vector<double> camera_bbox, vector<double> world_bbox)
{
    setCameraBoundingBox(camera_bbox);
    setWorldBoundingBox(world_bbox);
}

bool CameraPosToWorld::matchPosPair(vector<vector<double>> pos_pair_camera, vector<vector<double>> pos_pair_world)
{
    setPosPair(pos_pair_camera, pos_pair_world);

    if(!pos_pair_inited_)
    {
        return false;
    }

    bool update_trans_success = updateTransMatrix();

    return update_trans_success;
}

vector<double> CameraPosToWorld::getWorldPosFromCamera(double x, double y)
{
    vector<double> unit_world_pos;

    unit_world_pos.resize(2);

    vector<double> unit_camera_pos = toUnitCameraPos(x, y);

    MatrixXd camera_pos_matrix(3, 1);
    camera_pos_matrix(0, 0) = unit_camera_pos[0];
    camera_pos_matrix(1, 0) = unit_camera_pos[1];
    camera_pos_matrix(2, 0) = 1.0;

    MatrixXd world_pos_matrix = trans_matrix_ * camera_pos_matrix;

    unit_world_pos[0] = 1.0 * world_pos_matrix(0, 0) / world_pos_matrix(2, 0);
    unit_world_pos[1] = 1.0 * world_pos_matrix(1, 0) / world_pos_matrix(2, 0);

    vector<double> world_pos = toSourceWorldPos(unit_world_pos);

    return world_pos;
}

vector<double> CameraPosToWorld::getWorldPosFromCamera(vector<double> camera_pos)
{
    vector<double> unit_world_pos;

    if(camera_pos.size() != 2)
    {
        cout << "CameraPosToWorld::getWorldPosFromCamera : camera_pos not available." << endl;
        return unit_world_pos;
    }

    unit_world_pos.resize(2);

    vector<double> unit_camera_pos = toUnitCameraPos(camera_pos);

    MatrixXd camera_pos_matrix(3, 1);
    camera_pos_matrix(0, 0) = unit_camera_pos[0];
    camera_pos_matrix(1, 0) = unit_camera_pos[1];
    camera_pos_matrix(2, 0) = 1.0;

    MatrixXd world_pos_matrix = trans_matrix_ * camera_pos_matrix;

    unit_world_pos[0] = 1.0 * world_pos_matrix(0, 0) / world_pos_matrix(2, 0);
    unit_world_pos[1] = 1.0 * world_pos_matrix(1, 0) / world_pos_matrix(2, 0);

    vector<double> world_pos = toSourceWorldPos(unit_world_pos);

    return world_pos;
}

vector<double> CameraPosToWorld::getCameraPosFromWorld(double x, double y)
{
    vector<double> unit_camera_pos;

    unit_camera_pos.resize(2);

    vector<double> unit_world_pos = toUnitWorldPos(x, y);

    MatrixXd world_pos_matrix(3, 1);
    world_pos_matrix(0, 0) = unit_world_pos[0];
    world_pos_matrix(1, 0) = unit_world_pos[1];
    world_pos_matrix(2, 0) = 1.0;

    MatrixXd camera_pos_matrix = trans_matrix_inverse_ * world_pos_matrix;

    unit_camera_pos[0] = 1.0 * camera_pos_matrix(0, 0) / camera_pos_matrix(2, 0);
    unit_camera_pos[1] = 1.0 * camera_pos_matrix(1, 0) / camera_pos_matrix(2, 0);

    vector<double> camera_pos = toSourceCameraPos(unit_camera_pos);

    return camera_pos;
}

vector<double> CameraPosToWorld::getCameraPosFromWorld(vector<double> world_pos)
{
    vector<double> unit_camera_pos;

    if(world_pos.size() != 2)
    {
        cout << "CameraPosToWorld::getCameraPosFromWorld : world_pos not available." << endl;
        return unit_camera_pos;
    }

    unit_camera_pos.resize(2);

    vector<double> unit_world_pos = toUnitWorldPos(world_pos);

    MatrixXd world_pos_matrix(3, 1);
    world_pos_matrix(0, 0) = unit_world_pos[0];
    world_pos_matrix(1, 0) = unit_world_pos[1];
    world_pos_matrix(2, 0) = 1.0;

    MatrixXd camera_pos_matrix = trans_matrix_inverse_ * world_pos_matrix;

    unit_camera_pos[0] = 1.0 * camera_pos_matrix(0, 0) / camera_pos_matrix(2, 0);
    unit_camera_pos[1] = 1.0 * camera_pos_matrix(1, 0) / camera_pos_matrix(2, 0);

    vector<double> camera_pos = toSourceCameraPos(unit_camera_pos);

    return camera_pos;
}

MatrixXd CameraPosToWorld::getTransMatrix()
{
    return trans_matrix_;
}

MatrixXd CameraPosToWorld::getTransMatrixInverse()
{
    return trans_matrix_inverse_;
}

bool CameraPosToWorld::checkPosAvalible()
{
    if(camera_start_pos_inited_ && camera_end_pos_inited_ && world_start_pos_inited_ && world_end_pos_inited_)
    {
        return true;
    }

    if(!camera_start_pos_inited_)
    {
        cout << "CameraPosToWorld::checkPosAvalible : camera_start_pos is not given." << endl;
    }
    if(!camera_end_pos_inited_)
    {
        cout << "CameraPosToWorld::checkPosAvalible : camera_end_pos is not given." << endl;
    }
    if(!world_start_pos_inited_)
    {
        cout << "CameraPosToWorld::checkPosAvalible : world_start_pos is not given." << endl;
    }
    if(!world_end_pos_inited_)
    {
        cout << "CameraPosToWorld::checkPosAvalible : world_end_pos is not given." << endl;
    }

    return false;
}

vector<double> CameraPosToWorld::toUnitCameraPos(double x, double y)
{
    vector<double> unit_camera_pos;
    unit_camera_pos.resize(2);

    unit_camera_pos[0] = 1.0 * (x - camera_start_pos_[0]) / (camera_end_pos_[0] - camera_start_pos_[0]);
    unit_camera_pos[1] = 1.0 * (y - camera_start_pos_[1]) / (camera_end_pos_[1] - camera_start_pos_[1]);

    return unit_camera_pos;
}

vector<double> CameraPosToWorld::toUnitCameraPos(vector<double> camera_pos)
{
    return toUnitCameraPos(camera_pos[0], camera_pos[1]);
}

vector<double> CameraPosToWorld::toSourceCameraPos(double unit_x, double unit_y)
{
    vector<double> camera_pos;
    camera_pos.resize(2);

    camera_pos[0] = camera_start_pos_[0] +  1.0 * unit_x * (camera_end_pos_[0] - camera_start_pos_[0]);
    camera_pos[1] = camera_start_pos_[1] +  1.0 * unit_y * (camera_end_pos_[1] - camera_start_pos_[1]);

    return camera_pos;
}

vector<double> CameraPosToWorld::toSourceCameraPos(vector<double> unit_camera_pos)
{
    return toSourceCameraPos(unit_camera_pos[0], unit_camera_pos[1]);
}

vector<double> CameraPosToWorld::toUnitWorldPos(double x, double y)
{
    vector<double> unit_world_pos;
    unit_world_pos.resize(2);

    unit_world_pos[0] = 1.0 * (x - world_start_pos_[0]) / (world_end_pos_[0] - world_start_pos_[0]);
    unit_world_pos[1] = 1.0 * (y - world_start_pos_[1]) / (world_end_pos_[1] - world_start_pos_[1]);

    return unit_world_pos;
}

vector<double> CameraPosToWorld::toUnitWorldPos(vector<double> world_pos)
{
    return toUnitWorldPos(world_pos[0], world_pos[1]);
}

vector<double> CameraPosToWorld::toSourceWorldPos(double unit_x, double unit_y)
{
    vector<double> world_pos;
    world_pos.resize(2);

    world_pos[0] = world_start_pos_[0] +  1.0 * unit_x * (world_end_pos_[0] - world_start_pos_[0]);
    world_pos[1] = world_start_pos_[1] +  1.0 * unit_y * (world_end_pos_[1] - world_start_pos_[1]);

    return world_pos;
}

vector<double> CameraPosToWorld::toSourceWorldPos(vector<double> unit_world_pos)
{
    return toSourceWorldPos(unit_world_pos[0], unit_world_pos[1]);
}

void CameraPosToWorld::setPosPair(vector<vector<double>> pos_pair_camera, vector<vector<double>> pos_pair_world)
{
    pos_pair_inited_ = false;

    if(!checkPosAvalible())
    {
        return;
    }

    if(pos_pair_camera.size() != pos_pair_world.size())
    {
        cout << "CameraPosToWorld::setPosPair : pos_pair_camera and pos_pair_world don't have the same pos num." << endl;
        return;
    }

    if(pos_pair_camera.size() == 0)
    {
        cout << "CameraPosToWorld::setPosPair : pos_pair_camera and pos_pair_world are empty." << endl;
        return;
    }

    unit_pos_pair_.resize(pos_pair_camera.size());

    for(int i = 0; i < unit_pos_pair_.size(); ++i)
    {
        vector<double> current_unit_camera_pos = toUnitCameraPos(pos_pair_camera[i]);
        vector<double> current_unit_world_pos = toUnitWorldPos(pos_pair_world[i]);

        unit_pos_pair_[i].resize(4);
        unit_pos_pair_[i][0] = current_unit_camera_pos[0];
        unit_pos_pair_[i][1] = current_unit_camera_pos[1];
        unit_pos_pair_[i][2] = current_unit_world_pos[0];
        unit_pos_pair_[i][3] = current_unit_world_pos[1];
    }

    pos_pair_inited_ = true;
}

void CameraPosToWorld::normal(MatrixXd& P, Matrix3d& T)
{
    double cx = P.col(0).mean();
    double cy = P.col(1).mean();

    P.array().col(0) -= cx;
    P.array().col(1) -= cy;

    double stdx = sqrt((P.col(0).transpose() * P.col(0)).mean());
    double stdy = sqrt((P.col(1).transpose() * P.col(1)).mean());


    double sqrt_2 = sqrt(2);
    double scalex = sqrt_2 / stdx;
    double scaley = sqrt_2 / stdy;

    P.array().col(0) *= scalex;
    P.array().col(1) *= scalex;

    T << scalex, 0, -scalex*cx,
    0, scaley, -scaley*cy,
    0, 0, 1;
}

VectorXd CameraPosToWorld::solveHomographyDLT(MatrixXd& srcNormal, MatrixXd& dstNormal)
{
    int n = srcNormal.rows();
    // 2. DLT
    MatrixXd input ( 2*n, 9 );

    for ( int i=0; i<n; ++i ) {

        input ( 2*i, 0 ) = 0.;
        input ( 2*i, 1 ) = 0.;
        input ( 2*i, 2 ) = 0.;
        input ( 2*i, 3 ) = srcNormal ( i, 0 );
        input ( 2*i, 4 ) = srcNormal ( i, 1 );
        input ( 2*i, 5 ) = 1.;
        input ( 2*i, 6 ) = -srcNormal ( i, 0 ) * dstNormal ( i, 1 );
        input ( 2*i, 7 ) = -srcNormal ( i, 1 ) * dstNormal ( i, 1 );
        input ( 2*i, 8 ) = -dstNormal ( i, 1 );

        input ( 2*i+1, 0 ) = srcNormal ( i, 0 );
        input ( 2*i+1, 1 ) = srcNormal ( i, 1 );
        input ( 2*i+1, 2 ) = 1.;
        input ( 2*i+1, 3 ) = 0.;
        input ( 2*i+1, 4 ) = 0.;
        input ( 2*i+1, 5 ) = 0.;
        input ( 2*i+1, 6 ) = -srcNormal ( i, 0 ) * dstNormal ( i, 0 );
        input ( 2*i+1, 7 ) = -srcNormal ( i, 1 ) * dstNormal ( i, 0 );
        input ( 2*i+1, 8 ) = -dstNormal ( i, 0 );
    }

 // 3. SVD分解
    JacobiSVD<MatrixXd> svdSolver ( input, ComputeThinU | ComputeThinV );
    MatrixXd V = svdSolver.matrixV();
    return V.rightCols ( 1 );
}

bool CameraPosToWorld::updateTransMatrix()
{
    if(unit_pos_pair_.size() < 4)
    {
        cout << "CameraPosToWorld::computeTransMatrix : pos pair num not enough." << endl;
        return false;
    }

    MatrixXd A(2 * unit_pos_pair_.size(), 9);
    MatrixXd b(2 * unit_pos_pair_.size(), 1);

    for(int i = 0; i < unit_pos_pair_.size(); ++i)
    {
        A(2 * i, 0) = unit_pos_pair_[i][0];
        A(2 * i, 1) = unit_pos_pair_[i][1];
        A(2 * i, 2) = 1.0;
        A(2 * i, 3) = 0.0;
        A(2 * i, 4) = 0.0;
        A(2 * i, 5) = 0.0;
        A(2 * i, 6) = -1.0 * unit_pos_pair_[i][0] * unit_pos_pair_[i][2];
        A(2 * i, 7) = -1.0 * unit_pos_pair_[i][1] * unit_pos_pair_[i][2];
        A(2 * i, 8) = -1.0 * unit_pos_pair_[i][2];

        A(2 * i + 1, 0) = 0.0;
        A(2 * i + 1, 1) = 0.0;
        A(2 * i + 1, 2) = 0.0;
        A(2 * i + 1, 3) = unit_pos_pair_[i][0];
        A(2 * i + 1, 4) = unit_pos_pair_[i][1];
        A(2 * i + 1, 5) = 1.0;
        A(2 * i + 1, 6) = -1.0 * unit_pos_pair_[i][0] * unit_pos_pair_[i][3];
        A(2 * i + 1, 7) = -1.0 * unit_pos_pair_[i][1] * unit_pos_pair_[i][3];
        A(2 * i + 1, 8) = -1.0 * unit_pos_pair_[i][3];

        b(2 * i, 0) = 0.0;
        b(2 * i + 1, 0) = 0.0;
    }

    JacobiSVD<MatrixXd> svd(A, ComputeFullV);

    MatrixXd V = svd.matrixV();

    MatrixXd x = V.col(8);

    for(int i = 0; i < 9; ++i)
    {
        x(i, 0) /= x(8, 0);
    }

    trans_matrix_(0, 0) = x(0, 0);
    trans_matrix_(0, 1) = x(1, 0);
    trans_matrix_(0, 2) = x(2, 0);
    trans_matrix_(1, 0) = x(3, 0);
    trans_matrix_(1, 1) = x(4, 0);
    trans_matrix_(1, 2) = x(5, 0);
    trans_matrix_(2, 0) = x(6, 0);
    trans_matrix_(2, 1) = x(7, 0);
    trans_matrix_(2, 2) = x(8, 0);

    trans_matrix_inverse_ = trans_matrix_.inverse();

    return true;
}
