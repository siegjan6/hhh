#-------------------------------------------------
#
# Project created by QtCreator 2015-07-30T15:26:06
#
#-------------------------------------------------

QT       -= gui

TARGET = MDSecurityPolicyConfiger
TEMPLATE = lib

DEFINES += MDSECURITYPOLICYCONFIGER_LIBRARY
#-------------------------------------------------
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDAccountConfiger.pri)
include(../../pri/MDPowerGroupConfiger.pri)
include(../../pri/MDAccountStrategyConfiger.pri)
#-------------------------------------------------
include(../../pri/MDSecurityPolicyConfiger.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDSecurityPolicyConfiger.cpp \
    MDSecurityPolicyConfigerImp.cpp

HEADERS +=\
    MDSecurityPolicyConfigerImp.h \
    ../../include/MDSecurityPolicyConfiger/MDSecurityPolicyConfiger_Global.h \
    ../../include/MDSecurityPolicyConfiger/MDSecurityPolicyConfiger.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
