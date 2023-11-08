#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T20:45:51
#
#-------------------------------------------------

QT       -= gui

TARGET = MDAlarmExpert
TEMPLATE = lib


DEFINES += MDALARMEXPERT_LIBRARY
DEFINES += MDUASERVER_STATIC
DEFINES += UA_ENABLE_HISTORIZING

#-------------------------------------------------
include(../../pri/MDCommonHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDAlarmHelper.pri)
include(../../pri/MDAlarmLevelConfiger.pri)
include(../../pri/MDAlarmConditionConfiger.pri)
include(../../pri/MDAlarmConfiger.pri)
include(../../pri/MDEventHelper.pri)
include(../../pri/MDExpression.pri)
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDHistoryExpert.pri)
include(../../pri/MDCommandQueueEngine.pri)
include(../../pri/MDSwitchFactory.pri)
include(../../pri/MDDataArchivesEngine.pri)
include(../../pri/MDDataSource.pri)
include(../../pri/MDDataGroup.pri)
include(../../pri/MDUaServer.pri)
#include(../../pri/MDUaServerSrc.pri)
#include(../../pri/MDUaSingleton.pri)
# ignore MDRTDBManager replace with MDUaExpert
#include(../../pri/MDRTDBManager.pri)
include(../../pri/MDUaExpert.pri)
#-------------------------------------------------
include(../../pri/MDAlarmExpert.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

#PRECOMPILED_HEADER+=../../Common/MDUaServer/QUaServer/wrapper/quabaseobject.h
#PRECOMPILED_HEADER+=../../Common/MDUaServer/QUaServer/wrapper/quaoffnormalalarm.h

SOURCES += MDAlarmExpert.cpp \
    MDAlarmExpertImp.cpp \
    MDAlarmVariable.cpp \
    MDAlarmConditionRT.cpp \
    MDAlarmRecord.cpp \
    MDAlarmRecordMgr.cpp \
    MDAlarmRecordStatusMgr.cpp \
    MDAlarmContacter.cpp \
    MDAlarmCommandEngine.cpp \
    MDOffNormalAlarm.cpp \
    MDExclusiveLevelAlarm.cpp \
    CommonDevice/MDCommonDevice.cpp \
    CommonDevice/MDCommonDeviceAlarm.cpp \
    CommonDevice/MDCommonDeviceMgr.cpp

HEADERS += MDAlarmExpertImp.h \
    ../../include/MDAlarmExpert/MDAlarmExpert.h \
    ../../include/MDAlarmExpert/MDAlarmExpert_Global.h \
    MDAlarmVariable.h \
    MDAlarmConditionRT.h \
    MDAlarmRecord.h \
    MDAlarmRecordMgr.h \
    MDAlarmRecordStatusMgr.h \
    ../../include/MDAlarmExpert/MDAlarmContacter.h \
    MDAlarmCommandEngine.h \
    MDOffNormalAlarm.h \
    MDExclusiveLevelAlarm.h \
    CommonDevice/MDCommonDevice.h \
    CommonDevice/MDCommonDeviceAlarm.h \
    CommonDevice/MDCommonDeviceMgr.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
