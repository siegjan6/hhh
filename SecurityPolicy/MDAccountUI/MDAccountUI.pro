#-------------------------------------------------
#
# Project created by QtCreator 2015-08-08T11:59:36
#
#-------------------------------------------------

QT       += widgets

TARGET = MDAccountUI
TEMPLATE = lib

DEFINES += MDACCOUNTUI_LIBRARY
#-------------------------------------------------
include(../../pri/MDEventHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDAccountConfiger.pri)
include(../../pri/MDPowerGroupConfiger.pri)
include(../../pri/MDAccountStrategyConfiger.pri)
include(../../pri/MDSecurityPolicyConfiger.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDActionSPUI.pri)
include(../../pri/MDAccountConfiger.pri)
include(../../pri/MDObjBatchOperation.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDObjectManagerUI.pri)
include(../../pri/MDMultiLanguage.pri)
#-------------------------------------------------
include(../../pri/MDAccountUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDAccountUI.cpp \
    MDAccountTreeWindow.cpp \
    MDAccountPasswordDialog.cpp \
    MDAccountMainDialog.cpp \
    MDAccountGeneralDialog.cpp \
    MDAccountUIImp.cpp \
    MDAccountSPListDialog.cpp \
    MDAccountForBrowser.cpp

HEADERS +=\
    MDAccountTreeWindow.h \
    MDAccountPasswordDialog.h \
    MDAccountMainDialog.h \
    MDAccountGeneralDialog.h \
    MDAccountUIImp.h \
    ../../include/MDAccountUI/MDAccountUI_Global.h \
    ../../include/MDAccountUI/MDAccountUI.h \
    MDAccountSPListDialog.h \
    MDStringResource.h \
    MDAccountForBrowser.h

FORMS += \
    MDAccountGeneralDialog.ui \
    MDAccountPasswordDialog.ui

