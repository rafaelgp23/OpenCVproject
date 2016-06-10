#ifndef VISION_H
#define VISION_H

#include "opencv2/opencv.hpp"
#include "utils.h"

using namespace cv;

class Vision
{
    friend class MainWindow;

public:
    static Vision* getInstance();
    void convertImage(Mat target_frame);
    void captureImage();
    void faceDetect();
    void combineEyes();

private:
    Vision();
    static Vision *m_Instance;

    cv::CascadeClassifier m_EyeCascade;
    cv::CascadeClassifier m_FacesCascade;

    std::vector<Rect> m_Eyes;
    std::vector<Rect> m_Faces;
    double m_Angle;
    int m_SteadyControl;

    cv::Size2i m_DisplaySize;
    cv::VideoCapture m_VideoCapture;
    cv::Mat m_RawFrame;
    cv::Mat m_GrayFrame;
    cv::Mat m_FacesFrame;
    cv::Mat m_HSVFrame;
    cv::Mat m_BinaryFrame;
    int m_IdCamera;
};

#endif // VISION_H
