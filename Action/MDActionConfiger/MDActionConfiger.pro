#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T12:57:59
#
#-------------------------------------------------

QT       -= gui

TARGET = MDActionConfiger
TEMPLATE = lib

DEFINES += MDACTIONCONFIGER_LIBRARY

#-------------------------------------------------
include(../../pri/MDActionIDHelper.pri)
include(../../pri/MDActionHelper.pri)
#-------------------------------------------------
include(../../pri/MDActionConfiger.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDActionConfiger.cpp \
    MDActionConfigerImp.cpp

HEADERS += MDActionConfigerImp.h \
    ../../include/MDActionConfiger/MDActionConfiger.h \
    ../../include/MDActionConfiger/MDActionConfiger_Global.h

