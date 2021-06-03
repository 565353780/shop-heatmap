#include "Example.h"

Example::Example()
{

}

Example::~Example()
{

}

void Example::run_example()
{
    vector<string> video_path_vector;
    vector<int> id_vector;
    vector<bool> all_frame_used_vector;

    video_path_vector.emplace_back("http://192.168.0.203:8085/stream.mjpg");
    id_vector.emplace_back(1);
    all_frame_used_vector.emplace_back(false);

    video_path_vector.emplace_back("http://192.168.0.201:8083/stream.mjpg");
    id_vector.emplace_back(3);
    all_frame_used_vector.emplace_back(false);

    MultiVideo_Manager multivideo_manager;

    bool open_success = multivideo_manager.openVideosWithIDs(video_path_vector, id_vector, all_frame_used_vector);

    while(open_success)
    {
        bool get_image_success = multivideo_manager.updateImage();

        if(!get_image_success)
        {
            cout << "Example::run_example : get image failed." << endl;
            return;
        }

        for(int i = 0; i < video_path_vector.size(); ++i)
        {
            imshow("Video : " + to_string(id_vector[i]), multivideo_manager.getImageWithID(id_vector[i]));
        }

        waitKey(1);
    }
}
