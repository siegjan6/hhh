#-------------------------------------------------
#
# Project created by QtCreator 2015-07-10T13:50:16
#
#-------------------------------------------------

QT       -= gui

TARGET = MDDataGroup
TEMPLATE = lib

DEFINES += MDDATAGROUP_LIBRARY

include(../../pri/MDDataGroup.pri)
ProjectName =$${LibraryName}
include(../../pri/common.pri)


SOURCES += MDDataGroup.cpp \
    MDDataGroupManager.cpp \
    MDDataItem.cpp \
    MDDataGroupFactory.cpp

HEADERS += MDDataGroup.h\
    MDDataGroupManager.h \
    MDDataItem.h \
    ../../include/MDDataGroup/MDDataGroupFactory.h \
    ../../include/MDDataGroup/mddatagroup_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
