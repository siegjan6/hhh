#-------------------------------------------------
#
# Project created by QtCreator 2015-04-30T18:18:00
#
#-------------------------------------------------

QT       -= gui
#QT       += widgets
QT       += script

TARGET = MDScriptEngine
TEMPLATE = lib

#支持c++11,可以支持类似Enum::enumValue之类的语法
CONFIG += c++11

DEFINES += MDSCRIPTENGINE_LIBRARY

include(../../pri/MDScriptEngine.pri)
#include(../../pri/MDSwitchFactory.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDScriptEngine.cpp \
    MDScriptEngineImp.cpp \
    MDObjectWrapper.cpp \
    MDObjectWrapperImp.cpp \
    MDScriptEnginePool.cpp

HEADERS +=  MDScriptEngineImp.h \
    MDObjectWrapperImp.h \
    ../../include/MDScriptEngine/MDScriptEngine.h \
    ../../include/MDScriptEngine/MDObjectWrapper.h \
    ../../include/MDScriptEngine/MDScriptEngine_Global.h \
    MDScriptEnginePool.h

OTHER_FILES +=
