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
    m_Display1 = &m_Vision->m_RawFrame;
    m_Display2 = &m_Vision->m_FacesFrame;
    //Vision::getInstance()->m_DisplaySize = cv::Size2i(ui->display1->width()-1,ui->display1->height()-1);

    m_DisplayTimer = new QTimer(this);
    connect(m_DisplayTimer, SIGNAL(timeout()), this, SLOT(refreshDisplay()));
    m_DisplayTimer->start(1);
}

void MainWindow::refreshDisplay()
{
    m_Vision->captureImage();
    ui->display1->setPixmap(QPixmap::fromImage(Mat2QImage(m_Display1->clone())));

    m_Vision->faceDetect();
    ui->display2->setPixmap(QPixmap::fromImage(Mat2QImage(m_Display2->clone())));
}

MainWindow::~MainWindow()
{
    delete ui;
}
