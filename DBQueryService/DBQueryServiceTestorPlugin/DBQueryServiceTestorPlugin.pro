#QT       -= gui
QT       += widgets script

#CONFIG += c++11

#include(../../pri/MDDataArchives.pri)
#include(../../pri/MDDataArchivesEngine.pri)
INCLUDEPATH +=../../include/Testor
INCLUDEPATH +=../../include/MDDataArchivesInterface

include(../../pri/MDDBQueryService.pri)

TARGET = DBQueryServiceTestorPlugin
TEMPLATE = lib

HEADERS += \
    JSQueryTestor.h \
    DBQueryServiceTestorPlugin.h

SOURCES += \
    JSQueryTestor.cpp \
    DBQueryServiceTestorPlugin.cpp
