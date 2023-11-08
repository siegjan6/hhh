#-------------------------------------------------
#
# Project created by QtCreator 2015-05-29T10:05:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Test
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=


ProjectName = Test

include(../../pri/common.pri)



include(../../pri/NSJavaScriptCodeEditor.pri)

INCLUDEPATH += $$PWD/../../bin/debug
DEPENDPATH += $$PWD/../../bin/debug
