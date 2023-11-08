#-------------------------------------------------
#
# Project created by QtCreator 2015-08-15T14:27:31
#
#-------------------------------------------------

QT       -= gui

TARGET = MDActionIDHelper
TEMPLATE = lib

DEFINES += MDACTIONIDHELPER_LIBRARY

include(../../pri/MDActionIDHelper.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += \
    MDActionIDData.cpp \
    MDActionID.cpp

HEADERS +=\
    MDActionIDData.h \
    ../../include/MDActionIDHelper/MDActionID.h \
    ../../include/MDActionIDHelper/MDActionIDHelper_Global.h \
    MDStringResource.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
