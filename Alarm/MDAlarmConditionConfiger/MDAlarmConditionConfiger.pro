#-------------------------------------------------
#
# Project created by QtCreator 2015-08-19T14:02:30
#
#-------------------------------------------------

QT       -= gui

TARGET = MDAlarmConditionConfiger
TEMPLATE = lib

DEFINES += MDALARMCONDITIONCONFIGER_LIBRARY

#-------------------------------------------------
include(../../pri/MDAlarmHelper.pri)
include(../../pri/MDEventHelper.pri)
#-------------------------------------------------
include(../../pri/MDAlarmConditionConfiger.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDAlarmConditionConfiger.cpp \
    MDAlarmConditionConfigerImp.cpp

HEADERS += MDAlarmConditionConfigerImp.h \
    ../../include/MDAlarmConditionConfiger/MDAlarmConditionConfiger.h \
    ../../include/MDAlarmConditionConfiger/MDAlarmConditionConfiger_Global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
