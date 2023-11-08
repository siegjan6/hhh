#-------------------------------------------------
#
# Project created by QtCreator 2015-07-09T15:43:53
#
#-------------------------------------------------

QT       -= gui
QT       += core sql

TARGET = MDDataArchivesEngine
TEMPLATE = lib

DEFINES += MDDATAARCHIVESENGINE_LIBRARY

include(../../pri/MDDataArchivesEngine.pri)
ProjectName =$${LibraryName}
include(../../pri/common.pri)

include(../../pri/MDCommandQueueEngine.pri)
include(../../pri/MDSysLogExpert.pri)
include(../../pri/MDDBConnect.pri)

SOURCES += MDDataArchivesEngine.cpp \
    MDDataArchivesNotifyParam.cpp \
    MDDataArchivesCore.cpp \
    MDDatabaseService.cpp \
    MDNotifyParamCMD.cpp \
    DataArchivesNotifyParam.cpp \
    MDDatabaseCheckCMD.cpp \
    tempdbworker.cpp \
    timerthread.cpp \
    DBConnectionTemp.cpp

HEADERS +=\
    MDDataArchivesCore.h \
    ../../include/MDDataArchivesEngine/MDDataArchivesEngine.h \
    ../../include/MDDataArchivesEngine/mddataarchivesengine_global.h \
    ../../include/MDDataArchivesEngine/MDDataArchivesNotifyParam.h \
    MDDatabaseService.h \
    MDNotifyParamCMD.h \
    DataArchivesNotifyParam.h \
    MDDatabaseCheckCMD.h \
    tempdbworker.h \
    timerthread.h \
    MDDatabaseCommand.h \
    DBConnectionTemp.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
