#-------------------------------------------------
#
# Project created by QtCreator 2015-08-19T13:47:39
#
#-------------------------------------------------

QT       -= gui

TARGET = MDAlarmLevelConfiger
TEMPLATE = lib

DEFINES += MDALARMLEVELCONFIGER_LIBRARY
#-------------------------------------------------
include(../../pri/MDAlarmHelper.pri)
#-------------------------------------------------
include(../../pri/MDAlarmLevelConfiger.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDAlarmLevelConfiger.cpp \
    MDAlarmLevelConfigerImp.cpp

HEADERS += \
    ../../include/MDAlarmLevelConfiger/MDAlarmLevelConfiger_Global.h \
    ../../include/MDAlarmLevelConfiger/MDAlarmLevelConfiger.h \
    MDAlarmLevelConfigerImp.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
