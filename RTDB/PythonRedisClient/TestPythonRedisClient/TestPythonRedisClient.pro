#-------------------------------------------------
#
# Project created by QtCreator 2016-01-19T16:44:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestPythonRedisClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h
FORMS    += mainwindow.ui

include(../../pri/MDPythonRedisClient.pri)

#设置生成文件名和目录
CONFIG(debug, debug|release){
TARGET = TestPythonRedisClientd
DESTDIR = ../../bin/debug

}else{
TARGET = TestPythonRedisClient
DESTDIR = ../../bin/release
}

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/release/ -lNetSCADA6.PythonRedisClient
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/debug/ -lNetSCADA6.PythonRedisClientd
#else:unix:!macx: LIBS += -L$$PWD/../../bin/ -llibNetSCADA6.PythonRedisClientd

INCLUDEPATH += $$PWD/../../bin/debug
DEPENDPATH += $$PWD/../../bin/debug
