#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T13:37:37
#
#-------------------------------------------------

QT       -= gui

TARGET = MDProjectConfiger
TEMPLATE = lib

DEFINES += MDPROJECTCONFIGER_LIBRARY
#-------------------------------------------------
include(../../pri/MDEventHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDDataArchives.pri)
# ignore MDRTDBManager replace with MDUaExpert
#include(../../pri/MDRTDBManager.pri)
include(../../pri/MDUaExpert.pri)
include(../../pri/MDEventConfiger.pri)
include(../../pri/MDActionConfiger.pri)
include(../../pri/MDAccountConfiger.pri)
include(../../pri/MDPowerGroupConfiger.pri)
include(../../pri/MDAccountStrategyConfiger.pri)
include(../../pri/MDSecurityPolicyConfiger.pri)
include(../../pri/MDAlarmLevelConfiger.pri)
include(../../pri/MDAlarmConditionConfiger.pri)
include(../../pri/MDAlarmConfiger.pri)
include(../../pri/MDProjectHelper.pri)
include(../../pri/MDRecipeConfiger.pri)
#-------------------------------------------------
include(../../pri/MDProjectConfiger.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDProjectConfiger.cpp \
    MDProjectConfigerImp.cpp

HEADERS +=\
    MDProjectConfigerImp.h \
    ../../include/MDProjectConfiger/MDProjectConfiger.h \
    ../../include/MDProjectConfiger/MDProjectConfiger_Global.h

