#-------------------------------------------------
#
# Project created by QtCreator 2016-11-25T15:58:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Image-Viewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../ClionProjects/Distributed-Systems/common/Base64/base64.cpp \
    ../ClionProjects/Distributed-Systems/common/CM/CM.cpp \
    ../ClionProjects/Distributed-Systems/common/Message/Message.cpp \
    ../ClionProjects/Distributed-Systems/common/UDP/UDPSocket.cpp \
    ../ClionProjects/Distributed-Systems/Peer/Peer.cpp

HEADERS  += mainwindow.h \
    ../ClionProjects/Distributed-Systems/common/Base64/base64.h \
    ../ClionProjects/Distributed-Systems/common/CM/CM.h \
    ../ClionProjects/Distributed-Systems/common/Message/Message.h \
    ../ClionProjects/Distributed-Systems/common/UDP/UDPSocket.h \
    ../ClionProjects/Distributed-Systems/Peer/Peer.h

FORMS    += mainwindow.ui
