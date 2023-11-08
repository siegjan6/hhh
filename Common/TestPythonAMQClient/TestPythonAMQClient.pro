#-------------------------------------------------
#
# Project created by QtCreator 2016-01-19T16:44:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestPythonAMQClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h
FORMS    += mainwindow.ui

include(../../pri/NSPythonAMQClient.pri)

#设置生成文件名和目录
CONFIG(debug, debug|release){
TARGET = TestPythonAMQClientd
DESTDIR = ../../bin/debug

}else{
TARGET = TestPythonAMQClient
DESTDIR = ../../../bin/release
}
INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../bin/debug

