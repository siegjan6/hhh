#-------------------------------------------------
#
# Project created by QtCreator 2015-07-31T10:37:27
#
#-------------------------------------------------

QT       += widgets

TARGET = MDSecurityPolicyUI
TEMPLATE = lib

DEFINES += MDSECURITYPOLICYUI_LIBRARY
#-------------------------------------------------
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDPowerGroupConfiger.pri)
include(../../pri/MDSecurityPolicyConfiger.pri)
include(../../pri/MDAccountConfiger.pri)
include(../../pri/MDAccountStrategyConfiger.pri)
include(../../pri/MDObjectManagerUI.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDSecurityPolicyExpert.pri)
include(../../pri/MDAccountUI.pri)
include(../../pri/MDPowerGroupUI.pri)
include(../../pri/MDEventHelper.pri)
include(../../pri/MDActionIDHelper.pri)
include(../../pri/MDProjectConfiger.pri)
#-------------------------------------------------
include(../../pri/MDSecurityPolicyUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDSecurityPolicyUI.cpp \
    MDSecurityCheckDialog.cpp \
    MDLoginDialog.cpp \
    MDSecurityPolicyUIImp.cpp \
    MDESigCheckDialog.cpp \
    MDNSigCheckDialog.cpp \
    MDAccountStrategyMainDialog.cpp \
    MDASAccountPasswordDialog.cpp \
    MDSecurityPolicyMainDialog.cpp \
    MDAccountMgrDialog.cpp \
    MDPowerGroupMgrDialog.cpp

HEADERS +=\
    MDSecurityCheckDialog.h \
    MDLoginDialog.h \
    MDSecurityPolicyUIImp.h \
    ../../include/MDSecurityPolicyUI/MDSecurityPolicyUI_Global.h \
    ../../include/MDSecurityPolicyUI/MDSecurityPolicyUI.h \
    MDESigCheckDialog.h \
    MDNSigCheckDialog.h \
    MDAccountStrategyMainDialog.h \
    MDASAccountPasswordDialog.h \
    MDSecurityPolicyMainDialog.h \
    MDAccountMgrDialog.h \
    MDPowerGroupMgrDialog.h \
    MDStringResource.h

FORMS += \
    MDSecurityCheckDialog.ui \
    MDLoginDialog.ui \
    MDESigCheckDialog.ui \
    MDNSigCheckDialog.ui \
    MDASAccountPasswordDialog.ui \
    MDAccountMgrDialog.ui \
    MDPowerGroupMgrDialog.ui

