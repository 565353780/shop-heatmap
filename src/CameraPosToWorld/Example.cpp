#include "Example.h"

Example::Example()
{

}

Example::~Example()
{

}

void Example::run_example()
{
    vector<double> camera_bbox;
    camera_bbox.resize(4);

    camera_bbox[0] = 0.0;
    camera_bbox[1] = 0.0;

    camera_bbox[2] = 400.0;
    camera_bbox[3] = 400.0;

    vector<double> world_bbox;
    world_bbox.resize(4);

    world_bbox[0] = 0.0;
    world_bbox[1] = 0.0;

    world_bbox[2] = 400.0;
    world_bbox[3] = 400.0;

    vector<vector<double>> pos_pair_camera;
    pos_pair_camera.resize(4);
    for(int i = 0; i < pos_pair_camera.size(); ++i)
    {
        pos_pair_camera[i].resize(2);
    }

    pos_pair_camera[0][0] = 0.0;
    pos_pair_camera[0][1] = 0.0;

    pos_pair_camera[1][0] = 400.0;
    pos_pair_camera[1][1] = 0.0;

    pos_pair_camera[2][0] = 0.0;
    pos_pair_camera[2][1] = 400.0;

    pos_pair_camera[3][0] = 400.0;
    pos_pair_camera[3][1] = 400.0;

    vector<vector<double>> pos_pair_world;
    pos_pair_world.resize(4);
    for(int i = 0; i < pos_pair_world.size(); ++i)
    {
        pos_pair_world[i].resize(2);
    }

    pos_pair_world[0][0] = 0.0;
    pos_pair_world[0][1] = 0.0;

    pos_pair_world[1][0] = 400.0;
    pos_pair_world[1][1] = 0.0;

    pos_pair_world[2][0] = 190.0;
    pos_pair_world[2][1] = 400.0;

    pos_pair_world[3][0] = 210.0;
    pos_pair_world[3][1] = 400.0;

    CameraPosToWorld camera_pos_to_world;

    camera_pos_to_world.setCameraAndWorldBoundingBox(camera_bbox, world_bbox);

    bool match_success = camera_pos_to_world.matchPosPair(pos_pair_camera, pos_pair_world);

    if(match_success)
    {
        cout << camera_pos_to_world.getTransMatrix() << endl;

        for(int i = 0; i < 10; ++i)
        {
            double camera_x = 40 * i;
            double camera_y = 200;
            vector<double> world_pos = camera_pos_to_world.getWorldPosFromCamera(camera_x, camera_y);
            cout << camera_x << " , " << camera_y << " -> " <<  world_pos[0] << " , " << world_pos[1] << endl;
        }

        double camera_x = 0;
        double camera_y = 0;
        vector<double> world_pos = camera_pos_to_world.getWorldPosFromCamera(camera_x, camera_y);
        cout << camera_x << " , " << camera_y << " -> " <<  world_pos[0] << " , " << world_pos[1] << endl;

        camera_x = 0;
        camera_y = 400;
        world_pos = camera_pos_to_world.getWorldPosFromCamera(camera_x, camera_y);
        cout << camera_x << " , " << camera_y << " -> " <<  world_pos[0] << " , " << world_pos[1] << endl;

        camera_x = 400;
        camera_y = 0;
        world_pos = camera_pos_to_world.getWorldPosFromCamera(camera_x, camera_y);
        cout << camera_x << " , " << camera_y << " -> " <<  world_pos[0] << " , " << world_pos[1] << endl;

        camera_x = 400;
        camera_y = 400;
        world_pos = camera_pos_to_world.getWorldPosFromCamera(camera_x, camera_y);
        cout << camera_x << " , " << camera_y << " -> " <<  world_pos[0] << " , " << world_pos[1] << endl;
    }

    cout << "out" << endl;
}
