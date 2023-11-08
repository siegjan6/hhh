#-------------------------------------------------
#
# Project created by QtCreator 2015-06-08T15:19:48
#
#-------------------------------------------------

QT       += core widgets

TARGET = MDEventUI
TEMPLATE = lib

DEFINES += MDEVENTUI_LIBRARY
#-------------------------------------------------
include(../../pri/MDBaseForm.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDEventConfiger.pri)
include(../../pri/MDActionHelper.pri)
include(../../pri/MDEventHelper.pri)
include(../../pri/MDObjectManagerUI.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDObjBatchOperation.pri)
include(../../pri/MDMultiLanguage.pri)
#-------------------------------------------------
include(../../pri/MDEventUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDEventUI.cpp \
    MDEventUIImp.cpp \
    MDEventMainDialog.cpp \
    MDEventGeneralDialog.cpp \
    MDEventActionDialog.cpp \
    MDEventConditionDialog_VariableVCIR.cpp \
    MDEventConditionDialog_Combined.cpp \
    MDEventConditionDialog_VariableQC.cpp \
    MDEventConditionDialog_CycleTimer.cpp \
    MDEventTreeWindow.cpp \
    MDEventObjectsForBrowser.cpp

HEADERS += ../../include/MDEventUI/MDEventUI.h \
    ../../include/MDEventUI/MDEventUI_Global.h \
    MDEventUIImp.h \
    MDEventMainDialog.h \
    MDEventGeneralDialog.h \
    MDEventActionDialog.h \
    MDEventConditionDialog_VariableVCIR.h \
    MDEventConditionDialog_Combined.h \
    MDEventConditionDialog_VariableQC.h \
    MDEventConditionDialog_CycleTimer.h \
    MDEventTreeWindow.h \
    MDStringResource.h \
    MDEventObjectsForBrowser.h

FORMS += \
    MDEventGeneralDialog.ui \
    MDEventActionDialog.ui \
    MDEventConditionDialog_VariableVCIR.ui \
    MDEventConditionDialog_Combined.ui \
    MDEventConditionDialog_VariableQC.ui \
    MDEventConditionDialog_CycleTimer.ui

