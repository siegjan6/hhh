#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T16:17:57
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

include(../../pri/MDSysLogExpert.pri)
include(../../pri/MDDataSource.pri)

TARGET = DBConnectionPoolTestor
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app

DESTDIR = ../../bin/debug

INCLUDEPATH += ../../include/

SOURCES += main.cpp \
    MDDBConnCache.cpp \
    DBConnectionTemp.cpp

HEADERS += \
    MDDBConnCache.h \
    DBConnectionTemp.h
