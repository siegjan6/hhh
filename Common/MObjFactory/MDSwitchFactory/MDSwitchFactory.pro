#-------------------------------------------------
#
# Project created by QtCreator 2016-05-05T13:18:02
#
#-------------------------------------------------

QT       -= gui

TARGET = MDSwitchFactory
TEMPLATE = lib

DEFINES += MDSWITCHFACTORY_LIBRARY

#-------------------------------------------------
include(../../../pri/MDMObjFactory.pri)
include(../../../pri/MDSysLogExpert.pri)
#-------------------------------------------------
include(../../../pri/MDSwitchFactory.pri)
ProjectName = $${LibraryName}
include(../../../pri/common3.pri)

SOURCES += MDSwitchFactory.cpp \
    MDSwitchFactoryImp.cpp \
    MDFuncSwitchFactory.cpp \
    MDLogSwitchFactory.cpp

HEADERS += MDSwitchFactoryImp.h \
    ../../../include/MDSwitchFactory/MDSwitchFactory.h \
    ../../../include/MDSwitchFactory/MDSwitchFactory_Global.h \
    ../../../include/MDSwitchFactory/MDFuncSwitchFactory.h \
    ../../../include/MDSwitchFactory/MDLogSwitchFactory.h
