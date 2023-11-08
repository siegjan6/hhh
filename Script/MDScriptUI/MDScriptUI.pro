#-------------------------------------------------
#
# Project created by QtCreator 2015-06-03T13:26:21
#
#-------------------------------------------------

QT       +=core gui widgets

TARGET = MDScriptUI
TEMPLATE = lib

DEFINES += MDSCRIPTUI_LIBRARY
#-------------------------------------------------
include(../../pri/MDEventHelper.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDJavaScriptCodeEditor.pri)
include(../../pri/MDScriptEditor.pri)
#-------------------------------------------------
include(../../pri/MDScriptUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDScriptUI.cpp \
    MDScriptUIImp.cpp \
    MDScriptFileNode.cpp

HEADERS +=\
    MDScriptUIImp.h \
    ../../include/MDScriptUI/MDScriptUI.h \
    ../../include/MDScriptUI/MDScriptUI_Global.h \
    MDScriptFileNode.h
