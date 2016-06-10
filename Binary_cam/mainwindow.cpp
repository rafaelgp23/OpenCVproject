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
  //  ui->display1->setPixmap(QPixmap::fromImage(Mat2QImage(m_Display1->clone())));

    m_Vision->faceDetect();
  //  ui->display2->setPixmap(QPixmap::fromImage(Mat2QImage(m_Display2->clone())));

    //    namedWindow("gray",cv::WINDOW_AUTOSIZE);
    //    cv::imshow("gray",m_Vision->m_GrayFrame);
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

MainWindow::~MainWindow()
{
    delete ui;
    cv::destroyAllWindows();
    qApp->quit();
}

void MainWindow::on_pushButton_Exit_clicked()
{
    cv::destroyAllWindows();
    qApp->quit();
    exit(EXIT_SUCCESS);
}

void MainWindow::on_pushButton_Settings_clicked()
{
    ui->label_CurrentPage->setText("Settings");
    ui->stackedWidget->setCurrentIndex(0);
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
