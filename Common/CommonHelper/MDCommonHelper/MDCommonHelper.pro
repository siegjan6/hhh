#-------------------------------------------------
#
# Project created by QtCreator 2015-02-11T09:26:15
#
#-------------------------------------------------

QT       -= gui

TARGET = MDCommonHelper
TEMPLATE = lib

DEFINES += MDCOMMONHELPER_LIBRARY

include(../../../pri/MDCommonHelper.pri)
ProjectName = $${LibraryName}
include(../../../pri/common3.pri)

SOURCES += \
    MDObject.cpp \
    MDObjectData.cpp

HEADERS += \
    MDObjectData.h \
    ../../../include/MDCommonHelper/MDCommonConstant.h \
    ../../../include/MDCommonHelper/MDCommonHelper_Global.h \
    ../../../include/MDCommonHelper/MDObject.h

