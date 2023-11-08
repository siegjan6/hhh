#-------------------------------------------------
#
# Project created by QtCreator 2015-08-08T14:40:33
#
#-------------------------------------------------

QT       += widgets

TARGET = MDPowerGroupUI
TEMPLATE = lib

DEFINES += MDPOWERGROUPUI_LIBRARY
#-------------------------------------------------
include(../../pri/MDEventHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDAccountConfiger.pri)
include(../../pri/MDPowerGroupConfiger.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDActionSPUI.pri)
include(../../pri/MDAccountUI.pri)
include(../../pri/MDAccountConfiger.pri)
include(../../pri/MDObjBatchOperation.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDObjectManagerUI.pri)
include(../../pri/MDMultiLanguage.pri)
#-------------------------------------------------
include(../../pri/MDPowerGroupUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDPowerGroupUI.cpp \
    MDPowerGroupUIImp.cpp \
    MDPowerGroupTreeWindow.cpp \
    MDPowerGroupMainDialog.cpp \
    MDPowerGroupGeneralDialog.cpp \
    MDPowerGroupAccountDialog.cpp \
    MDPowerGroupSPListDialog.cpp \
    MDPowerGroupForBrowser.cpp

HEADERS += MDPowerGroupUIImp.h \
    MDPowerGroupTreeWindow.h \
    MDPowerGroupMainDialog.h \
    MDPowerGroupGeneralDialog.h \
    ../../include/MDPowerGroupUI/MDPowerGroupUI_Global.h \
    ../../include/MDPowerGroupUI/MDPowerGroupUI.h \
    MDPowerGroupAccountDialog.h \
    MDPowerGroupSPListDialog.h \
    MDStringResource.h \
    MDPowerGroupForBrowser.h

FORMS += \
    MDPowerGroupGeneralDialog.ui \
    MDPowerGroupAccountDialog.ui
