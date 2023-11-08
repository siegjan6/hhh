#-------------------------------------------------
#
# Project created by QtCreator 2015-07-27T16:55:36
#
#-------------------------------------------------

QT += widgets core

TARGET = MDDataSelectUI
TEMPLATE = lib

DEFINES += MDDATASELECTUI_LIBRARY

include(../../pri/MDDataSelectUI.pri)
ProjectName =$${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDDataArchives.pri)

SOURCES += \
    DataSelectUI.cpp \
    MDDataSelectUI.cpp

HEADERS +=\
        ../../include/MDDataSelectUI/mddataselectui_global.h \
    DataSelectUI.h \
    ../../include/MDDataSelectUI/MDDataSelectUI.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
