#-------------------------------------------------
#
# Project created by QtCreator 2016-09-05T13:21:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        cao.pb.cc

HEADERS  += mainwindow.h\
        cao.pb.h

FORMS    += mainwindow.ui

ProjectName = TestProtoBuf

INCLUDEPATH += ../../../include/
include(MDProtoBufLocal.pri)
include(../../../pri/common3.pri)
