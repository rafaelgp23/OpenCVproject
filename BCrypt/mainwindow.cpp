#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("BCrypt");
    m_PasswordManager = Password::getInstance();

    m_Tries = 3;
    bool ok;
    while(1){
        QString password = QInputDialog::getText(this, tr("Autenticacao"),
                              tr(QString(("Tentativas restantes: " + QString::number(m_Tries) + " \nSenha: ")).toStdString().c_str()), QLineEdit::Normal,
                              NULL, &ok);
        if(!ok) this->~MainWindow();
        if(m_PasswordManager->validate(password.toStdString())){
            QMessageBox::information(0,"Autenticacao","Acesso garantido.");
            m_Tries = 3;
            break;
        }
        m_Tries--;
        if(m_Tries<=0){
            QMessageBox::information(0,"Autenticacao","Tentativas esgotadas.\nEncerrando aplicacao.");
            this->~MainWindow();
        }
   }

}

void MainWindow::on_pushButton_AbrirTxt_clicked()
{
    QString desktopDir = QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
    QString fileSelect = QFileDialog::getOpenFileName(this, tr("Selecionar Arquivo"),  desktopDir, tr("Text files (*.txt)"));
    if(!fileSelect.isEmpty()){
        ui->lineEdit_AbrirTxt->setText(fileSelect);
        on_pushButton_Carregar_clicked();
    }
}

void MainWindow::on_pushButton_Carregar_clicked()
{
    ui->textBrowser->clear();

    QFile file(ui->lineEdit_AbrirTxt->text());
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,"Erro","Arquivo nao encontrado!");
        return;
    }

    QTextStream in(&file);

    while(!in.atEnd())
        ui->textBrowser->append(in.readLine());

    file.close();
}

void MainWindow::on_pushButton_Encriptar_clicked()
{
    QFile file(ui->lineEdit_AbrirTxt->text());
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,"Erro","Arquivo nao encontrado!");
        return;
    }

    if(QFileInfo(file).suffix() != "txt"){
        QMessageBox::information(0,"Erro","Formato de arquivo invalido!");
        return;
    }

    QString backupDir = ui->lineEdit_AbrirDir->text();
    QDir dir(backupDir);
    if (!dir.exists() || backupDir.isEmpty()){
        QMessageBox::information(0,"BCrypt","Pasta de backup nao encontrada!");
        return;
    }

    QString chave = getChave();
    if(chave.isEmpty()) return;

    system(QString(QString("") + "(echo " + chave + " && sleep 0 && echo " + chave + ") | bcrypt " + file.fileName()).toStdString().c_str());
    chave.clear();
    file.setFileName(file.fileName() +".bfe");

    dir.mkpath(backupDir);
    file.rename(backupDir + "/" + QFileInfo(file.fileName()).fileName());
    ui->textBrowser->clear();
    QMessageBox::information(0,"BCrypt","Encriptacao realizada com sucesso!");
}

QString MainWindow::getChave()
{
    bool ok;
    while(1){
        QString chave = QInputDialog::getText(this, tr("Autenticacao"),
                              tr(QString(("Tentativas restantes: " + QString::number(m_Tries) + " \nSenha: ")).toStdString().c_str()), QLineEdit::Normal,
                              NULL, &ok);
        if(!ok) return NULL;
        if(m_PasswordManager->validate(chave.toStdString())){
            m_Tries = 3;
            return chave;
        }
        m_Tries--;
        if(m_Tries<=0){
            QMessageBox::information(0,"Autenticacao","Tentativas esgotadas.\nEncerrando aplicacao.");
            this->~MainWindow();
        }
   }
}

void MainWindow::on_pushButton_Desencript_clicked()
{
    QDir dir(ui->lineEdit_AbrirDir->text());
    if (!dir.exists() || ui->lineEdit_AbrirDir->text().isEmpty()){
        QMessageBox::information(0,"BCrypt","Pasta de backup nao encontrada!");
        return;
    }

    QString chave = getChave();
    if(chave.isEmpty()) return;

    system(QString(QString("") + "echo " + chave + " | bcrypt " + dir.path() + "/*.txt.bfe").toStdString().c_str());
    chave.clear();
    QMessageBox::information(0,"BCrypt","Todos os arquivos na pasta de backup foram desencriptados!");
}

MainWindow::~MainWindow()
{
    delete ui;
    qApp->quit();
    exit(EXIT_SUCCESS);
}

void MainWindow::on_pushButton_AbrirDir_clicked()
{
    QString dirSelect = QFileDialog::getExistingDirectory(this, tr("Selecionar Diretorio"),
                                                    ui->lineEdit_AbrirDir->text(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!dirSelect.isEmpty())
        ui->lineEdit_AbrirDir->setText(dirSelect);
}

void MainWindow::on_lineEdit_AbrirDir_returnPressed()
{
    QDir dir(ui->lineEdit_AbrirDir->text());
    if (!dir.exists() || ui->lineEdit_AbrirDir->text().isEmpty()){
        QMessageBox::information(0,"BCrypt","Diretorio invalido!");
    }
    else
        QMessageBox::information(0,"BCrypt","Diretorio valido!");
}

void MainWindow::on_lineEdit_AbrirTxt_returnPressed()
{
    on_pushButton_Carregar_clicked();
}
