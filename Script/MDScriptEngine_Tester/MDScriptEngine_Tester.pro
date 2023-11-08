#-------------------------------------------------
#
# Project created by QtCreator 2015-04-30T20:25:23
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = MDScriptEngine_Tester
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../../pri/MDScriptEngine.pri)
include(../../pri/SampleObjs.pri)
include(../../pri/SampleObjsWrapper.pri)

SOURCES += main.cpp
