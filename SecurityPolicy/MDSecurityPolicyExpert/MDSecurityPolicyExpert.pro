#-------------------------------------------------
#
# Project created by QtCreator 2015-07-30T17:15:17
#
#-------------------------------------------------

QT       -= gui

TARGET = MDSecurityPolicyExpert
TEMPLATE = lib

DEFINES += MDSECURITYPOLICYEXPERT_LIBRARY
RELEASE_SEPARATE_DEBUG = $$(CUSTOM_RELEASE_SEPARATE_DEBUG)
win32:!win32-g++ {
equals(RELEASE_SEPARATE_DEBUG, "SEPARATE") {
    CONFIG(debug, debug|release){
        DEFINES += MDKVCLIENT_LIBRARY_MT
        !build_pass:message('MDSecurityPolicyExpert -->>(DEFINES, MDKVCLIENT_LIBRARY_MT)')
    }
    else{
        DEFINES += MDKVCLIENT_LIBRARY_MD
        !build_pass:message('MDSecurityPolicyExpert -->>(DEFINES, MDKVCLIENT_LIBRARY_MD)')
    }
}
else {
#    DEFINES += MDKVCLIENT_LIBRARY_MT
    CONFIG(debug, debug|release){
        DEFINES += MDKVCLIENT_LIBRARY_MD
        !build_pass:message('MDSecurityPolicyExpert -->>(DEFINES, MDKVCLIENT_LIBRARY_MT)')
    }
    else{
        DEFINES += MDKVCLIENT_LIBRARY_MT
        !build_pass:message('MDSecurityPolicyExpert -->>(DEFINES, MDKVCLIENT_LIBRARY_MD)')
    }
}
}
#-------------------------------------------------
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDAccountConfiger.pri)
include(../../pri/MDPowerGroupConfiger.pri)
include(../../pri/MDAccountStrategyConfiger.pri)
include(../../pri/MDSecurityPolicyConfiger.pri)
include(../../pri/MDLogHelper.pri)
include(../../pri/MDActionIDHelper.pri)
include(../../pri/MDCommonHelper.pri)
include(../../pri/MDKVClient.pri)
#-------------------------------------------------
include(../../pri/MDSecurityPolicyExpert.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDSecurityPolicyExpert.cpp \
    MDSecurityPolicyExpertImp.cpp \
    MDSecurityPolicyContacter.cpp

HEADERS +=\
    MDSecurityPolicyExpertImp.h \
    ../../include/MDSecurityPolicyExpert/MDSecurityPolicyExpert_Global.h \
    ../../include/MDSecurityPolicyExpert/MDSecurityPolicyExpert.h \
    ../../include/MDSecurityPolicyExpert/MDSecurityPolicyContacter.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
