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

    //temporary atribute
    int counter = 0;
    int ang = 0;

private:
    Vision();
    static Vision *m_Instance;

    cv::Size2i m_DisplaySize;
    cv::VideoCapture m_VideoCapture;
    cv::Mat m_RawFrame;
    cv::Mat m_FacesFrame;
    cv::Mat m_HSVFrame;
    cv::Mat m_BinaryFrame;
    int m_IdCamera;
};

#endif // VISION_H
