#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :   //Constructor
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    idCam = 0;  //default id of the first camera connected to the computer

    while (!vidCap.isOpened())
        vidCap.open(idCam);  //connect program to camera

    displayTimer = new QTimer(this);
    connect(displayTimer, SIGNAL(timeout()), this, SLOT(refreshDisplay()));//connect timer to the method to be called when timer time out
    displayTimer->start(1);   //period of timer display refresh, in milliseconds
}

void MainWindow::refreshDisplay() //Method called to refresh video frame
{
    vidCap >> frame;  //capture image and store it in a openCV matrix

    //convert openCV Mat to Qt image
    cv::cvtColor(frame, frame,cv::COLOR_BGR2RGB);
    QImage qFrame = QImage((uchar*) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

    ui->videoLabel->setPixmap(QPixmap::fromImage(qFrame)); //display QImage on the label
}

MainWindow::~MainWindow()
{
    delete ui;
}
