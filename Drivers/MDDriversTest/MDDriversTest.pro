#-------------------------------------------------
#
# Project created by QtCreator 2015-06-09T14:16:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MDDriversTest
TEMPLATE = app


SOURCES += main.cpp\
        cdialogtest.cpp

HEADERS  += cdialogtest.h

FORMS    += cdialogtest.ui

LibraryName = Mind.RTDB.MDDriversTest
ProjectName = $${LibraryName}

include(../../pri/common.pri)
include(../../pri/MDRTDBManager.pri)

INCLUDEPATH +=../../include/MDRTDBInterfaces/
INCLUDEPATH +=../../include/MDDriverInterfaces/
INCLUDEPATH +=../../include/interface/Common/
