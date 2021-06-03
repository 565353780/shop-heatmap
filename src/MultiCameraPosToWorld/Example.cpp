#include "Example.h"

Example::Example()
{

}

Example::~Example()
{

}

void Example::run_example()
{
    vector<vector<double>> pos_pair_camera;
    vector<vector<double>> pos_pair_world;

    pos_pair_camera.resize(4);
    pos_pair_world.resize(4);
    for(int i = 0; i < pos_pair_camera.size(); ++i)
    {
        pos_pair_camera[i].resize(2);
        pos_pair_world[i].resize(2);
    }

    pos_pair_camera[0][0] = 0.0;
    pos_pair_camera[0][1] = 0.0;

    pos_pair_camera[1][0] = 0.0;
    pos_pair_camera[1][1] = 1.0;

    pos_pair_camera[2][0] = 1.0;
    pos_pair_camera[2][1] = 0.0;

    pos_pair_camera[3][0] = 1.0;
    pos_pair_camera[3][1] = 1.0;

    pos_pair_world[0][0] = 0.0;
    pos_pair_world[0][1] = 0.0;

    pos_pair_world[1][0] = 0.0;
    pos_pair_world[1][1] = 1.0;

    pos_pair_world[2][0] = 1.0;
    pos_pair_world[2][1] = 0.0;

    pos_pair_world[3][0] = 1.0;
    pos_pair_world[3][1] = 1.0;

    vector<vector<vector<double>>> all_target_pos_vec;
    all_target_pos_vec.resize(2);
    for(int i = 0; i < all_target_pos_vec.size(); ++i)
    {
        all_target_pos_vec[i].resize(1);
        for(int j = 0; j < all_target_pos_vec[i].size(); ++j)
        {
            all_target_pos_vec[i][j].resize(2);
        }
    }

    all_target_pos_vec[0][0][0] = 0.5;
    all_target_pos_vec[0][0][1] = 0.5;

    all_target_pos_vec[1][0][0] = 0.55;
    all_target_pos_vec[1][0][1] = 0.5;

    MultiCameraPosToWorld multi_camera_pos_to_world;

    for(int i = 0; i < all_target_pos_vec.size(); ++i)
    {
        multi_camera_pos_to_world.addCameraPosToWorldWithtID(i);
        multi_camera_pos_to_world.matchPosPairWithID(pos_pair_camera, pos_pair_world, i);
    }

    vector<vector<double>> world_target_pos_vec = multi_camera_pos_to_world.getWorldTargetPos(all_target_pos_vec);

    cout << world_target_pos_vec.size() << endl;

    MultiCameraPosToWorldWidget* multi_camera_pos_to_world_widget = new MultiCameraPosToWorldWidget();

    for(int i = 0; i < all_target_pos_vec.size(); ++i)
    {
        multi_camera_pos_to_world_widget->addTransMatrixWithID(multi_camera_pos_to_world.getTransMatrixWithID(i), i);
    }

    multi_camera_pos_to_world_widget->show();
}
