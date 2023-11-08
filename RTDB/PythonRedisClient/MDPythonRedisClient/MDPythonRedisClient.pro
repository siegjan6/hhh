#-------------------------------------------------
#
# Project created by QtCreator 2016-01-19T16:03:32
#
#-------------------------------------------------
QT       += core
QT       += gui

TARGET = MDPythonRedisClient
TEMPLATE = lib

DEFINES += MDPYTHONREDISCLIENT_LIBRARY


include(../../pri/MDPythonRedisClient.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)
include($$PWD/build/python.prf)

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
DEPENDPATH += $$PWD/../

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../bin/release/ -lPythonQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../bin/debug/ -lPythonQtd

RESOURCES += RedisPython.qrc

SOURCES += \
    MDPythonRedisClient.cpp

HEADERS +=\
    MDPythonRedisClient.h



unix {
    target.path = /usr/lib
    INSTALLS += target
}


