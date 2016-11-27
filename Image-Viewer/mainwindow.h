#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../ClionProjects/Distributed-Systems/Peer/Peer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Peer peer;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Login_clicked();
    
    void on_Click_View_List_clicked();

    void on_AddImage_clicked();

    void on_DeleteImage_clicked();

    void on_DownloadImage_clicked();

    void on_Insert_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
