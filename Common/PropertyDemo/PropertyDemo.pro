#-------------------------------------------------
#
# Project created by QtCreator 2014-11-04T14:05:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += c++11
#设置工程名称,注意只需要修改变量值，不要修改变量名称
ProjectName = PropertyDemo
include(../../pri/common.pri)

include(../../pri/MDPropertyBrowser.pri)
#include(../../pri/MDObjBatchOperation.pri)

INCLUDEPATH += object \
                property \


SOURCES += main.cpp\
        mainwindow.cpp \
    object/ctest.cpp \
    object/cdialog.cpp \
    object/csubtest.cpp \
    object/cdialog_1.cpp

HEADERS  += mainwindow.h \
    object/ctest.h \
    object/cdialog.h \
    object/csubtest.h \
    object/cdialog_1.h

FORMS    += mainwindow.ui \
    object/cdialog.ui \
    object/cdialog_1.ui

RESOURCES += \
    imageIcon.qrc
