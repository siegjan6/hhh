#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T20:44:48
#
#-------------------------------------------------

QT       -= gui

TARGET = MDAlarmHelper
TEMPLATE = lib

DEFINES += MDALARMHELPER_LIBRARY
#-------------------------------------------------
include(../../pri/MDEventHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
#-------------------------------------------------
include(../../pri/MDAlarmHelper.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += \
    MDAlarmLevelImp.cpp \
    MDAlarmLevel.cpp \
    MDAlarmConditionImp.cpp \
    MDAlarmCondition.cpp \
    MDAlarmPara.cpp \
    MDAlarmParaImp.cpp

HEADERS +=\
    MDAlarmLevelImp.h \
    ../../include/MDAlarmHelper/MDAlarmLevel.h \
    ../../include/MDAlarmHelper/MDAlarmHelper_Global.h \
    MDAlarmConditionImp.h \
    ../../include/MDAlarmHelper/MDAlarmCondition.h \
    MDAlarmParaImp.h \
    ../../include/MDAlarmHelper/MDAlarmConstant.h \
    ../../include/MDAlarmHelper/MDAlarmPara.h

