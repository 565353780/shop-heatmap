#ifndef MULTIVIDEO_MANAGER_H
#define MULTIVIDEO_MANAGER_H

#include "MultiVideo_Manager/OpenCV_VideoServer.h"

class MultiVideo_Manager
{    
public:
    struct VideoServerWithID
    {
        OpenCV_VideoServer* video_server;
        int id;
        Mat image;
    };

public:
    MultiVideo_Manager();
    ~MultiVideo_Manager();

    void init();

    bool openVideoWithID(string video_path, int id, bool all_frame_used);
    bool openVideosWithIDs(vector<string> video_path_vector, vector<int> id_vector, vector<bool> all_frame_used_vector);

    bool changeVideoID(int source_id, int target_id);

    bool updateImage();

    Mat& getImageWithID(int id);

    double getPropWithID(int prop_id, int id);

private:
    vector<VideoServerWithID> video_server_vector_;
};

#endif // MULTIVIDEO_MANAGER_H
