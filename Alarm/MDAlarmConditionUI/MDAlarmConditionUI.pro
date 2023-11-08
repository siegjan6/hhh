#-------------------------------------------------
#
# Project created by QtCreator 2015-08-19T14:20:38
#
#-------------------------------------------------

QT       += widgets

TARGET = MDAlarmConditionUI
TEMPLATE = lib

DEFINES += MDALARMCONDITIONUI_LIBRARY
#-------------------------------------------------
include(../../pri/MDAlarmHelper.pri)
include(../../pri/MDEventHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDAlarmConditionConfiger.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDObjectManagerUI.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDActionSPUI.pri)
include(../../pri/MDObjBatchOperation.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDMultiLanguage.pri)
#-------------------------------------------------
include(../../pri/MDAlarmConditionUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDAlarmConditionUI.cpp \
    MDAlarmConditionUIImp.cpp \
    MDAlarmConditionTreeWindow.cpp \
    MDAlarmConditionMainDialog.cpp \
    MDAlarmConditionGeneralDialog.cpp \
    MDAlarmConditionSPListDialog.cpp \
    MDAlarmConditionParaDialog.cpp \
    MDAlarmConditionForBrowser.cpp

HEADERS += MDAlarmConditionUIImp.h \
    MDAlarmConditionTreeWindow.h \
    MDAlarmConditionMainDialog.h \
    MDAlarmConditionGeneralDialog.h \
    MDAlarmConditionSPListDialog.h \
    ../../include/MDAlarmConditionUI/MDAlarmConditionUI.h \
    ../../include/MDAlarmConditionUI/MDAlarmConditionUI_Global.h \
    MDAlarmConditionParaDialog.h \
    MDStringResource.h \
    MDAlarmConditionForBrowser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    MDAlarmConditionGeneralDialog.ui \
    MDAlarmConditionParaDialog.ui
