#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T11:16:04
#
#-------------------------------------------------

QT       += gui sql

TARGET = MDHistoryExpert
TEMPLATE = lib

DEFINES += MDHISTORYEXPERT_LIBRARY
#-------------------------------------------------
include(../../pri/MDActionIDHelper.pri)
include(../../pri/MDActionHelper.pri)
include(../../pri/MDDataArchivesEngine.pri)
include(../../pri/MDDBQueryService.pri)
include(../../pri/MDSysLogExpert.pri)
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDDataSource.pri)
include(../../pri/MDDataGroup.pri)
include(../../pri/MDDBQueryEngine.pri)
# ignore MDRTDBManager replace with MDUaExpert
#include(../../pri/MDRTDBManager.pri)
include(../../pri/MDUaExpert.pri)
#-------------------------------------------------
include(../../pri/MDHistoryExpert.pri)
ProjectName = $${LibraryName}
include(../../pri/common1.pri)

SOURCES += MDHistoryExpert.cpp \
    MDHistoryExpertImp.cpp \
    MDHistoryContacter.cpp \
    MDQueryTask.cpp \
    MDQueryTaskMgr.cpp \
    MDQueryTaskMgrImp.cpp

HEADERS +=\
    MDHistoryExpertImp.h \
    ../../include/MDHistoryExpert/MDHistoryContacter.h \
    ../../include/MDHistoryExpert/MDHistoryExpert.h \
    ../../include/MDHistoryExpert/MDHistoryExpert_Global.h \
    MDQueryTask.h \
    MDQueryTaskMgrImp.h \
    ../../include/MDHistoryExpert/MDQueryTaskMgr.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
