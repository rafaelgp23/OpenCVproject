#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QFileDialog"
#include "QMessageBox"
#include "QTextStream"
#include "QDesktopServices"
#include "QInputDialog"
#include "QString"

#include "password.h"

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
    void on_pushButton_Carregar_clicked();

    void on_pushButton_Encriptar_clicked();

    void on_pushButton_Desencript_clicked();

    void on_pushButton_AbrirTxt_clicked();

    void on_pushButton_AbrirDir_clicked();

    void on_lineEdit_AbrirDir_returnPressed();

    void on_lineEdit_AbrirTxt_returnPressed();

private:
    Ui::MainWindow *ui;
    Password *m_PasswordManager;

    int m_Tries;
    QString getChave();
};

#endif // MAINWINDOW_H
