#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T14:45:48
#
#-------------------------------------------------

QT       += gui

TARGET = MDProjectExpert
TEMPLATE = lib

DEFINES += MDPROJECTEXPERT_LIBRARY
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
# ignore MDRTDBManager replace with MDUaExpert
#include(../../pri/MDRTDBManager.pri)
include(../../pri/MDUaExpert.pri)

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
#include(../../pri/MDUaExpert.pri)
include(../../pri/MDKVClient.pri)
#-------------------------------------------------
include(../../pri/MDProjectExpert.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

#支持c++11,可以支持类似Enum::enumValue之类的语法?
CONFIG += c++11

SOURCES += \
    MDProjectExpert.cpp \
    MDProjectExpertImp.cpp \
    MDProjectModuleContacterImp.cpp \
    MDProjectModuleExpertProxy.cpp \
    MDProjectContacter.cpp \
    MDProjectUaWrappers.cpp \
    MDProjectWrappers.cpp \
    MDProjectWrappersCollecter.cpp \
    IMDProjectExpertImp.cpp \
    MDProjectWrappersUaCollecter.cpp \
    MDStdTimer.cpp \
    MDProjectModuleExpert.cpp \
    CommandEngineForExecuteHMIAction.cpp \
    CommandEngineForHandleChangedVariable.cpp \
    CommandEngineInMainThread.cpp \
    MDEvent.cpp \
    MDRecipeEvent.cpp

HEADERS += \
    MDProjectExpertImp.h \
    MDProjectModuleContacterImp.h \
    ../../include/MDProjectExpert/MDProjectExpert.h \
    ../../include/MDProjectExpert/MDProjectExpert_Global.h \
    MDProjectModuleExpertProxy.h \
    ../../include/MDProjectExpert/MDProjectContacter.h \
    MDProjectUaWrappers.h \
    MDProjectWrappers.h \
    MDProjectWrappersCollecter.h \
    ../../include/MDProjectExpert/IMDProjectExpert.h \
    IMDProjectExpertImp.h \
    MDProjectWrappersUaCollecter.h \
    MDStdTimer.h \
    MDProjectModuleExpert.h \
    CommandEngineForExecuteHMIAction.h \
    CommandEngineForHandleChangedVariable.h \
    CommandEngineInMainThread.h \
    MDEvent.h \
    MDRecipeEvent.h

