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
    bool captureImage();
    void thresholding();

private:
    Vision();
    static Vision *m_Instance;

    VideoCapture m_VideoCapture;
    cv::Mat m_RawFrame;
    cv::Mat m_HSVFrame;
    cv::Mat m_BinaryFrame;
    int m_IdCamera;

};

#endif // VISION_H
