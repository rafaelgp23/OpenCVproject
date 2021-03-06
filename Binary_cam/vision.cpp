#include "vision.h"
#include "math.h"

using namespace std;

Vision* Vision::m_Instance = new Vision();

Vision::Vision()
{
    m_IdCamera = 0;
    while (!(m_VideoCapture.isOpened()))
        m_VideoCapture.open(m_IdCamera);

    m_CameraDefaultSettings[0] = m_VideoCapture.get(CV_CAP_PROP_BRIGHTNESS);
    m_CameraDefaultSettings[1] = m_VideoCapture.get(CV_CAP_PROP_CONTRAST);
    m_CameraDefaultSettings[2] = m_VideoCapture.get(CV_CAP_PROP_SATURATION);

    m_EyeCascade.load("/usr/local/share/OpenCV/haarcascades/haarcascade_eye.xml");
    m_FacesCascade.load("/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml");
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
    std::vector<Rect> faces;

    cv::cvtColor(m_RawFrame, m_GrayFrame, cv::COLOR_BGR2GRAY);
    cv::equalizeHist( m_GrayFrame, m_GrayFrame );

    //DETECT EYES
    m_EyeCascade.detectMultiScale( m_GrayFrame, m_Eyes, 1.1, 5, 0|cv::CASCADE_SCALE_IMAGE , Size(25, 25));

    combineEyes();//define pair of eyes and left and right eye of each pair

    //DETECT FACES

    //defines a angle for each pair of eye
    for(int i=m_Angles.size();i<m_Eyes.size()/2;++i)
        m_Angles.push_back(0);


    for(int i=0;i<m_Eyes.size();i+=2){ //For each pair of eyes

        //Ignores false detections
        double newAng = atan((double)(m_Eyes[i].y - m_Eyes[i+1].y) / (double)(m_Eyes[i].x - m_Eyes[i+1].x));
        if(m_SteadyControl<5 || abs(newAng*180/M_PI-m_Angles[i/2]*180/M_PI)<30){ //ignores false detection and, in this case, keeps the value of angle
            m_Angles[i/2] = newAng;
            m_SteadyControl = 7;
        }
        else
            m_SteadyControl--;

        if(m_SteadyControl<0){
            m_Angles.clear();
            for(int i=m_Angles.size();i<m_Eyes.size()/2;++i)
                m_Angles.push_back(0);
        }

        //Rotate the image to a image where the face is vertically oriented
        cv::Mat grayframe = m_GrayFrame.clone();

        int bound_w = int((m_GrayFrame.size().height * fabs(sin(m_Angles[i/2]))) + (m_GrayFrame.size().width * fabs(cos(m_Angles[i/2]))));
        int bound_h = int((m_GrayFrame.size().height * fabs(cos(m_Angles[i/2]))) + (m_GrayFrame.size().width * fabs(sin(m_Angles[i/2]))));

        cv::Mat rotate = cv::getRotationMatrix2D(cv::Point2f(m_GrayFrame.cols/2,m_GrayFrame.rows/2), m_Angles[i/2]*180/M_PI, 1.0);
        rotate.at<double>(0,2) += bound_w / 2 - m_GrayFrame.cols/2;
        rotate.at<double>(1,2) += bound_h / 2 - m_GrayFrame.rows/2;

        warpAffine(m_GrayFrame, grayframe, rotate, Size2i(bound_w, bound_h));

        //Detect the faces on the rotated image
        faces.clear();
        m_FacesCascade.detectMultiScale( grayframe, faces, 1.3, 5, 0|cv::CASCADE_SCALE_IMAGE , Size(30, 30));

        //remap the face detections to the original non-rotated frame
        for(int j = 0;j<faces.size();++j){
            quadrangle quad;
            quad.p[0] = cv::Point(faces[j].x,faces[j].y);
            quad.p[1] = cv::Point(faces[j].x+faces[j].width,faces[j].y);
            quad.p[2] = cv::Point(faces[j].x+faces[j].width,faces[j].y+faces[j].height);
            quad.p[3] = cv::Point(faces[j].x,faces[j].y+faces[j].height);

            for(int k=0;k<4;++k){
                double ang = atan2((double)(grayframe.rows/2 - quad.p[k].y),(double)(quad.p[k].x - grayframe.cols/2));
                float dist = sqrt(pow(quad.p[k].x-grayframe.cols/2,2)+pow(quad.p[k].y-grayframe.rows/2,2));

                quad.p[k].x = m_FacesFrame.cols/2 + dist*cos(m_Angles[i/2]-ang);
                quad.p[k].y = m_FacesFrame.rows/2 + dist*sin(m_Angles[i/2]-ang);
            }

            //Remove repeated detections
            cv::Point c2, c1 = cv::Point((quad.p[0].x + quad.p[1].x + quad.p[2].x + quad.p[3].x)/4,(quad.p[0].y + quad.p[1].y + quad.p[2].y + quad.p[3].y)/4);
            for(int i=0;i<m_Faces.size();++i){
                c2 = cv::Point((m_Faces[i].p[0].x + m_Faces[i].p[1].x + m_Faces[i].p[2].x + m_Faces[i].p[3].x)/4,(m_Faces[i].p[0].y + m_Faces[i].p[1].y + m_Faces[i].p[2].y + m_Faces[i].p[3].y)/4);
                float dist = sqrt((c2.x-c1.x,2)+pow(c2.y-c1.y,2));
                if(dist<10){
                    m_Faces[i] = quad;
                    return;
                }
            }
            m_Faces.push_back(quad); //if not repeated
        }
    }
}

void Vision::combineEyes(){ //define pair of eyes and left and right eye of each pair
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
