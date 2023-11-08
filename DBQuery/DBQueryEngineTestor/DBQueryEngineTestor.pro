#-------------------------------------------------
#
# Project created by QtCreator 2016-11-01T17:06:17
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = DBQueryEngineTestor
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


include(../../pri/MDDBQueryEngine.pri)
ProjectName =$${TARGET}
include(../../pri/common.pri)


SOURCES += main.cpp \
    Test.cpp
