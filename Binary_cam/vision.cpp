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

void Vision::captureImage(){
    while (!m_VideoCapture.isOpened())
        m_VideoCapture.open(m_IdCamera);

    m_VideoCapture >> m_RawFrame;

    //    Mat r = cv::getRotationMatrix2D(cv::Point2f(m_RawFrame.cols/2,m_RawFrame.rows/2), 10, 1.0);
    //    warpAffine(m_RawFrame, m_RawFrame, r, Size2i(1280,720));

    //resize image to the display size
    resize(m_RawFrame, m_RawFrame, m_DisplaySize);
}

void Vision::faceDetect(){
    m_RawFrame.copyTo(m_FacesFrame);
    Mat grayFrame;

    cv::CascadeClassifier faceCascade("/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml");
    cv::cvtColor(m_RawFrame, grayFrame, cv::COLOR_BGR2GRAY);
    cv::equalizeHist( grayFrame, grayFrame );
    //m_GrayFrame = grayFrame;

    std::vector<Rect> faces;

    faceCascade.detectMultiScale( grayFrame, faces, 1.3, 5, 0|cv::CASCADE_SCALE_IMAGE , Size(50, 50) );

    //move this part to mainwindow.cpp \/
    for(int i=0;i<faces.size();++i){
        cv::Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
        ellipse( m_FacesFrame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    }
}

//undone
void Vision::thresholding()
{
    //convert image color model from BRG to HSV
    cv::cvtColor(m_RawFrame, m_HSVFrame, cv::COLOR_BGR2HSV);

    //threshold image by previously setted HSV interval

    //cv::inRange(m_RawFrame,,,m_BinaryFrame);
}
