#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T20:45:27
#
#-------------------------------------------------

QT       -= gui

TARGET = MDAlarmConfiger
TEMPLATE = lib

DEFINES += MDALARMCONFIGER_LIBRARY
#-------------------------------------------------
include(../../pri/MDAlarmHelper.pri)
include(../../pri/MDAlarmLevelConfiger.pri)
include(../../pri/MDAlarmConditionConfiger.pri)
#-------------------------------------------------
include(../../pri/MDAlarmConfiger.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDAlarmConfiger.cpp \
    MDAlarmConfigerImp.cpp

HEADERS += MDAlarmConfigerImp.h \
    ../../include/MDAlarmConfiger/MDAlarmConfiger.h \
    ../../include/MDAlarmConfiger/MDAlarmConfiger_Global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
