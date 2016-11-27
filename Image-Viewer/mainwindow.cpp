#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), peer(NULL, 1234) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_Login_clicked() {

    //Getting Values
    std::string Username = ui->Input_Username->text().toStdString();
    std::string Password = ui->Input_Password->text().toStdString();
    std::string IP_Address = ui->Input_IP->text().toStdString();
    std::string Port = ui->Input_Port->text().toStdString();
    long int _token;

    //peer.registry_addr = IP_Address;
    //peer.registry_port = (uint16_t) stoi(Port);

    // TODO: REMOVE THIS
    peer.registry_addr = "10.40.57.28";
    peer.registry_port = 1234;

    int n = peer.retrieve_token(Username, Password, _token);

    if (n == SUCCESS) {
        token = _token;
        ui->Login_Color->setStyleSheet("background-color:green");
        ui->Token_Value->setText(QString::number(token));
    } else
        ui->Login_Color->setStyleSheet("background-color:red");
}

//Clicking button to view list of images.
void MainWindow::on_Click_View_List_clicked() {

    std::vector<std::string> image_container;
    int n = peer.view_imagelist(image_container, token);
    std::cout << n;
    for (auto image_name : image_container)
        ui->Image_List->addItem(QString::fromStdString(image_name));
}

//Clicking button to add image after entering the image name.
void MainWindow::on_AddImage_clicked() {
    std::string Image_Name = ui->Input_Add_Name->text().toStdString();

    int n = peer.add_entry(Image_Name, token);

    if (n == SUCCESS)
        QMessageBox::information(this, tr("Plumber GUI"), tr("Image added successfully."));

}

//Clicking button to delete image after entering the image name.
void MainWindow::on_DeleteImage_clicked() {
    std::string Image_Name = ui->Input_Delete_Name->text().toStdString();

    int n = peer.remove_entry(Image_Name, token);

    if (n == SUCCESS)
        QMessageBox::information(this, tr("Plumber GUI"), tr("Image removed successfully."));

}

//Clicking button to download image after entering the image name.
void MainWindow::on_DownloadImage_clicked() {
    std::string Image_Name = ui->Input_Download_Name->text().toStdString();

    //Use this whenever you feel like it.
    //QMessageBox::information(this, tr("Plumber GUI"), tr("Image removed successfully."));

}
