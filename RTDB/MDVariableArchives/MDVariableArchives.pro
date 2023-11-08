#-------------------------------------------------
#
# Project created by QtCreator 2015-07-22T15:20:52
#
#-------------------------------------------------
CONFIG += c++11
QT       -= gui
QT += sql

#TARGET = MDVariableArchives
TEMPLATE = lib

DEFINES += MDVARIABLEARCHIVES_LIBRARY
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDVariableArchives.pri)
ProjectName = $${LibraryName}

include(../../pri/common.pri)

#逻辑、日志开关
include(../../pri/MDSwitchFactory.pri)
INCLUDEPATH += ../../include/

SOURCES += \
    MDArchivesVariable.cpp \
    MDArchivesVariableManager.cpp \
    MDVariableArchivesEngine.cpp \
    MDArchivesDataCmd.cpp \
    MDVariableArchivesFactory.cpp \
    MDVariableArchivesManager.cpp

HEADERS +=\
        mdvariablearchives_global.h \
    MDArchivesVariable.h \
    MDArchivesVariableManager.h \
    IMDVariableData.h \
    MDVariableArchivesEngine.h \
    MDArchivesDataCmd.h \
    MDVariableArchivesFactory.h \
    ../../include/MDVariableArchives/MDVariableArchivesFactory.h \
    ../../include/MDVariableArchives/mdvariablearchives_global.h \
    MDVariableArchivesManager.h \
    ../../include/MDVariableArchives/IMDVariableArchivesManager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
