#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include "../Peer/Peer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    Peer peer;
    long int token;

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:

    void on_Login_clicked();

    void on_Click_View_List_clicked();

    void on_AddImage_clicked();

    void on_DeleteImage_clicked();

    void on_DownloadImage_clicked();

    void errorString(QString err) {
        // Add here what happens when server thread crashes/reports error
    }

    void on_Click_Downloaded_List_clicked();

    void on_Display_Button_clicked();

    void on_pushButton_clicked();

    void on_Grant_Access_clicked();

private:
    Ui::MainWindow *ui;
};

class Server : public QObject {
Q_OBJECT
    Peer *peer;
public:
    Server(Peer *_peer) : peer(_peer) {

    }

    ~Server() {

    }

public slots:

    void runServer() {
        peer->runServer();
    }

signals:

    void finished();

    void error(QString err);

private:
    // add your variables here
};

#endif // MAINWINDOW_H
