#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T14:45:48
#
#-------------------------------------------------

QT       += gui
QT       += xml
QT       += widgets

TARGET = MDUaExpert
TEMPLATE = lib

DEFINES += MDRTDBMANAGER_LIBRARY
#DEFINES += MDVARIABLEEXPERT_LIBRARY
DEFINES += MDUAEXPERT_LIBRARY
DEFINES += MDPROJECTEXPERT_LIBRARY
DEFINES += MDALARMEXPERT_LIBRARY
DEFINES += MDUAEXPERT_STATIC
DEFINES += MDUASERVER_STATIC
DEFINES += UA_ENABLE_HISTORIZING
RELEASE_SEPARATE_DEBUG = $$(CUSTOM_RELEASE_SEPARATE_DEBUG)
win32:!win32-g++ {
equals(RELEASE_SEPARATE_DEBUG, "SEPARATE") {
    CONFIG(debug, debug|release){
        DEFINES += MDKVCLIENT_LIBRARY_MT
        !build_pass:message('MDRTDBManagerUI -->>(DEFINES, MDKVCLIENT_LIBRARY_MT)')
    }
    else{
        DEFINES += MDKVCLIENT_LIBRARY_MD
        !build_pass:message('MDRTDBManagerUI -->>(DEFINES, MDKVCLIENT_LIBRARY_MD)')
    }
}
else {
    DEFINES += MDKVCLIENT_LIBRARY_MT
}
}

#-------------------------------------------------
include(../../pri/MDUtility.pri)
include(../../pri/MDCommonHelper.pri)
include(../../pri/MDExpression.pri)
include(../../pri/MDDBConnect.pri)
include(../../pri/MDCommandQueueEngine.pri)
include(../../pri/MDSysLogExpert.pri)
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDMObjFactory.pri)
include(../../pri/MDSwitchFactory.pri)
include(../../pri/MDSwitchFactoryUI.pri)
include(../../pri/MDQJson2SortJson.pri)

include(../../pri/MDActionIDHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDLogHelper.pri)
include(../../pri/MDProjectHelper.pri)
include(../../pri/MDMultiLanguage.pri)

include(../../pri/MDExtSerialPort.pri)
include(../../pri/MDDriverCommunicationLibs.pri)
include(../../pri/MDDriverLibs.pri)
include(../../pri/MDVariableArchives.pri)
#include(../../pri/MDRTDBManager.pri)

include(../../pri/MDScriptEngine.pri)

include(../../pri/MDActionHelper.pri)
include(../../pri/MDEventHelper.pri)
include(../../pri/MDAlarmHelper.pri)
include(../../pri/MDRecipeHelper.pri)

include(../../pri/MDActionConfiger.pri)
include(../../pri/MDEventConfiger.pri)
include(../../pri/MDAccountConfiger.pri)
include(../../pri/MDAccountStrategyConfiger.pri)
include(../../pri/MDPowerGroupConfiger.pri)
include(../../pri/MDSecurityPolicyConfiger.pri)
include(../../pri/MDDataGroup.pri)
include(../../pri/MDDataSource.pri)
include(../../pri/MDDataArchives.pri)
include(../../pri/MDDataArchivesEngine.pri)
include(../../pri/MDAlarmLevelConfiger.pri)
include(../../pri/MDAlarmConditionConfiger.pri)
include(../../pri/MDAlarmConfiger.pri)
include(../../pri/MDRecipeConfiger.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDDBQueryService.pri)

include(../../pri/MDDBQueryConfigure.pri)
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDDBQueryEngine.pri)

#开始编译界面库
#---------------------------------------------------------------------
include(../../pri/MDCommandQueueEngineUI.pri)
include(../../pri/MDSysLogUI.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDFilterTreePlugin.pri)
include(../../pri/MDDataQueryUI.pri)
include(../../pri/MDObjBatchOperation.pri)

include(../../pri/MDObjectManagerUI.pri)
include(../../pri/MDActionSPUI.pri)
include(../../pri/MDActionUI.pri)

include(../../pri/MDRTDBManagerUI.pri)
include(../../pri/MDDataSelectUI.pri)
include(../../pri/MDDataArchivesUI.pri)

include(../../pri/MDSecurityPolicyExpert.pri)
include(../../pri/MDHistoryExpert.pri)

#include(../../pri/MDScriptUI.pri)
include(../../pri/MDEventUI.pri)
include(../../pri/MDEventActionUI.pri)
include(../../pri/MDAccountUI.pri)
include(../../pri/MDPowerGroupUI.pri)
include(../../pri/MDSecurityPolicyUI.pri)
include(../../pri/MDAlarmLevelUI.pri)
include(../../pri/MDAlarmConditionUI.pri)
include(../../pri/MDAlarmUI.pri)
include(../../pri/MDRuntimeUI.pri)
include(../../pri/MDRecipeUI.pri)

