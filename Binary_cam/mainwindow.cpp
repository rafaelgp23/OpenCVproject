#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    this->setWindowTitle("Computer Visual Project");
    m_Vision = Vision::getInstance();
    while (!(m_Vision->m_VideoCapture.isOpened()))
        m_Vision->m_VideoCapture.open(m_Vision->m_IdCamera);
    on_pushButton_Settings_clicked();
    setDisplayRatio();
    setupCam();

    //setup DisplayRatio GroupBox
    connect(ui->radioButton_defaultRatio,SIGNAL(toggled(bool)),this,SLOT(setDisplayRatio()));
    connect(ui->radioButton_16_9,SIGNAL(toggled(bool)),this,SLOT(setDisplayRatio()));
    connect(ui->radioButton_4_3,SIGNAL(toggled(bool)),this,SLOT(setDisplayRatio()));

    //setup sliders
    connect(ui->horizontalSlider_Bright,SIGNAL(valueChanged(int)),this,SLOT(setupCam()));
    connect(ui->horizontalSlider_Contr,SIGNAL(valueChanged(int)),this,SLOT(setupCam()));
    connect(ui->horizontalSlider_Satur,SIGNAL(valueChanged(int)),this,SLOT(setupCam()));

    m_Display1 = &m_Vision->m_RawFrame;
    m_Display2 = &m_Vision->m_FacesFrame;

    //this timer refreshes the display frames
    m_DisplayTimer = new QTimer(this);
    connect(m_DisplayTimer, SIGNAL(timeout()), this, SLOT(refreshDisplay()));
    m_DisplayTimer->start(1);//milisecond
}

//display the video using QLabel
void MainWindow::refreshDisplay()
{
    m_Vision->captureImage();
    ui->display1->setPixmap(QPixmap::fromImage(Mat2QImage(m_Display1->clone())));

    m_Vision->faceDetect();
    drawFace();
    log();
    ui->display2->setPixmap(QPixmap::fromImage(Mat2QImage(m_Display2->clone())));

    // log();
}

void MainWindow::keyPressEvent(QKeyEvent *ev){
    switch (ev->key()) {
    case Qt::Key_Escape:
        on_pushButton_Exit_clicked();
        break;
    default:
        return;
        break;
    }
}

void MainWindow::log(){
    //ui->textBrowser->moveCursor(QTextCursor::Start);
    ui->textBrowser->clear();

    ui->textBrowser->insertPlainText("Faces detected: " + QString::number(m_Vision->m_Faces.size()) + "\n");
    ui->textBrowser->insertPlainText("Eyes detected: " + QString::number(m_Vision->m_Eyes.size()) + "\n");
    ui->textBrowser->insertPlainText("Face angle: " + QString::number((int)(m_Vision->m_Angle*180/M_PI)) + " deg\n");

}

void MainWindow::drawFace(){
    //draw eyes
    if(ui->checkBox_drawEyes->isChecked())
        for(int i=0;i<m_Vision->m_Eyes.size();++i){
            cv::Point eyeCenter( m_Vision->m_Eyes[i].x + m_Vision->m_Eyes[i].width*0.5, m_Vision->m_Eyes[i].y + m_Vision->m_Eyes[i].height*0.5 );
            cv::ellipse( m_Vision->m_FacesFrame, eyeCenter, Size( m_Vision->m_Eyes[i].width*0.5, m_Vision->m_Eyes[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }

    //draw eyes combined
    if(ui->checkBox_drawEyesCombined->isChecked())
        for(int i=0;i<m_Vision->m_Eyes.size();i+=2){
            cv::Point leftEye( m_Vision->m_Eyes[i].x + m_Vision->m_Eyes[i].width*0.5, m_Vision->m_Eyes[i].y + m_Vision->m_Eyes[i].height*0.5 );
            cv::Point rightEye( m_Vision->m_Eyes[i+1].x + m_Vision->m_Eyes[i+1].width*0.5, m_Vision->m_Eyes[i+1].y + m_Vision->m_Eyes[i+1].height*0.5 );
            cv::line(m_Vision->m_FacesFrame,leftEye,rightEye, Scalar( 0, 0, 0 ),2);
        }

    //draw faces
    if(ui->checkBox_drawFaces->isChecked())
        for(int i=0;i<m_Vision->m_Faces.size();++i)
            for(int j=0;j<4;++j)
                cv::line(m_Vision->m_FacesFrame,m_Vision->m_Faces[0].p[j],m_Vision->m_Faces[0].p[(j+1)%4], Scalar( 0, 255, 255 ),2);
}

void MainWindow::setupCam(){
    m_Vision->m_VideoCapture.set(CV_CAP_PROP_BRIGHTNESS,float(ui->horizontalSlider_Bright->value())/100);
    m_Vision->m_VideoCapture.set(CV_CAP_PROP_CONTRAST,float(ui->horizontalSlider_Contr->value())/100);
    m_Vision->m_VideoCapture.set(CV_CAP_PROP_SATURATION,float(ui->horizontalSlider_Satur->value())/100);
}

void MainWindow::setDisplayRatio(){

    int width = ui->display1->width()-1, height = ui->display1->height()-1;
    int w, h;

    if(ui->radioButton_defaultRatio->isChecked()){
        w = m_Vision->m_VideoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
        h = m_Vision->m_VideoCapture.get(CV_CAP_PROP_FRAME_HEIGHT);
    }
    else if(ui->radioButton_16_9->isChecked()){
        w = 16;
        h = 9;
    }
    else if(ui->radioButton_4_3->isChecked()){
        w = 4;
        h = 3;
    }

    if(float(width)/w<float(height)/h)
        m_Vision->m_DisplaySize = cv::Size2i(width,width*h/w);
    else
        m_Vision->m_DisplaySize = cv::Size2i(height*w/h,height);
}

void MainWindow::on_pushButton_Settings_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->label_CurrentPage->setText("Settings");
}

void MainWindow::on_pushButton_DevMode_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->label_CurrentPage->setText("Developer Mode");
}

void MainWindow::on_pushButton_Exit_clicked()
{
    cv::destroyAllWindows();
    qApp->quit();
    exit(EXIT_SUCCESS);
}

MainWindow::~MainWindow()
{
    delete ui;
    cv::destroyAllWindows();
    qApp->quit();
}
