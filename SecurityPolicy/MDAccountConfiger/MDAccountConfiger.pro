#-------------------------------------------------
#
# Project created by QtCreator 2015-07-30T16:06:01
#
#-------------------------------------------------

QT       -= gui

TARGET = MDAccountConfiger
TEMPLATE = lib

DEFINES += MDACCOUNTCONFIGER_LIBRARY
#-------------------------------------------------
include(../../pri/MDSecurityPolicyHelper.pri)
#-------------------------------------------------
include(../../pri/MDAccountConfiger.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDAccountConfiger.cpp \
    MDAccountConfigerImp.cpp

HEADERS +=\
    MDAccountConfigerImp.h \
    ../../include/MDAccountConfiger/MDAccountConfiger_Global.h \
    ../../include/MDAccountConfiger/MDAccountConfiger.h

