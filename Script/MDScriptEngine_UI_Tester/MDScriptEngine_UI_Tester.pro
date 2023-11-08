#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T13:57:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MDScriptEngine_UI_Tester
TEMPLATE = app

#支持c++11,可以支持类似Enum::enumValue之类的语法
QMAKE_CXXFLAGS += -std=c++11

unix{
    LIBS += ../../include/V8/lib/linux/libv8.so.3.14.5
}
win32{
    CONFIG(debug, debug|release){
        LIBS += ../../include/V8/lib/win/debug/libv8_g.a
        LIBS += ../../include/V8/lib/win/debug/libv8preparser_g.a
    }
    else{
        LIBS += ../../include/V8/lib/win/release/libv8.a
        LIBS += ../../include/V8/lib/win/release/libv8preparser.a
    }
    LIBS += -lwinmm
    LIBS += -lws2_32
}

include(../../pri/MDScriptEngine.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    ScriptThread.cpp \
    MDWindow.cpp \
    MDUiWrappers.cpp \
    ../../include/MDScriptEngine/WrapperHelper.cpp \
    MDUser.cpp

HEADERS  += mainwindow.h \
    ScriptThread.h \
    MDWindow.h \
    MDUiWrappers.h \
    ../../include/MDScriptEngine/WrapperHelper.h \
    MDUser.h

FORMS    += mainwindow.ui
