#-------------------------------------------------
#
# Project created by QtCreator 2015-06-12T15:12:52
#
#-------------------------------------------------

QT       += core widgets

TARGET = MDActionUI
TEMPLATE = lib

DEFINES += MDACTIONUI_LIBRARY

#-------------------------------------------------
include(../../pri/MDActionIDHelper.pri)
include(../../pri/MDActionHelper.pri)
include(../../pri/MDEventHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDActionConfiger.pri)
include(../../pri/MDObjectManagerUI.pri)
include(../../pri/MDActionSPUI.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDObjBatchOperation.pri)
include(../../pri/MDMultiLanguage.pri)
#-------------------------------------------------
include(../../pri/MDActionUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDActionUI.cpp \
    MDActionUIImp.cpp \
    MDActionGeneralDialog.cpp \
    MDActionMainDialog.cpp \
    MDActionParaDialog_VariableWriteValue.cpp \
    MDActionTreeWindow.cpp \
    MDActionSPListDialogEx.cpp \
    MDActionObjectsForBrowser.cpp

HEADERS +=\
    MDActionUIImp.h \
    ../../include/MDActionUI/MDActionUI.h \
    ../../include/MDActionUI/MDActionUI_Global.h \
    MDActionGeneralDialog.h \
    MDActionMainDialog.h \
    MDActionParaDialog_VariableWriteValue.h \
    MDActionTreeWindow.h \
    MDActionSPListDialogEx.h \
    MDStringResource.h \
    MDActionObjectsForBrowser.h

FORMS += \
    MDActionGeneralDialog.ui \
    MDActionParaDialog_VariableWriteValue.ui
