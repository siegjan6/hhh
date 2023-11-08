#-------------------------------------------------
#
# Project created by QtCreator 2015-02-11T16:05:21
#
#-------------------------------------------------
CONFIG += c++11
QT       -= gui
QT       += xml
QT       += widgets

TARGET = MDRTDBManager
TEMPLATE = lib

DEFINES += MDRTDBMANAGER_LIBRARY

include(../../pri/MDVariableArchives.pri)
include(../../pri/MDDriverLibs.pri)
include(../../pri/MDExpression.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDUaServer.pri)
include(../../pri/MDRTDBManager.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)
#多语言
include(../../pri/MDMultiLanguage.pri)
#日志
include(../../pri/MDSwitchFactory.pri)
#QJson->SortJson
include(../../pri/MDQJson2SortJson.pri)

SOURCES += \
    MDRTDBManager.cpp \
    MDChangeVariable.cpp \
    MDRTDBVariableUaBase.cpp \
    MDVariableChangeThread.cpp \
    MDVariableGroup.cpp \
    MDVariableManager.cpp \
    MDVariableTreeModel.cpp \
    MDRTDBManagerFactory.cpp \
    MDDeviceTreeModel.cpp \
    MDVariableQueueManager.cpp \
    MDRTDBDataExchangeEngine.cpp \
    MDRTDBSimulatiorEngine.cpp \
    MDVariableRunTreeModel.cpp \
    MDDeviceRunTreeModel.cpp \
    MDSystemVariableTimer.cpp \
    MDVariableDataConverter.cpp \
    MDRTDBVariable.cpp


HEADERS += \
    MDRTDBManager.h \
    MDChangeVariable.h \
    MDRTDBVariableUaBase.h \
    MDVariableChangeThread.h \
    MDVariableGroup.h \
    MDVariableManager.h \
    MDVariableTreeModel.h \
#    MDRTDBManagerFactory.h \
    MDDeviceTreeModel.h \
    ../../include/MDRTDBManager/MDRTDBManager_global.h \
    ../../include/MDRTDBManager/MDRTDBManagerFactory.h \
    ../../include/MDMultiLanguage/MDMultiLanguage.h \
    MDVariableQueueManager.h \
    MDRTDBDataExchangeEngine.h \
    MDRTDBSimulatiorEngine.h \
    MDVariableRunTreeModel.h \
    MDDeviceRunTreeModel.h \
    MDSystemVariableTimer.h \
    MDVariableDataConverter.h \
    MDRTDBVariable.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES +=

DISTFILES +=
