#-------------------------------------------------
#
# Project created by QtCreator 2016-05-19T15:57:47
#
#-------------------------------------------------

QT       += widgets

QT       -= gui

TARGET = MDSwitchFactoryUI
TEMPLATE = lib

DEFINES += MDSWITCHFACTORYUI_LIBRARY

#-------------------------------------------------
include(../../../pri/MDBaseForm.pri)
include(../../../pri/MDPropertyBrowser.pri)
include(../../../pri/MDObjBatchOperation.pri)
include(../../../pri/MDObjectManagerUI.pri)
include(../../../pri/MDMObjFactory.pri)
include(../../../pri/MDSwitchFactory.pri)
include(../../../pri/MDMultiLanguage.pri)
#-------------------------------------------------
include(../../../pri/MDSwitchFactoryUI.pri)
ProjectName = $${LibraryName}
include(../../../pri/common3.pri)
INCLUDEPATH += ../../../include/interface/Common

SOURCES += MDSwitchFactoryUI.cpp \
    MDSwitchTreeWindow.cpp

HEADERS += MDSwitchTreeWindow.h\
    ../../../include/MDSwitchFactoryUI/MDSwitchFactoryUI.h \
    ../../../include/MDSwitchFactoryUI/MDSwitchFactoryUI_Global.h

