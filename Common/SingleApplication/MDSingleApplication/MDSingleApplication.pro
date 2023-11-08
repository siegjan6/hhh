#-------------------------------------------------
#
# Project created by QtCreator 2015-07-09T20:57:42
#
#-------------------------------------------------

QT       -= gui
QT += widgets network

TARGET = MDSingleApplication
TEMPLATE = lib

CONFIG += c++11
DEFINES += MDSINGLEAPPLICATION_LIBRARY

include(../../../pri/MDSingleApplication.pri)
ProjectName = $${LibraryName}
include(../../../pri/common3.pri)

SOURCES += MDSingleApplication.cpp \
    MDSingleApplicationImp.cpp \
    MDSingleAppClientImp.cpp

HEADERS +=\
    MDSingleApplicationImp.h \
    ../../../include/MDSingleApplication/MDSingleApplication_Gobal.h \
    ../../../include/MDSingleApplication/MDSingleApplication.h \
    MDSingleAppClientImp.h

