#-------------------------------------------------
#
# Project created by QtCreator 2016-05-13T16:02:08
#
#-------------------------------------------------

QT       += core gui
QT += widgets

TARGET = MDQJson2SortJson
TEMPLATE = lib

DEFINES += MDQJSON2SORTJSON_LIBRARY


include(../../pri/MDQJson2SortJson.pri)
ProjectName = $${LibraryName}

include(../../pri/common.pri)


SOURCES += SortJsonObject.cpp

HEADERS  += \
    ../../include/MDQJson2SortJson/SortJsonObject.h
