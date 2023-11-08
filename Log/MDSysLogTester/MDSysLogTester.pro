#-------------------------------------------------
#
# Project created by QtCreator 2015-04-02T09:57:58
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MDSysLogTester
TEMPLATE = app

include(../../pri/MDSysLogTester.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
