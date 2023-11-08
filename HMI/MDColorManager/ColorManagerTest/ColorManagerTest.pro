#-------------------------------------------------
#
# Project created by QtCreator 2015-07-23T09:51:45
#
#-------------------------------------------------
QMAKE_CXXFLAGS += -std=c++11
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColorManagerTest
TEMPLATE = app

include(../../../pri/MDColorManager.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


