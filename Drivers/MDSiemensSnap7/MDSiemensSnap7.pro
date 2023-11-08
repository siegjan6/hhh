
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
TEMPLATE = lib

DEFINES += MDSIEMENSSNAP7_LIBRARY
win32{
DEFINES += BM_PLATFORM_MSVC_X64
}

SOURCES += \
    MDDriver.cpp \
    MDChannel.cpp \
    MDDevice.cpp \
    MDDataBlock.cpp \
    MDProtocolCommand.cpp \
    MDDeviceConfigDialog.cpp \
    MDDataBlockDialog.cpp \
    MDSiemensSnap7.cpp \
    snap7.cpp

HEADERS +=\
    MDDriver.h \
    MDChannel.h \
    MDDevice.h \
    MDDataBlock.h \
    MDProtocolCommand.h \
    MDDeviceConfigDialog.h \
    MDDataBlockDialog.h \
    qextserialport_global.h \
    MDSiemensSnap7.h \
    snap7.h

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
TARGET = MDDRV.SiemensS7

if(contains(DEFINES,BM_PLATFORM_MSVC_X64)) {
    LIBS += -L../../bin/debug/drivers\
            -lsnap7
}

if(contains(DEFINES,BM_PLATFORM_MSVC_X32)) {
    LIBS += -L$$PWD/debug\
            -lsnap7-msvc-x32
}

if(contains(DEFINES,BM_PLATFORM_MINGW_X64)) {
    LIBS += -L$$PWD/debug\
            -lsnap7-mingw-x64
}

if(contains(DEFINES,BM_PLATFORM_MINGW_X32)) {
    LIBS += -L$$PWD/debug\
            -lsnap7-mingw-x32
}

unix {
    LIBS += -L../../bin/debug/drivers\
            -lsnap7
}

LIBS +=  \
        -L../../bin/debug\
        -lMind.RTDB.MDDriverCommunicationLibsd\
        -lMind.RTDB.MDDriverLibsd\
}
else{
DESTDIR = ../../bin/release/drivers
TARGET = MDDRV.SiemensS7

if(contains(DEFINES,BM_PLATFORM_MSVC_X64)) {
    LIBS += -L../../bin/release/drivers\
            -lsnap7
}

if(contains(DEFINES,BM_PLATFORM_MSVC_X32)) {
    LIBS += -L$$PWD/release\
            -lsnap7-msvc-x32
}

if(contains(DEFINES,BM_PLATFORM_MINGW_X64)) {
    LIBS += -L$$PWD/release\
            -lsnap7-mingw-x64
}

if(contains(DEFINES,BM_PLATFORM_MINGW_X32)) {
    LIBS += -L$$PWD/release\
            -lsnap7-mingw-x32
}

unix {
    LIBS += -L../../bin/release/drivers\
            -lsnap7
}

LIBS +=  \
        -L../../bin/release\
        -lMind.RTDB.MDDriverCommunicationLibs\
        -lMind.RTDB.MDDriverLibs\
}

FORMS += \
    MDDeviceConfigDialog.ui \
    MDDataBlockDialog.ui


# 解决msvc编译后中文乱码的问题
msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8
msvc:QMAKE_CXXFLAGS_WARN_ON += -wd4819
