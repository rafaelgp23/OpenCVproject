#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <opencv2/opencv.hpp>
#include "vision.h"
#include "utils.h"

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
        void setDisplayRatio();
        void setupCam();

        void on_pushButton_Exit_clicked();
        void on_pushButton_Settings_clicked();        

private:
    //void mousePressEvent( QMouseEvent* ev);
    void keyPressEvent(QKeyEvent * ev);


    Ui::MainWindow *ui;    
    Vision *m_Vision;
    cv::Mat *m_Display1;
    cv::Mat *m_Display2;
    QTimer* m_DisplayTimer;

    void setVideoRatio(int width, int height);
};

#endif // MAINWINDOW_H
