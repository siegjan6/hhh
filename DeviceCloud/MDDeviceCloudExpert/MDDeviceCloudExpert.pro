#-------------------------------------------------
#
# Project created by QtCreator 2015-11-19T16:23:00
#
#-------------------------------------------------

QT       -= gui
QT       += network core

TARGET = MDDeviceCloudExpert
TEMPLATE = lib

DEFINES += MDDEVICECLOUDEXPERT_LIBRARY
RELEASE_SEPARATE_DEBUG = $$(CUSTOM_RELEASE_SEPARATE_DEBUG)
win32:!win32-g++ {
equals(RELEASE_SEPARATE_DEBUG, "SEPARATE") {
    CONFIG(debug, debug|release){
        DEFINES += MDKVCLIENT_LIBRARY_MT
        !build_pass:message('MDDeviceCloudExpert -->>(DEFINES, MDKVCLIENT_LIBRARY_MT)')
    }
    else{
        DEFINES += MDKVCLIENT_LIBRARY_MD
        !build_pass:message('MDDeviceCloudExpert -->>(DEFINES, MDKVCLIENT_LIBRARY_MD)')
    }
}
else {
#    DEFINES += MDKVCLIENT_LIBRARY_MT
    CONFIG(debug, debug|release){
        DEFINES += MDKVCLIENT_LIBRARY_MD
        !build_pass:message('MDDeviceCloudExpert -->>(DEFINES, MDKVCLIENT_LIBRARY_MT)')
    }
    else{
        DEFINES += MDKVCLIENT_LIBRARY_MT
        !build_pass:message('MDDeviceCloudExpert -->>(DEFINES, MDKVCLIENT_LIBRARY_MD)')
    }
}
}
#-------------------------------------------------
include(../../pri/MDCommandQueueEngine.pri)
include(../../pri/MDSysLogExpert.pri)
include(../../pri/MDRedisClient.pri)
include(../../pri/MDSwitchFactory.pri)
include(../../pri/MDKVClient.pri)
#-------------------------------------------------
include(../../pri/MDDeviceCloudExpert.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDDeviceCloudExpert.cpp \
    MDDeviceCloudExpertImp.cpp \
    MDDeviceCloudContacter.cpp \
    MDDeviceCloudClient.cpp \
    MDDeviceGetter.cpp \
    MDDeviceCommandEngine.cpp

HEADERS += MDDeviceCloudExpertImp.h \
    MDDeviceCloudClient.h \
    ../../include/MDDeviceCloudExpert/MDDeviceCloudContacter.h \
    ../../include/MDDeviceCloudExpert/MDDeviceCloudExpert.h \
    ../../include/MDDeviceCloudExpert/MDDeviceCloudExpert_Global.h \
    MDDeviceGetter.h \
    MDDeviceCommandEngine.h
