#-------------------------------------------------
#
# Project created by QtCreator 2016-04-12T14:41:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Binary_cam
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_videoio -lopencv_imgproc

#INCLUDEPATH += /usr/local/include/opencv
#LIBS += -L/usr/local/lib
#LIBS += -lopencv_core
#LIBS += -lopencv_imgproc
#LIBS += -lopencv_highgui
#LIBS += -lopencv_ml
#LIBS += -lopencv_video
#LIBS += -lopencv_videoio
#LIBS += -lopencv_features2d
#LIBS += -lopencv_calib3d
#LIBS += -lopencv_objdetect
#LIBS += -lopencv_contrib
#LIBS += -lopencv_legacy
#LIBS += -lopencv_flann
#LIBS += -lopencv_nonfree

SOURCES += main.cpp\
        mainwindow.cpp \
    vision.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    vision.h \
    utils.h

FORMS    += mainwindow.ui
