#-------------------------------------------------
#
# Project created by QtCreator 2015-11-24T13:30:00
#
#-------------------------------------------------

QT       += network
QT       += core
QT       -= gui

TARGET = MDRedisClient
TEMPLATE = lib

DEFINES += MDREDISCLIENT_LIBRARY

include(../../pri/MDRedisClient.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += \
    qtredis.cpp \
    reader.cpp \
    MDRedisClient.cpp

HEADERS +=\
    qtredis.h \
    reader.h \
    MDRedisClient.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
