#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
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
        void on_pushButton_Exit_clicked();

private:
    //void mousePressEvent( QMouseEvent* ev);

    Ui::MainWindow *ui;    
    Vision *m_Vision;
    cv::Mat *m_Display1;
    cv::Mat *m_Display2;
    QTimer* m_DisplayTimer;

};

#endif // MAINWINDOW_H
