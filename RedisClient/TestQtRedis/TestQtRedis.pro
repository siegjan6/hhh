#-------------------------------------------------
#
# Project created by QtCreator 2015-11-23T19:04:27
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestQtRedis
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui



INCLUDEPATH += $$PWD/../../include/MDRedisClient/include
DEPENDPATH += $$PWD/../../include/MDRedisClient/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/release/ -lMind.MDRedisClient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/debug/ -lMind.MDRedisClientd
else:unix: LIBS += -L$$PWD/../../bin/ -lMind

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
