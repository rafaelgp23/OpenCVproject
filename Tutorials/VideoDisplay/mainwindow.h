#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void refreshDisplay();

private:
    Ui::MainWindow *ui;
    QTimer* displayTimer;
    cv::VideoCapture vidCap;
    int idCam;
    cv::Mat frame;
};

#endif // MAINWINDOW_H
