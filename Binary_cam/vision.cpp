#include "vision.h"
#include "math.h"

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
    //    while (!m_VideoCapture.isOpened())
    //        m_VideoCapture.open(m_IdCamera);

    m_VideoCapture >> m_RawFrame;

    //        Mat r = cv::getRotationMatrix2D(cv::Point2f(m_RawFrame.cols/2,m_RawFrame.rows/2), 90, 1.0);
    //        warpAffine(m_RawFrame, m_RawFrame, r, m_RawFrame.size());

    //resize image to the display size
    resize(m_RawFrame, m_RawFrame, m_DisplaySize);
}

void Vision::faceDetect(){
    m_RawFrame.copyTo(m_FacesFrame);
    Mat grayFrame;

    counter = (counter + 1)%360;
    if(counter%15==1){
        ang= (ang + 10) % 360;
        std::cout<<ang<<" "<<counter<<std::endl;
    }

    cv::CascadeClassifier faceCascade("/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml");
    cv::cvtColor(m_RawFrame, grayFrame, cv::COLOR_BGR2GRAY);
    cv::equalizeHist( grayFrame, grayFrame );

    //paralel?
    std::vector<Rect> faces;
    Mat rotate;

    int bound_w = int((grayFrame.size().height * fabs(sin(M_PI/4))) + (grayFrame.size().width * fabs(cos(M_PI/4))));
    int bound_h = int((grayFrame.size().height * fabs(cos(M_PI/4))) + (grayFrame.size().width * fabs(sin(M_PI/4))));

    rotate = cv::getRotationMatrix2D(cv::Point2f(grayFrame.cols/2,grayFrame.rows/2), ang, 1.0);
    rotate.at<double>(0,2) += bound_w / 2 - grayFrame.cols/2;
    rotate.at<double>(1,2) += bound_h / 2 - grayFrame.rows/2;

    warpAffine(grayFrame, grayFrame, rotate, Size2i(bound_w, bound_h));
    faceCascade.detectMultiScale( grayFrame, faces, 1.3, 5, 0|cv::CASCADE_SCALE_IMAGE , Size(50, 50) );

    //end paraelel

    imshow("rotated",grayFrame);

    //move this part to mainwindow.cpp \/
    for(int i=0;i<faces.size();++i){
        cv::Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
        ellipse( m_FacesFrame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 0 ), 4, 8, 0 );
    }
}
