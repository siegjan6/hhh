#-------------------------------------------------
#
# Project created by QtCreator 2016-02-22T16:48:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MDCommandQueueEngineTester
TEMPLATE = app

include(../../../pri/MDCommandQueueEngine.pri)
include(../../../pri/MDCommandQueueEngineUI.pri)
LibraryName = Mind.MDCommandQueueEngineTester
ProjectName = $${LibraryName}
include(../../../pri/common3.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