include(../../pri/MDSingleApplication.pri)
include(../../pri/MDDBQueryUI.pri)

#开始编译运行时库
#---------------------------------------------------------------------
include(../../pri/MDVariableExpert.pri)
include(../../pri/MDActionExpert.pri)
include(../../pri/MDEventExpert.pri)
include(../../pri/MDScriptExpert.pri)
#include(../../pri/MDAlarmExpert.pri)
include(../../pri/MDLogExpert.pri)
include(../../pri/MDRedisClient.pri)
include(../../pri/MDDeviceCloudExpert.pri)
include(../../pri/MDRecipeExpert.pri)
include(../../pri/MDUaServer.pri)
#include(../../pri/MDUaServerSrc.pri)
#include(../../pri/MDUaSingleton.pri)
include(../../pri/MDKVClient.pri)
#-------------------------------------------------
include(../../pri/MDUaExpert.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

#支持c++11,可以支持类似Enum::enumValue之类的语法?
CONFIG += c++11

SOURCES += \
    ../../Project/MDProjectExpert/MDProjectExpert.cpp \
    ../../Project/MDProjectExpert/MDProjectExpertImp.cpp \
    ../../Project/MDProjectExpert/MDProjectModuleContacterImp.cpp \
    ../../Project/MDProjectExpert/MDProjectModuleExpertProxy.cpp \
    ../../Project/MDProjectExpert/MDProjectContacter.cpp \
    ../../Project/MDProjectExpert/MDProjectWrappers.cpp \
    ../../Project/MDProjectExpert/MDProjectUaWrappers.cpp \
    ../../Project/MDProjectExpert/MDProjectWrappersCollecter.cpp \
    ../../Project/MDProjectExpert/MDProjectWrappersUaCollecter.cpp \
    ../../Project/MDProjectExpert/IMDProjectExpertImp.cpp \
    ../../Project/MDProjectExpert/MDStdTimer.cpp \
    ../../Project/MDProjectExpert/MDProjectModuleExpert.cpp \
    ../../Project/MDProjectExpert/CommandEngineForExecuteHMIAction.cpp \
    ../../Project/MDProjectExpert/CommandEngineForHandleChangedVariable.cpp \
    ../../Project/MDProjectExpert/CommandEngineInMainThread.cpp \
    ../../Project/MDProjectExpert/MDEvent.cpp \
    ../../Project/MDProjectExpert/MDRecipeEvent.cpp

HEADERS += \
    ../../Project/MDProjectExpert/MDProjectExpertImp.h \
    ../../Project/MDProjectExpert/MDProjectModuleContacterImp.h \
    ../../include/MDProjectExpert/MDProjectExpert.h \
    ../../include/MDProjectExpert/MDProjectExpert_Global.h \
    ../../Project/MDProjectExpert/MDProjectModuleExpertProxy.h \
    ../../include/MDProjectExpert/MDProjectContacter.h \
    ../../Project/MDProjectExpert/MDProjectWrappers.h \
    ../../Project/MDProjectExpert/MDProjectUaWrappers.h \
    ../../Project/MDProjectExpert/MDProjectWrappersCollecter.h \
    ../../Project/MDProjectExpert/MDProjectWrappersUaCollecter.h \
    ../../include/MDProjectExpert/IMDProjectExpert.h \
    ../../Project/MDProjectExpert/IMDProjectExpertImp.h \
    ../../Project/MDProjectExpert/MDStdTimer.h \
    ../../Project/MDProjectExpert/MDProjectModuleExpert.h \
    ../../Project/MDProjectExpert/CommandEngineForExecuteHMIAction.h \
    ../../Project/MDProjectExpert/CommandEngineForHandleChangedVariable.h \
    ../../Project/MDProjectExpert/CommandEngineInMainThread.h \
    ../../Project/MDProjectExpert/MDEvent.h \
    ../../Project/MDProjectExpert/MDRecipeEvent.h


# MDAlarmExpert
SOURCES += \
    ../../Alarm/MDAlarmExpert/MDAlarmExpert.cpp \
    ../../Alarm/MDAlarmExpert/MDAlarmExpertImp.cpp \
    ../../Alarm/MDAlarmExpert/MDAlarmVariable.cpp \
    ../../Alarm/MDAlarmExpert/MDAlarmConditionRT.cpp \
    ../../Alarm/MDAlarmExpert/MDAlarmRecord.cpp \
    ../../Alarm/MDAlarmExpert/MDAlarmRecordMgr.cpp \
    ../../Alarm/MDAlarmExpert/MDAlarmRecordStatusMgr.cpp \
    ../../Alarm/MDAlarmExpert/MDAlarmContacter.cpp \
    ../../Alarm/MDAlarmExpert/MDAlarmCommandEngine.cpp \
    ../../Alarm/MDAlarmExpert/MDOffNormalAlarm.cpp \
    ../../Alarm/MDAlarmExpert/MDExclusiveLevelAlarm.cpp \
    ../../Alarm/MDAlarmExpert/CommonDevice/MDCommonDevice.cpp \
    ../../Alarm/MDAlarmExpert/CommonDevice/MDCommonDeviceAlarm.cpp \
    ../../Alarm/MDAlarmExpert/CommonDevice/MDCommonDeviceMgr.cpp

HEADERS += \
    ../../Alarm/MDAlarmExpert/MDAlarmExpertImp.h \
    ../../include/MDAlarmExpert/MDAlarmExpert.h \
    ../../include/MDAlarmExpert/MDAlarmExpert_Global.h \
    ../../Alarm/MDAlarmExpert/MDAlarmVariable.h \
    ../../Alarm/MDAlarmExpert/MDAlarmConditionRT.h \
    ../../Alarm/MDAlarmExpert/MDAlarmRecord.h \
    ../../Alarm/MDAlarmExpert/MDAlarmRecordMgr.h \
    ../../Alarm/MDAlarmExpert/MDAlarmRecordStatusMgr.h \
    ../../include/MDAlarmExpert/MDAlarmContacter.h \
    ../../Alarm/MDAlarmExpert/MDAlarmCommandEngine.h \
    ../../Alarm/MDAlarmExpert/MDOffNormalAlarm.h \
    ../../Alarm/MDAlarmExpert/MDExclusiveLevelAlarm.h \
    ../../Alarm/MDAlarmExpert/CommonDevice/MDCommonDevice.h \
    ../../Alarm/MDAlarmExpert/CommonDevice/MDCommonDeviceAlarm.h \
    ../../Alarm/MDAlarmExpert/CommonDevice/MDCommonDeviceMgr.h



# MDRTDBManager
SOURCES += \
    ../../RTDB/MDRTDBManager/MDRTDBManager.cpp \
    ../../RTDB/MDRTDBManager/MDChangeVariable.cpp \
    ../../RTDB/MDRTDBManager/MDRTDBVariableUaBase.cpp \
    ../../RTDB/MDRTDBManager/MDVariableChangeThread.cpp \
    ../../RTDB/MDRTDBManager/MDVariableGroup.cpp \
    ../../RTDB/MDRTDBManager/MDVariableManager.cpp \
    ../../RTDB/MDRTDBManager/MDVariableTreeModel.cpp \
    ../../RTDB/MDRTDBManager/MDRTDBManagerFactory.cpp \
    ../../RTDB/MDRTDBManager/MDDeviceTreeModel.cpp \
    ../../RTDB/MDRTDBManager/MDVariableQueueManager.cpp \
    ../../RTDB/MDRTDBManager/MDRTDBDataExchangeEngine.cpp \
    ../../RTDB/MDRTDBManager/MDRTDBSimulatiorEngine.cpp \
    ../../RTDB/MDRTDBManager/MDVariableRunTreeModel.cpp \
    ../../RTDB/MDRTDBManager/MDDeviceRunTreeModel.cpp \
    ../../RTDB/MDRTDBManager/MDSystemVariableTimer.cpp \
    ../../RTDB/MDRTDBManager/MDVariableDataConverter.cpp \
    ../../RTDB/MDRTDBManager/MDRTDBVariable.cpp


HEADERS += \
    ../../RTDB/MDRTDBManager/MDRTDBManager.h \
    ../../RTDB/MDRTDBManager/MDChangeVariable.h \
    ../../RTDB/MDRTDBManager/MDRTDBVariableUaBase.h \
    ../../RTDB/MDRTDBManager/MDVariableChangeThread.h \
    ../../RTDB/MDRTDBManager/MDVariableGroup.h \
    ../../RTDB/MDRTDBManager/MDVariableManager.h \
    ../../RTDB/MDRTDBManager/MDVariableTreeModel.h \
#    ../../RTDB/MDRTDBManager/MDRTDBManagerFactory.h \
    ../../RTDB/MDRTDBManager/MDDeviceTreeModel.h \
    ../../include/MDRTDBManager/MDRTDBManager_global.h \
    ../../include/MDRTDBManager/MDRTDBManagerFactory.h \
    ../../include/MDMultiLanguage/MDMultiLanguage.h \
    ../../RTDB/MDRTDBManager/MDVariableQueueManager.h \
    ../../RTDB/MDRTDBManager/MDRTDBDataExchangeEngine.h \
    ../../RTDB/MDRTDBManager/MDRTDBSimulatiorEngine.h \
    ../../RTDB/MDRTDBManager/MDVariableRunTreeModel.h \
    ../../RTDB/MDRTDBManager/MDDeviceRunTreeModel.h \
    ../../RTDB/MDRTDBManager/MDSystemVariableTimer.h \
    ../../RTDB/MDRTDBManager/MDVariableDataConverter.h \
    ../../RTDB/MDRTDBManager/MDRTDBVariable.h
