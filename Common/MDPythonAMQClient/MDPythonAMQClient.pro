#-------------------------------------------------
#
# Project created by QtCreator 2016-01-19T16:03:32
#
#-------------------------------------------------
QT       += core
QT       += gui
QT       += widgets #python控制台需要
TARGET = MDPythonRedisClient
TEMPLATE = lib

DEFINES += MDPYTHONAMQCLIENT_LIBRARY


include(../../pri/MDPythonAMQClient.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)
include($$PWD/build/python.prf)

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../include
DEPENDPATH += $$PWD/../../

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/release/ -lPythonQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/debug/ -lPythonQtd

RESOURCES += AMQPython.qrc

SOURCES += \
    MDPythonAMQClient.cpp

HEADERS +=\
    MDPythonAMQClient.h



unix {
    target.path = /usr/lib
    INSTALLS += target
}

