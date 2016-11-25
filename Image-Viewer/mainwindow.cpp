#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
