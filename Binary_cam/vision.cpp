#include "vision.h"

Vision* Vision::m_Instance = new Vision();

Vision::Vision()
{
     m_IdCamera = 0;
}

Vision* Vision::getInstance()
{
    return m_Instance;
}

bool Vision::captureImage(){
    while (!m_VideoCapture.isOpened())
        m_VideoCapture.open(m_IdCamera);

    m_VideoCapture >> m_RawFrame;

    //resize image to the display size
    //resize(m_RawFrame, m_RawFrame, m_DisplaySize);
    return true;
}

void Vision::thresholding()
{
    //convert image color model from BRG to HSV
    cv::cvtColor(m_RawFrame, m_HSVFrame, cv::COLOR_BGR2HSV);

    //threshold image by previously setted HSV interval

    //cv::inRange(m_RawFrame,,,m_BinaryFrame);
}
