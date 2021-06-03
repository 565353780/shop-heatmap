#ifndef MULTIVIEW_PEOPLE_DETECTOR_H
#define MULTIVIEW_PEOPLE_DETECTOR_H

#include <QObject>
#include <QJsonObject>

#include "Darknet_Detector/Darknet_Detector.h"
#include "MultiVideo_Manager/MultiVideo_Manager.h"
#include "CVMatToQImage/CVMatToQImage.h"
#include "MultiCameraPosToWorld/MultiCameraPosToWorld.h"
//#include "MultiCameraPosToWorld/MultiCameraPosToWorldWidget.h"
#include "DataManager/DataManager.h"

class MultiView_People_Detector : public QObject
{
    Q_OBJECT

public:
    explicit MultiView_People_Detector(QObject *parent = nullptr);
    ~MultiView_People_Detector();

    void init_darknet_detector(string cfg_file, string weights_file);

    void openMultiCamera(bool all_frame_used);

private:
    void open_video_with_id(string video_path, int id, bool all_frame_used);

    void detect();

public slots:
    void slot_updateCameraParam();
    void slot_startDetect();
    void slot_stopDetect();

signals:
    void signal_findPeople(double x, double y);

    void signal_update();

public:
    QImage show_image;

private:
    Darknet_Detector darknet_detector_;
    MultiVideo_Manager multivideo_manager_;
    CVMatToQImage cv_mat_to_qimage_;
    MultiCameraPosToWorld multi_camera_pos_to_world_;
//    MultiCameraPosToWorldWidget* multi_camera_pos_to_world_widget_;
    DataManager* data_manager_;
    QThread* data_manager_thread_;

    bool open_camera_finished_;
    bool camera_ready_;
    bool keep_detect_;
};

#endif // MULTIVIEW_PEOPLE_DETECTOR_H
