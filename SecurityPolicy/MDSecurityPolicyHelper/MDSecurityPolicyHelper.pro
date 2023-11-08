#-------------------------------------------------
#
# Project created by QtCreator 2015-07-25T15:25:19
#
#-------------------------------------------------

QT       -= gui

TARGET = MDSecurityPolicyHelper
TEMPLATE = lib

DEFINES += MDSECURITYPOLICYHELPER_LIBRARY
#-------------------------------------------------
include(../../pri/MDActionIDHelper.pri)
#-------------------------------------------------
include(../../pri/MDSecurityPolicyHelper.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += \
    MDAccountPropertyData.cpp \
    MDAccountProperty_Config.cpp \
    MDAccountProperty_Runtime.cpp \
    MDAccountData.cpp \
    MDAccount.cpp \
    MDPowerGroup.cpp \
    MDAccountStrategy.cpp \
    MDAccountStrategyData.cpp \
    MDPowerGroupData.cpp \
    MDActionSPData.cpp \
    MDActionSP.cpp \
    MDActionSPSessionData.cpp \
    MDActionSPSession.cpp \
    MDSigInput.cpp \
    MDSigOutput.cpp \
    MDActionSPListData.cpp \
    MDSecurityPolicyParaData.cpp \
    MDSecurityPolicyPara.cpp

HEADERS +=\
    MDAccountPropertyData.h \
    ../../include/MDSecurityPolicyHelper/MDSecurityPolicyHelper_Global.h \
    ../../include/MDSecurityPolicyHelper/MDAccountProperty_Config.h \
    MDAccountData.h \
    ../../include/MDSecurityPolicyHelper/MDAccountProperty_Runtime.h \
    ../../include/MDSecurityPolicyHelper/MDAccount.h \
    ../../include/MDSecurityPolicyHelper/MDPowerGroup.h \
    ../../include/MDSecurityPolicyHelper/MDAccountStrategy.h \
    MDPowerGroupData.h \
    MDActionSPData.h \
    ../../include/MDSecurityPolicyHelper/MDSecurityPolicyConstant.h \
    ../../include/MDSecurityPolicyHelper/MDActionSP.h \
    MDActionSPSessionData.h \
    ../../include/MDSecurityPolicyHelper/MDSigOutput.h \
    ../../include/MDSecurityPolicyHelper/MDSigInput.h \
    ../../include/MDSecurityPolicyHelper/MDActionSPSession.h \
    MDActionSPListData.h \
    MDSecurityPolicyParaData.h \
    ../../include/MDSecurityPolicyHelper/MDSecurityPolicyPara.h


