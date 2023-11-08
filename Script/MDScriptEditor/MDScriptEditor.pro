#-------------------------------------------------
#
# Project created by QtCreator 2019-08-09T15:29:31
#
#-------------------------------------------------

CONFIG += c++11
QT       += core gui webenginewidgets webchannel
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MDScriptEditor
TEMPLATE = lib

DEFINES += MDSCRIPTEDITOR_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += USING_CEF_SHARED
DEFINES += WRAPPING_CEF_SHARED
DEFINES += CINTERFACE

win32: DEFINES += NOMINMAX

defined(COMPILER_GCC){
    QMAKE_CXXFLAGS +=  -Wno-unused-parameter
    QMAKE_CXXFLAGS +=  -Wno-unknown-pragmas
}

include(../../pri/MDScriptEditor.pri)
ProjectName = $${LibraryName}
include(../../pri/MDBaseForm.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/common.pri)

#win32:include(cef_win32.pri)
#else:unix:include(cef_linux32.pri)

SOURCES +=  \
#    AppProxy.cpp \
#    BrowserManager.cpp \
#    ClientApp.cpp \
#    ClientHandler.cpp \
#    ClientProxy.cpp \
#    ClientV8Handler.cpp \
    WebContainer.cpp \
    MDScriptEditor.cpp \
    EditorDialog.cpp \
    EditorForm.cpp \
#    jsbridge.cpp \
    webobjectinstance.cpp
#    cmindwebbrowser.cpp

HEADERS +=         \
#        mindcef_global.h \
#    AppProxy.h \
#    BrowserManager.h \
#    ClientApp.h \
#    ClientProxy.h \
#    ClientV8Handler.h \
    WebContainer.h \
#    ClientHandler.h \
    EditorDialog.h \
    EditorForm.h \
#    jsbridge.h \
    webobjectinstance.h
#    cmindwebbrowser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#win32 {
#INCLUDEPATH += $$PWD/win32/include
#DEPENDPATH += $$PWD/win32/include
#}

#unix {
#INCLUDEPATH += $$PWD/linux32/include
#DEPENDPATH += $$PWD/linux32/include
#}
