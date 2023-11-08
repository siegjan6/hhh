#-------------------------------------------------
#
# Project created by QtCreator 2015-08-27T18:52:47
#
#-------------------------------------------------

QT       += widgets

TARGET = MDRuntimeUI
TEMPLATE = lib

DEFINES += MDRUNTIMEUI_LIBRARY
#-------------------------------------------------
include(../../pri/MDUtility.pri)
include(../../pri/MDCommonHelper.pri)
include(../../pri/MDExpression.pri)
include(../../pri/MDDBConnect.pri)
include(../../pri/MDCommandQueueEngine.pri)
include(../../pri/MDSysLogExpert.pri)
include(../../pri/MDSQLDataBaseProxy.pri)

include(../../pri/MDActionIDHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDLogHelper.pri)
include(../../pri/MDProjectHelper.pri)
include(../../pri/MDMultiLanguage.pri)

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
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDDBQueryService.pri)

#开始编译界面库
#---------------------------------------------------------------------
include(../../pri/MDSysLogUI.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDFilterTreePlugin.pri)
include(../../pri/MDDataQueryUI.pri)

include(../../pri/MDObjectManagerUI.pri)
include(../../pri/MDActionSPUI.pri)
include(../../pri/MDActionUI.pri)

include(../../pri/MDRTDBManagerUI.pri)
include(../../pri/MDDataSelectUI.pri)
include(../../pri/MDDataArchivesUI.pri)

include(../../pri/MDColorManager.pri)
include(../../pri/MDDrawObj.pri)
include(../../pri/MDDrawNode.pri)
include(../../pri/MDCurve.pri)
include(../../pri/MDRecipeGrid.pri)
include(../../pri/MDGauge.pri)
include(../../pri/MDDiagram.pri)
include(../../pri/MDDataGroupGrid.pri)
include(../../pri/MDAlarmGrid.pri)
include(../../pri/MDLogGrid.pri)
include(../../pri/MDDataArchivesWidget.pri)
include(../../pri/MDDrawSpecial.pri)
include(../../pri/MDDrawVectors.pri)
include(../../pri/MDDrawWidgets.pri)
include(../../pri/MDHMIForm.pri)
include(../../pri/MDGraphicsStorage.pri)
include(../../pri/MDHMIFramework.pri)

include(../../pri/MDSecurityPolicyExpert.pri)
include(../../pri/MDHistoryExpert.pri)

#include(../../pri/MDReportDesigner.pri)
#include(../../pri/MDScriptUI.pri)
include(../../pri/MDEventUI.pri)
include(../../pri/MDEventActionUI.pri)
include(../../pri/MDAccountUI.pri)
include(../../pri/MDPowerGroupUI.pri)
include(../../pri/MDSecurityPolicyUI.pri)
include(../../pri/MDAlarmLevelUI.pri)
include(../../pri/MDAlarmConditionUI.pri)
include(../../pri/MDAlarmUI.pri)
#-------------------------------------------------
include(../../pri/MDRuntimeUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDRuntimeUI.cpp \
    MDOPCUAServerParaForm.cpp \
    MDRuntimeUIImp.cpp \
    MDRuntimeParaMainDialog.cpp \
    MDRuntimePara.cpp \
    MDRuntimeParaDialog.cpp \
    MDProjectParaForm.cpp

HEADERS +=\
    MDOPCUAServerParaForm.h \
    MDRuntimeUIImp.h \
    ../../include/MDRuntimeUI/MDRuntimeUI.h \
    ../../include/MDRuntimeUI/MDRuntimeUI_Global.h \
    MDRuntimeParaMainDialog.h \
    MDRuntimePara.h \
    MDRuntimeParaDialog.h \
    MDProjectParaForm.h \
    MDStringResource.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    MDOPCUAServerParaForm.ui \
    MDRuntimeParaDialog.ui \
    MDProjectParaForm.ui
