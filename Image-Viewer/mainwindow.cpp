#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow), peer(NULL, 1234)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Login_clicked()
{
    std::string User = "a";
    std::string Pass = "b";
    std::string Username = ui->Input_Username->text().toStdString();
    std::string Password = ui->Input_Password->text().toStdString();

    //Check the values of Username and Password.

    //LOGGED IN.
    if(Username == User && Password == Pass)
    {
        ui->Login_Color->setStyleSheet("background-color:green");
        ui->Token_Value->setText("123");
    }
    else
    {
        ui->Login_Color->setStyleSheet("background-color:red");
    }

}

//Clicking button to view list of images.
void MainWindow::on_Click_View_List_clicked()
{
    //ui->Image_List->model()

}

//Clicking button to add image after entering the image name.
void MainWindow::on_AddImage_clicked()
{
    std::string Image_Name = ui->Input_Add_Name->text().toStdString();

}

//Clicking button to delete image after entering the image name.
void MainWindow::on_DeleteImage_clicked()
{
    std::string Image_Name = ui->Input_Delete_Name->text().toStdString();

}

//Clicking button to download image after entering the image name.
void MainWindow::on_DownloadImage_clicked()
{
    std::string Image_Name = ui->Input_Download_Name->text().toStdString();

}

//Clicking button to insert ip address and port
void MainWindow::on_Insert_clicked()
{
    std::string IP_Address = ui->Input_IP->text().toStdString();
    std::string Port = ui->Input_Port->text().toStdString();

}
