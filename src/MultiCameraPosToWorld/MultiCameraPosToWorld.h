#ifndef MULTICAMERAPOSTOWORLD_H
#define MULTICAMERAPOSTOWORLD_H

#include <opencv2/opencv.hpp>
using namespace cv;

#include "CameraPosToWorld/CameraPosToWorld.h"

class MultiCameraPosToWorld
{    
public:
    MultiCameraPosToWorld();
    ~MultiCameraPosToWorld();

    void init();

    void addCameraPosToWorldWithtID(int id);

    void setCameraBoundingBoxWithID(double x_min, double y_min, double x_max, double y_max, int id);
    void setWorldBoundingBoxWithID(double x_min, double y_min, double x_max, double y_max, int id);

    bool matchPosPairWithID(vector<vector<double>> pos_pair_camera, vector<vector<double>> pos_pair_world, int id);

    int getOpenedVideoNum();

    int getOpenedVideoID(int video_idx);

    vector<double> getWorldPosFromCameraWithID(double x, double y, int id);

    MatrixXd getTransMatrixWithID(int id);

    bool haveThisCameraID(int id);

private:
    vector<int> opened_video_id_vector_;
    vector<CameraPosToWorld> camera_pos_to_world_vector_;

private:
    struct Target
    {
        int id;
        double x;
        double y;
        bool visiable;

        vector<double> x_in_multicamera;
        vector<double> y_in_multicamera;
        vector<bool> visiable_in_multicamera;
    };

public:
    vector<vector<double>> getWorldTargetPos(vector<vector<vector<double>>> all_target_pos_vec);

    Mat getWorldTargetPosMap(int image_width, int image_height);

private:
    bool haveThisTargetID(int id);
    void addTarget(double x, double y, int first_find_camera_idx);
    void removeTargetWithId(int target_id);

    double getDist(vector<double> target_pos_1, vector<double> target_pos_2);

    void initTargetList();

    void mergeMultiTargetWithCameraIdx(vector<vector<double>> target_pos_vec, int camera_idx);

    void matchMultiTargetPos(vector<vector<vector<double>>> all_target_pos_vec);

    void showWorldTargetPos(int image_width, int image_height);

private:
    std::list<Target> target_list_;

    double max_target_dist_;
};

#endif // MULTICAMERAPOSTOWORLD_H
