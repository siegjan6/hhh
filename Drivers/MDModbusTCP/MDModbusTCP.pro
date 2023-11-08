#-------------------------------------------------
#
# Project created by QtCreator 2014-03-31T15:47:33
#
#-------------------------------------------------
CONFIG += c++11
QT       -= gui
QT       +=network
QT       += widgets
QT       += xml
#TARGET = MDModbusTCP
TEMPLATE = lib

DEFINES += MDMODBUSTCP_LIBRARY

SOURCES += MDModbusTCP.cpp \
    MDDriver.cpp \
    MDChannel.cpp \
    MDDevice.cpp \
    MDDataBlock.cpp \
    MDProtocolCommand.cpp \
    MDDeviceConfigDialog.cpp \
    MDDataBlockDialog.cpp

HEADERS += MDModbusTCP.h\
    MDDriver.h \
    MDChannel.h \
    MDDevice.h \
    MDDataBlock.h \
    MDProtocolCommand.h \
    MDDeviceConfigDialog.h \
    MDDataBlockDialog.h \
    qextserialport_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH +=../../include/interface/Common/

INCLUDEPATH +=../../include/MDDriverCommunicationLibs
INCLUDEPATH +=../../include/MDDriverInterfaces
INCLUDEPATH +=../../include/MDDriverLibs
INCLUDEPATH +=../../include/MDRTDBInterfaces

include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDQJson2SortJson.pri)
include(../../pri/common_driver.pri)

contains(TEMPLATE, app){

}else{
#去除lib前缀和版本号后缀
CONFIG += plugin no_plugin_name_prefix
#lib前缀还是不能少的
unix:ProjectName = lib$${ProjectName}
}

CONFIG(debug,debug|release){
DESTDIR = ../../bin/debug/drivers
#TARGET = Mind.RTDB.Drivers.MDModbusTCP
TARGET = MDDRV.ModBusTCPC
LIBS +=  \
        -L../../bin/debug\
        -lMind.RTDB.MDDriverCommunicationLibsd\
        -lMind.RTDB.MDDriverLibsd\
}
else{
DESTDIR = ../../bin/release/drivers
#TARGET = Mind.RTDB.Drivers.MDModbusTCP
TARGET = MDDRV.ModBusTCPC
LIBS +=  \
        -L../../bin/release\
        -lMind.RTDB.MDDriverCommunicationLibs\
        -lMind.RTDB.MDDriverLibs\
}

FORMS += \
    MDDeviceConfigDialog.ui \
    MDDataBlockDialog.ui
