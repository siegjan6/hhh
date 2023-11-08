#-------------------------------------------------
#
# Project created by QtCreator 2015-05-17T09:58:08
#
#-------------------------------------------------

QT       -= gui

TARGET = MDScriptExpert
TEMPLATE = lib

DEFINES += MDSCRIPTEXPERT_LIBRARY
#-------------------------------------------------
include(../../pri/MDScriptEngine.pri)
include(../../pri/MDSwitchFactory.pri)
#-------------------------------------------------
include(../../pri/MDScriptExpert.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDScriptExpert.cpp \
    MDScriptExpertImp.cpp \
    MDScriptContacter.cpp

HEADERS +=\
    MDScriptExpertImp.h \
    ../../include/MDScriptExpert/MDScriptContacter.h \
    ../../include/MDScriptExpert/MDScriptExpert.h \
    ../../include/MDScriptExpert/MDScriptExpert_Global.h
