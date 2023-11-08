#-------------------------------------------------
#
# Project created by QtCreator 2014-03-31T09:09:15
#
#-------------------------------------------------
CONFIG += c++11
QT       -= gui
QT       += xml
QT       += core
QT       += widgets
#QT       += network


TARGET = MDDriverLibs
TEMPLATE = lib


DEFINES += MDDRIVERLIBS_LIBRARY
include(../../pri/MDDriverCommunicationLibs.pri)
#多语言
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDDriverLibs.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDSwitchFactory.pri)
include(../../pri/MDPropertyBrowser.pri)
#QJson->SortJson
include(../../pri/MDQJson2SortJson.pri)

INCLUDEPATH += ../../include/

SOURCES += \
    MDDriverManager.cpp \
    BaseDriver.cpp \
    BaseChannel.cpp \
    CommunicationEvent.cpp \
    MDDriverManagerFactory.cpp \
    BaseProtocolCommand.cpp \
    BaseDevice.cpp \
    BaseDataBlock.cpp \
    MDDriverInformation.cpp \
    MDDriverFunctions.cpp \
    CustomEvent.cpp

HEADERS +=\
    MDDriverManager.h \
    MDWriteVariable.h \
#    BaseDriver.h \
    CommunicationEvent.h \
    MDDriverManagerFactory.h \
    BaseProtocolCommand.h \
#    BaseDevice.h \
#    BaseDataBlock.h \
    ../../include/MDDriverLibs/BaseChannel.h \
    ../../include/MDDriverLibs/BaseDataBlock.h \
    ../../include/MDDriverLibs/BaseDevice.h \
    ../../include/MDDriverLibs/BaseDriver.h \
    ../../include/MDDriverLibs/BaseProtocolCommand.h \
    ../../include/MDDriverLibs/MDDriverManagerFactory.h \
    ../../include/MDDriverLibs/MDDriverFunctions.h \
    ../../include/MDDriverLibs/MDDriverLibs_global.h \
    MDDriverInformation.h \
    CustomEvent.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
