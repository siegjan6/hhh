#-------------------------------------------------
#
# Project created by QtCreator 2016-09-06T09:27:11
#
#-------------------------------------------------

#QT       -= gui
QT       += widgets script xml sql

CONFIG += c++11

include(../../pri/MDDataArchives.pri)
include(../../pri/MDDataArchivesEngine.pri)
INCLUDEPATH +=../../include/Testor

TARGET = DataArchiveTestorPlugin
TEMPLATE = lib

DEFINES += DATAARCHIVETESTORPLUGIN_LIBRARY

SOURCES += \
    DataArchiveTestorPlugin.cpp \
    JSDataArchivesTestor.cpp \
    TSQLExecutor.cpp

HEADERS +=\
        dataarchivetestorplugin_global.h \
    DataArchiveTestorPlugin.h \
    JSDataArchivesTestor.h \
    JSDBParameters.h \
    TSQLExecutor.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
