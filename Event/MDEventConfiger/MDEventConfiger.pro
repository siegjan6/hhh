#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T10:49:10
#
#-------------------------------------------------

QT       -= gui

TARGET = MDEventConfiger
TEMPLATE = lib

DEFINES += MDEVENTCONFIGER_LIBRARY
#-------------------------------------------------
include(../../pri/MDEventHelper.pri)
#-------------------------------------------------
include(../../pri/MDEventConfiger.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDEventConfiger.cpp \
    MDEventConfigerImp.cpp

HEADERS +=\
    MDEventConfigerImp.h \
    ../../include/MDEventConfiger/MDEventConfiger.h \
    ../../include/MDEventConfiger/MDEventConfiger_Global.h
