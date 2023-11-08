#-------------------------------------------------
#
# Project created by QtCreator 2016-04-25T15:53:53
#
#-------------------------------------------------

QT       -= gui

TARGET = MDMObjFactory
TEMPLATE = lib

DEFINES += MDMOBJFACTORY_LIBRARY

include(../../../pri/MDMObjFactory.pri)
ProjectName = $${LibraryName}
include(../../../pri/common3.pri)

SOURCES += MDMObjFactory.cpp \
    MDMObjPara.cpp \
    MDMObj.cpp \
    MDFactoryManager.cpp \
    MDMObjImp.cpp \
    MDMObjFactoryImp.cpp \
    MDFactoryFile.cpp

HEADERS += \
    ../../../include/MDMObjFactory/MDMObj.h \
    ../../../include/MDMObjFactory/MDMObjFactory.h \
    ../../../include/MDMObjFactory/MDMObjFactory_Global.h \
    ../../../include/MDMObjFactory/MDMObjPara.h \
    MDMObjImp.h \
    MDMObjFactoryImp.h \
    MDFactoryManager.h \
    MDFactoryFile.h

