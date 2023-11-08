#-------------------------------------------------
#
# Project created by QtCreator 2015-02-28T14:38:44
#
#-------------------------------------------------

QT       += sql core

QT       -= gui

TARGET = MDDBConnect

TEMPLATE = lib

DEFINES += MDDBCONNECT_LIBRARY

include(../../pri/MDDBConnect.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDSwitchFactory.pri)

SOURCES += \
    src/MDDBConnect.cpp \
    src/DBConnection.cpp \
    src/utils.cpp

HEADERS +=\
#    src/MDDBConnect.h \
#    ../../Include/MDDBConnect/MDDBConnect.h \
    ../../include/MDDBConnect/MDDBConnect.h \
    src/DBConnection.h \
    src/utils.h
#    src/DBConnection.h
