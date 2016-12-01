#include <iostream>
#include <fstream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "QMessageBox"
#include "QThread"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), peer(NULL, 1234) {
    ui->setupUi(this);

    QThread *thread = new QThread;
    Server *server = new Server(&peer);
    server->moveToThread(thread);
    connect(server, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(thread, SIGNAL(started()), server, SLOT(runServer()));
    connect(server, SIGNAL(finished()), thread, SLOT(quit()));
    connect(server, SIGNAL(finished()), server, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

    // Needed for images to properly scale inside Image Display label
    ui->Image_Display->setScaledContents(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

//////////////// 1  ////////////////
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
    peer.registry_addr = "10.40.54.1";
    peer.registry_port = 1234;

    int n = peer.retrieve_token(Username, Password, _token);
    token = 0;

    ui->Image_Viewable_List->clear();
    ui->Image_Downloaded_List->clear();

    if (n == SUCCESS) {
        token = _token;
        QMessageBox::information(this, tr("Plumber GUI"), tr("Login Success!."));
        ui->Login_Color->setStyleSheet("background-color:green");
        ui->Token_Value->setText(QString::number(token));
    } else if (n == CONNECTION_ERROR) {
        QMessageBox::information(this, tr("Plumber GUI"), tr("Connection Error occured"));
        ui->Login_Color->setStyleSheet("background-color:red");
        ui->Token_Value->setText("Invalid Token");
    } else {
        QMessageBox::information(this, tr("Plumber GUI"), tr("Login Failed"));
        ui->Login_Color->setStyleSheet("background-color:red");
        ui->Token_Value->setText("Invalid Token");
    }
}

//////////////// 2  ////////////////
//Clicking button to view image viewable list.
void MainWindow::on_Click_View_List_clicked() {
    std::vector<std::string> image_container;

    // Clear the viewable images list
    ui->Image_Viewable_List->clear();

    int n = peer.view_imagelist(image_container, token);

    if (image_container.empty())
        ui->Image_Viewable_List->addItem("No images available");
    else
        for (auto image_name : image_container) {
            QString line = QString::fromStdString(image_name);
            ui->Image_Viewable_List->addItem(line);
        }
}

//////////////// 3  ////////////////
//Clicking button to view image downloaded list.
void MainWindow::on_Click_Downloaded_List_clicked() {
    std::vector<std::string> image_container;

    ui->Image_Downloaded_List->clear();

    int n = peer.view_imagelist(image_container, token);
    std::cout << n;
    if(image_container.empty())
        ui->Image_Downloaded_List->addItem("No images available.");
    else
        for (auto image_name : image_container)
            ui->Image_Downloaded_List->addItem(QString::fromStdString(image_name));
}

//////////////// 4  ////////////////
//Clicking button to add image after entering the image name.
void MainWindow::on_AddImage_clicked() {
    std::string Image_Name = ui->Input_Add_Name->text().toStdString();

    int n = peer.add_entry(Image_Name, token);

    if (n == SUCCESS)
        QMessageBox::information(this, tr("Plumber GUI"), tr("Image added successfully."));
    else
        QMessageBox::information(this, tr("Plumber GUI"), tr("Image failed to add."));

}

//////////////// 5 ////////////////
//Clicking button to delete image after entering the image name.
void MainWindow::on_DeleteImage_clicked() {
    std::string Image_Name = ui->Input_Delete_Name->text().toStdString();

    int n = peer.remove_entry(Image_Name, token);

    if (n == SUCCESS)
        QMessageBox::information(this, tr("Plumber GUI"), tr("Image removed successfully."));
    else
        QMessageBox::information(this, tr("Plumber GUI"), tr("Image failed to remove."));

}

//////////////// 6  ////////////////
//Clicking button to download image after entering the image name.
void MainWindow::on_DownloadImage_clicked() {

    std::string Image_Name = ui->Input_Download_Name->text().toStdString();
    std::vector<std::string> image_container;

    int n = peer.download_image(Image_Name, token, image_container);

    if (n == SUCCESS) {
        std::ofstream downloaded_image (Image_Name, std::ios::binary);
        downloaded_image << image_container[0];
        QString qstring_image_name = QString::fromStdString(Image_Name);
        ui->Image_Downloaded_List->addItem(qstring_image_name);
        QMessageBox::information(this, tr("Plumber GUI"), tr("Image downloaded successfully."));
    }
    else
        QMessageBox::information(this, tr("Plumber GUI"), tr("Image failed to download."));

    // Use this whenever you feel like it.
    // Ok dude we should keep this as a super secret ultimate weapon against the plumber
    //QMessageBox::information(this, tr("Plumber GUI"), tr("Image removed successfully."));

}

//////////////// 7  ////////////////
//Clicking button to display image after entering the image name.
void MainWindow::on_Display_Button_clicked() {

    std::string Image_Name = ui->ImageName_to_Display->text().toStdString();

    // TODO: Implement stegnography decoding here, check num of views left, if num of views is 0 display encoded image
    QImage image_to_display(QString::fromStdString(Image_Name));
    ui->Image_Display->setPixmap(QPixmap::fromImage(image_to_display));
}

void MainWindow::on_Grant_Access_clicked()
{
    std::string Image_Name = ui->Input_Imgname_Allow->text().toStdString();
    std::string Username = ui->Input_Username_Allow->text().toStdString();

    int n = peer.set_image_viewable_by(Image_Name, token, Username);

    if(n == SUCCESS)
        QMessageBox::information(this, tr("Plumber GUI"), tr("Successfully added Viewer."));
    else
        QMessageBox::information(this, tr("Plumber GUI"), tr("Failed to add Viewer."));

}
