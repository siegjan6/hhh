#-------------------------------------------------
#
# Project created by QtCreator 2016-04-26T19:58:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MDMObjFactoryTester
TEMPLATE = app

include(../../../pri/MDSwitchFactory.pri)
include(../../../pri/MDSwitchFactoryUI.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
