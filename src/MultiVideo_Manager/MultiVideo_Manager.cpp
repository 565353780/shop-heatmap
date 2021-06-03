#include "MultiVideo_Manager.h"

MultiVideo_Manager::MultiVideo_Manager()
{

}

MultiVideo_Manager::~MultiVideo_Manager()
{

}

void MultiVideo_Manager::init()
{
    if(video_server_vector_.size() > 0)
    {
        for(int i = 0; i < video_server_vector_.size(); ++i)
        {
            if(video_server_vector_[i].video_server != NULL)
            {
                video_server_vector_[i].video_server->stopRun();
                video_server_vector_[i].video_server->quit();
                video_server_vector_[i].video_server->wait();
                delete video_server_vector_[i].video_server;
                video_server_vector_[i].video_server = NULL;
            }
        }
    }
    video_server_vector_.clear();
}

bool MultiVideo_Manager::openVideoWithID(string video_path, int id, bool all_frame_used)
{
    if(video_server_vector_.size() > 0)
    {
        for(int i = 0; i < video_server_vector_.size(); ++i)
        {
            if(video_server_vector_[i].id == id)
            {
                cout << "MultiVideo_Manager::openVideowithID : Already have this id." << endl;
                return false;
            }
        }
    }

    VideoServerWithID video_servber_with_id;
    video_servber_with_id.video_server = new OpenCV_VideoServer();
    bool open_success = video_servber_with_id.video_server->openVideo(video_path, all_frame_used);

    if(!open_success)
    {
        cout << "MultiVideo_Manager::openVideowithID : Open video failed." << endl;
        return false;
    }

    video_servber_with_id.id = id;
    video_servber_with_id.video_server->start();

    video_server_vector_.emplace_back(video_servber_with_id);

    return true;
}

bool MultiVideo_Manager::openVideosWithIDs(vector<string> video_path_vector, vector<int> id_vector, vector<bool> all_frame_used_vector)
{
    if(video_path_vector.size() == 0)
    {
        cout << "MultiVideo_Manager::openVideosWithIDs : No video input." << endl;
        return false;
    }

    if(video_path_vector.size() != id_vector.size())
    {
        cout << "MultiVideo_Manager::openVideosWithIDs : Can't match videos and ids. Not same number." << endl;
        return false;
    }

    for(int i = 0; i < video_path_vector.size(); ++i)
    {
        bool open_video_succcess = openVideoWithID(video_path_vector[i], id_vector[i], all_frame_used_vector[i]);

        if(!open_video_succcess)
        {
            cout << "MultiVideo_Manager::openVideosWithIDs : Can't open " << id_vector[i] << " video." << endl;
            return false;
        }
    }

    return true;
}

bool MultiVideo_Manager::changeVideoID(int source_id, int target_id)
{
    if(video_server_vector_.size() > 0)
    {
        for(int i = 0; i < video_server_vector_.size(); ++i)
        {
            if(video_server_vector_[i].id == target_id)
            {
                cout << "MultiVideo_Manager::changeVideoID : Already have this target_id." << endl;
                return false;
            }
        }

        for(int i = 0; i < video_server_vector_.size(); ++i)
        {
            if(video_server_vector_[i].id == source_id)
            {
                video_server_vector_[i].id = target_id;
                return true;
            }
        }

        cout << "MultiVideo_Manager::changeVideoID : Don't have this source_id." << endl;
        return false;
    }

    cout << "MultiVideo_Manager::changeVideoID : No video is opened." << endl;
    return false;
}

bool MultiVideo_Manager::updateImage()
{
    if(video_server_vector_.size() == 0)
    {
        return true;
    }

    for(int i = 0; i < video_server_vector_.size(); ++i)
    {
        bool get_image_success = video_server_vector_[i].video_server->get_frame(video_server_vector_[i].image);

        if(!get_image_success)
        {
            cout << "MultiVideo_Manager::updateImage : Can't get " << video_server_vector_[i].id << "'s image." << endl;
            return false;
        }
    }

    return true;
}

Mat& MultiVideo_Manager::getImageWithID(int id)
{
    if(video_server_vector_.size() > 0)
    {
        for(int i = 0; i < video_server_vector_.size(); ++i)
        {
            if(video_server_vector_[i].id == id)
            {
                return video_server_vector_[i].image;
            }
        }
    }

    Mat image;
    cout << "MultiVideo_Manager::getImageWithID : Can't find video with this id." << endl;
    return image;
}

double MultiVideo_Manager::getPropWithID(int prop_id, int id)
{
    if(video_server_vector_.size() > 0)
    {
        for(int i = 0; i < video_server_vector_.size(); ++i)
        {
            if(video_server_vector_[i].id == id)
            {
                return video_server_vector_[i].video_server->getProp(prop_id);
            }
        }
    }

    cout << "MultiVideo_Manager::getPropWithID : No video with this id." << endl;
    return -1;
}
