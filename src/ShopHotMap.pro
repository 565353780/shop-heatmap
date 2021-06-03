#-------------------------------------------------
#
# Project created by QtCreator 2020-12-18T17:10:42
#
#-------------------------------------------------

QT       += core gui network widgets sql

win32{
DESTDIR = ../bin_win
}
unix{
DESTDIR = ../bin_linux
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShopHotMap
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    MultiView_People_Detector.cpp \
    main.cpp \
    MainWindow.cpp \
    DatabaseManager/DatabaseManager.cpp \
    DataManager/DataManager.cpp \
    DataManager/DataManagerWidget.cpp

HEADERS += \
    MainWindow.h \
    DatabaseManager/DatabaseManager.h \
    DataManager/DataManager.h \
    DataManager/DataManagerWidget.h \
    MultiView_People_Detector.h

FORMS += \
    MainWindow.ui \
    DataManager/DataManagerWidget.ui

# MySql
win32{
LIBS += \
    -L"C:\Program Files\MySQL\MySQL Server 8.0\lib" \
    -llibmysql
}

# OpenCV
INCLUDEPATH += \
    /home/chli/OpenCV/opencv-3.4.0/build/installed/include \
    /home/chli/OpenCV/opencv-3.4.0/build/installed/include/opencv \
    /home/chli/OpenCV/opencv-3.4.0/build/installed/include/opencv2

LIBS += \
    -L/home/chli/OpenCV/opencv-3.4.0/build/installed/lib \
    -lopencv_highgui \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_imgcodecs \
    -lopencv_video \
    -lopencv_videoio

# CameraPosToWorld
SOURCES += \
#    CameraPosToWorld/Example.cpp \
    CameraPosToWorld/CameraPosToWorld.cpp

HEADERS += \
#    CameraPosToWorld/Example.h \
    CameraPosToWorld/CameraPosToWorld.h

INCLUDEPATH += \
    CameraPosToWorld/eigen-3.3.7

# MultiCameraPosToWorld
SOURCES += \
#    MultiCameraPosToWorld/Example.cpp \
    MultiCameraPosToWorld/MultiCameraPosToWorld.cpp \
    MultiCameraPosToWorld/MultiCameraPosToWorldWidget.cpp

HEADERS += \
#    MultiCameraPosToWorld/Example.h \
    MultiCameraPosToWorld/MultiCameraPosToWorld.h \
    MultiCameraPosToWorld/MultiCameraPosToWorldWidget.h

FORMS += \
    MultiCameraPosToWorld/MultiCameraPosToWorldWidget.ui

# CVMatToQImage
SOURCES += \
#    CVMatToQImage/Example.cpp \
    CVMatToQImage/CVMatToQImage.cpp

HEADERS += \
#    CVMatToQImage/Example.h \
    CVMatToQImage/CVMatToQImage.h

# MultiVideo_Manager
SOURCES += \
#    MultiVideo_Manager/Example.cpp \
    MultiVideo_Manager/OpenCV_GetImage.cpp \
    MultiVideo_Manager/OpenCV_VideoServer.cpp \
    MultiVideo_Manager/MultiVideo_Manager.cpp

HEADERS += \
#    MultiVideo_Manager/Example.h \
    MultiVideo_Manager/OpenCV_GetImage.h \
    MultiVideo_Manager/OpenCV_VideoServer.h \
    MultiVideo_Manager/MultiVideo_Manager.h

# HttpApi
QT += network websockets

SOURCES += \
#    HttpApi/Example.cpp \
    HttpApi/HttpApi.cpp \
    HttpApi/MySocketClient.cpp \
    HttpApi/MySocketServer.cpp \
    HttpApi/SimpleClient.cpp \
    HttpApi/SimpleServer.cpp

HEADERS += \
#    HttpApi/Example.h \
    HttpApi/HttpApi.h \
    HttpApi/MySocketClient.h \
    HttpApi/MySocketServer.h \
    HttpApi/SimpleClient.h \
    HttpApi/SimpleServer.h

# Darknet
SOURCES += \
#    Darknet_Detector/Example.cpp \
    Darknet_Detector/Darknet_Detector.cpp \
#    Darknet_Detector/DarknetDetector.cpp
    Darknet_Detector/darknet/src/softmax_layer.c \
    Darknet_Detector/darknet/src/l2norm_layer.c \
    Darknet_Detector/darknet/src/yolo_layer.c \
    Darknet_Detector/darknet/src/region_layer.c \
    Darknet_Detector/darknet/src/shortcut_layer.c \
    Darknet_Detector/darknet/src/parser.c \
    Darknet_Detector/darknet/src/detection_layer.c \
    Darknet_Detector/darknet/src/avgpool_layer.c \
    Darknet_Detector/darknet/src/convolutional_layer.c \
    Darknet_Detector/darknet/src/maxpool_layer.c \
    Darknet_Detector/darknet/src/lstm_layer.c \
    Darknet_Detector/darknet/src/crop_layer.c \
    Darknet_Detector/darknet/src/cost_layer.c \
    Darknet_Detector/darknet/src/blas.c \
    Darknet_Detector/darknet/src/activation_layer.c \
    Darknet_Detector/darknet/src/route_layer.c \
    Darknet_Detector/darknet/src/image.c \
    Darknet_Detector/darknet/src/activations.c \
    Darknet_Detector/darknet/src/list.c \
    Darknet_Detector/darknet/src/gemm.c \
    Darknet_Detector/darknet/src/batchnorm_layer.c \
    Darknet_Detector/darknet/src/rnn_layer.c \
    Darknet_Detector/darknet/src/data.c \
    Darknet_Detector/darknet/src/network.c \
    Darknet_Detector/darknet/src/cuda.c \
    Darknet_Detector/darknet/src/crnn_layer.c \
    Darknet_Detector/darknet/src/im2col.c \
    Darknet_Detector/darknet/src/dropout_layer.c \
    Darknet_Detector/darknet/src/deconvolutional_layer.c \
    Darknet_Detector/darknet/src/local_layer.c \
    Darknet_Detector/darknet/src/utils.c \
    Darknet_Detector/darknet/src/image_opencv.cpp \
    Darknet_Detector/darknet/src/upsample_layer.c \
    Darknet_Detector/darknet/src/option_list.c \
    Darknet_Detector/darknet/src/gru_layer.c \
    Darknet_Detector/darknet/src/tree.c \
    Darknet_Detector/darknet/src/normalization_layer.c \
    Darknet_Detector/darknet/src/box.c \
    Darknet_Detector/darknet/src/connected_layer.c \
    Darknet_Detector/darknet/src/matrix.c \
    #Darknet_Detector/darknet/src/compare.c \
    Darknet_Detector/darknet/src/col2im.c \
    Darknet_Detector/darknet/src/iseg_layer.c \
    Darknet_Detector/darknet/src/logistic_layer.c \
    Darknet_Detector/darknet/src/demo.c \
    Darknet_Detector/darknet/src/layer.c \
    Darknet_Detector/darknet/src/reorg_layer.c \
    Darknet_Detector/darknet/examples/regressor.c \
    Darknet_Detector/darknet/examples/nightmare.c \
    Darknet_Detector/darknet/examples/attention.c \
    #Darknet_Detector/darknet/examples/swag.c \
    Darknet_Detector/darknet/examples/art.c \
    Darknet_Detector/darknet/examples/super.c \
    Darknet_Detector/darknet/examples/rnn.c \
    Darknet_Detector/darknet/examples/lsd.c \
    #Darknet_Detector/darknet/examples/dice.c \
    Darknet_Detector/darknet/examples/go.c \
    Darknet_Detector/darknet/examples/yolo.c \
    Darknet_Detector/darknet/examples/captcha.c \
    Darknet_Detector/darknet/examples/detector.c \
    Darknet_Detector/darknet/examples/tag.c \
    Darknet_Detector/darknet/examples/darknet.c \
    #Darknet_Detector/darknet/examples/rnn_vid.c \
    Darknet_Detector/darknet/examples/classifier.c \
    Darknet_Detector/darknet/examples/segmenter.c \
    #Darknet_Detector/darknet/examples/voxel.c \
    Darknet_Detector/darknet/examples/coco.c \
    Darknet_Detector/darknet/examples/cifar.c \
    Darknet_Detector/darknet/examples/instance-segmenter.c \
    #Darknet_Detector/darknet/examples/writing.c

HEADERS += \
#    Darknet_Detector/Example.h \
    Darknet_Detector/Darknet_Detector.h \
#    Darknet_Detector/DarknetDetector.h
    Darknet_Detector/darknet/src/cost_layer.h \
    Darknet_Detector/darknet/src/crnn_layer.h \
    Darknet_Detector/darknet/src/layer.h \
    Darknet_Detector/darknet/src/utils.h \
    Darknet_Detector/darknet/src/maxpool_layer.h \
    Darknet_Detector/darknet/src/data.h \
    Darknet_Detector/darknet/src/batchnorm_layer.h \
    Darknet_Detector/darknet/src/network.h \
    Darknet_Detector/darknet/src/list.h \
    Darknet_Detector/darknet/src/avgpool_layer.h \
    Darknet_Detector/darknet/src/region_layer.h \
    Darknet_Detector/darknet/src/stb_image.h \
    Darknet_Detector/darknet/src/shortcut_layer.h \
    Darknet_Detector/darknet/src/lstm_layer.h \
    Darknet_Detector/darknet/src/crop_layer.h \
    Darknet_Detector/darknet/src/gemm.h \
    Darknet_Detector/darknet/src/im2col.h \
    Darknet_Detector/darknet/src/rnn_layer.h \
    Darknet_Detector/darknet/src/blas.h \
    Darknet_Detector/darknet/src/normalization_layer.h \
    Darknet_Detector/darknet/src/reorg_layer.h \
    Darknet_Detector/darknet/src/tree.h \
    Darknet_Detector/darknet/src/local_layer.h \
    Darknet_Detector/darknet/src/col2im.h \
    Darknet_Detector/darknet/src/demo.h \
    Darknet_Detector/darknet/src/route_layer.h \
    Darknet_Detector/darknet/src/upsample_layer.h \
    Darknet_Detector/darknet/src/convolutional_layer.h \
    Darknet_Detector/darknet/src/iseg_layer.h \
    Darknet_Detector/darknet/src/stb_image_write.h \
    Darknet_Detector/darknet/src/parser.h \
    Darknet_Detector/darknet/src/logistic_layer.h \
    Darknet_Detector/darknet/src/gru_layer.h \
    Darknet_Detector/darknet/src/image.h \
    Darknet_Detector/darknet/src/option_list.h \
    Darknet_Detector/darknet/src/activation_layer.h \
    Darknet_Detector/darknet/src/activations.h \
    Darknet_Detector/darknet/src/softmax_layer.h \
    Darknet_Detector/darknet/src/yolo_layer.h \
    Darknet_Detector/darknet/src/connected_layer.h \
    Darknet_Detector/darknet/src/dropout_layer.h \
    Darknet_Detector/darknet/src/classifier.h \
    Darknet_Detector/darknet/src/detection_layer.h \
    Darknet_Detector/darknet/src/matrix.h \
    Darknet_Detector/darknet/src/deconvolutional_layer.h \
    Darknet_Detector/darknet/src/l2norm_layer.h \
    Darknet_Detector/darknet/src/cuda.h \
    Darknet_Detector/darknet/src/box.h \
    Darknet_Detector/darknet/include/darknet.h

INCLUDEPATH += \
    $$PWD/Darknet_Detector/darknet/include \
    $$PWD/Darknet_Detector/darknet/src

CUDA_SOURCES += \
    Darknet_Detector/darknet/src/blas_kernels.cu \
    Darknet_Detector/darknet/src/deconvolutional_kernels.cu \
    Darknet_Detector/darknet/src/activation_kernels.cu \
    Darknet_Detector/darknet/src/convolutional_kernels.cu \
    Darknet_Detector/darknet/src/avgpool_layer_kernels.cu \
    Darknet_Detector/darknet/src/col2im_kernels.cu \
    Darknet_Detector/darknet/src/crop_layer_kernels.cu \
    Darknet_Detector/darknet/src/maxpool_layer_kernels.cu \
    Darknet_Detector/darknet/src/im2col_kernels.cu \
    Darknet_Detector/darknet/src/dropout_layer_kernels.cu

CUDA_SDK = "/usr/local/cuda-10.0" # Path to cuda SDK install
CUDA_DIR = "/usr/local/cuda-10.0" # Path to cuda toolkit install

# DO NOT EDIT BEYOND THIS UNLESS YOU KNOW WHAT YOU ARE DOING....

SYSTEM_NAME = ubuntu         # Depending on your system either 'Win32', 'x64', or 'Win64'
SYSTEM_TYPE = 64            # '32' or '64', depending on your system
CUDA_ARCH = sm_50           # Type of CUDA architecture, for example 'compute_10', 'compute_11', 'sm_10'
NVCC_OPTIONS = --use_fast_math

# include paths
INCLUDEPATH += $$CUDA_DIR/include \

# library directories
QMAKE_LIBDIR += $$CUDA_DIR/lib64/

CUDA_OBJECTS_DIR = ./


# Add the necessary libraries
#CUDA_LIBS = -lcuda -lcudart
CUDA_LIBS += -lcudart -lcufft -lcuda -lcublas -lcurand -lcudnn

# The following makes sure all path names (which often include spaces) are put between quotation marks
CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')
#LIBS += $$join(CUDA_LIBS,'.so ', '', '.so')
LIBS += \
    -L/usr/local/cuda-10.0/lib64 \
    $$CUDA_LIBS

# Configuration of the Cuda compiler
CONFIG(debug, debug|release) {
    # Debug mode
    cuda_d.input = CUDA_SOURCES
    cuda_d.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda_d.commands = $$CUDA_DIR/bin/nvcc -D_DEBUG $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda_d.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda_d
}
else {
    # Release mode
    cuda.input = CUDA_SOURCES
    cuda.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda.commands = $$CUDA_DIR/bin/nvcc $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda
}

DISTFILES += \
    Darknet_Detector/darknet/examples/detector-scipy-opencv.py \
    Darknet_Detector/darknet/examples/detector.py

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
