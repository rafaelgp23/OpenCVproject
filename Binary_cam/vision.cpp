#include "vision.h"
#include "math.h"

using namespace std;

Vision* Vision::m_Instance = new Vision();

Vision::Vision()
{
    m_EyeCascade.load("/usr/local/share/OpenCV/haarcascades/haarcascade_eye.xml");
    m_FacesCascade.load("/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml");
    m_IdCamera = 0;
    m_Angle = 0;
    m_SteadyControl = -1;
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
    m_Eyes.clear();
    m_Faces.clear();

    cv::cvtColor(m_RawFrame, m_GrayFrame, cv::COLOR_BGR2GRAY);
    cv::equalizeHist( m_GrayFrame, m_GrayFrame );

    //detect eyes
    m_EyeCascade.detectMultiScale( m_GrayFrame, m_Eyes, 1.1, 5, 0|cv::CASCADE_SCALE_IMAGE , Size(25, 25));

    for(int i=0;i<m_Eyes.size();++i){
        cv::Point eyeCenter( m_Eyes[i].x + m_Eyes[i].width*0.5, m_Eyes[i].y + m_Eyes[i].height*0.5 );
        cv::ellipse( m_FacesFrame, eyeCenter, Size( m_Eyes[i].width*0.5, m_Eyes[i].height*0.5), 0, 0, 360, Scalar( 0, 255, 255 ), 4, 8, 0 );
    }

    combineEyes();

    for(int i=0;i<m_Eyes.size();i+=2){
        cv::Point leftEye( m_Eyes[i].x + m_Eyes[i].width*0.5, m_Eyes[i].y + m_Eyes[i].height*0.5 );
        cv::Point rightEye( m_Eyes[i+1].x + m_Eyes[i+1].width*0.5, m_Eyes[i+1].y + m_Eyes[i+1].height*0.5 );
        cv::line(m_FacesFrame,leftEye,rightEye, Scalar( 0, 0, 255 ),2);
    }

    //detect faces

    //ignores false detection and, in this case, keeps the value of angle
    if(!m_Eyes.empty()){
        double newAng = atan((double)(m_Eyes[0].y - m_Eyes[1].y) / (double)(m_Eyes[0].x - m_Eyes[1].x));
        if(m_SteadyControl<0 || abs(newAng*180/M_PI-m_Angle*180/M_PI)<30){
            m_Angle = newAng;
            m_SteadyControl = 1;
        }
        else
            m_SteadyControl--;
    }
    std::cout<<"m_Angle: "<<(int)(m_Angle*180/M_PI)<<std::endl;

    m_FacesCascade.detectMultiScale( m_GrayFrame, m_Faces, 1.3, 5, 0|cv::CASCADE_SCALE_IMAGE , Size(30, 30));

    for(int i=0;i<m_Faces.size();++i){
        cv::rectangle(m_FacesFrame, m_Faces[i], Scalar( 255, 0, 0 ), 4, 8, 0);
    }

    //int ang = 0;
    //    if(m_Faces/2 > 0) // two eyes at least
    //    {
    //        ang = atan( (leftEye.y - rightEye.y) / (leftEye.x - rightEye/x) );
    //    }

    int bound_w = int((m_GrayFrame.size().height * fabs(sin(m_Angle))) + (m_GrayFrame.size().width * fabs(cos(m_Angle))));
    int bound_h = int((m_GrayFrame.size().height * fabs(cos(m_Angle))) + (m_GrayFrame.size().width * fabs(sin(m_Angle))));

    cv::Mat rotate = cv::getRotationMatrix2D(cv::Point2f(m_GrayFrame.cols/2,m_GrayFrame.rows/2), m_Angle*180/M_PI, 1.0);
    rotate.at<double>(0,2) += bound_w / 2 - m_GrayFrame.cols/2;
    rotate.at<double>(1,2) += bound_h / 2 - m_GrayFrame.rows/2;

    warpAffine(m_GrayFrame, m_GrayFrame, rotate, Size2i(bound_w, bound_h));

    //cv::CascadeClassifier faceCascade("/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml");

    //std::cout<<bound_h<<" "<<bound_w<<std::endl;

    imshow("rotated",m_GrayFrame);
    char k = cv::waitKey(1);
    if(k == 27) //esc
        exit(EXIT_SUCCESS);
}

void Vision::combineEyes(){
    //combine pair of eyes
    int minDist[m_Eyes.size()];
    int aux, dist;
    cv::Point c1, c2;

    for(int i=0;i<m_Eyes.size();++i){
        aux = INT32_MAX;
        for(int j=0;j<m_Eyes.size();++j)
            if(i!=j){
                c1 = cv::Point( m_Eyes[i].x + m_Eyes[i].width*0.5, m_Eyes[i].y + m_Eyes[i].height*0.5 );
                c2 = cv::Point( m_Eyes[j].x + m_Eyes[j].width*0.5, m_Eyes[j].y + m_Eyes[j].height*0.5 );
                dist = sqrt(pow(c2.x-c1.x,2)+pow(c2.y-c1.y,2));
                if(dist<aux){
                    aux = dist;
                    minDist[i] = j;
                }
            }
    }

    std::vector<Rect> combinedEyes;
    for(int i=0;i<m_Eyes.size();++i)
        if(minDist[i]<m_Eyes.size() && i == minDist[minDist[i]]){ //minDist are the same?
            combinedEyes.push_back(m_Eyes[i]);
            combinedEyes.push_back(m_Eyes[minDist[i]]);
            minDist[i] = m_Eyes.size();
            minDist[minDist[i]] = m_Eyes.size();
        }
    m_Eyes.clear();
    m_Eyes = combinedEyes;

    //define left and right eye

    for(int i=0;i<m_Eyes.size();i+=2)
        if(m_Eyes[i].x>m_Eyes[i+1].x){
            swap(m_Eyes[i],m_Eyes[i+1]);
        }

}
