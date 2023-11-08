#-------------------------------------------------
#
# Project created by QtCreator 2015-05-29T09:09:57
#
#-------------------------------------------------

QT       += core gui sql widgets

TARGET = DBQueryService
TEMPLATE = lib

DEFINES += DBQUERYSERVICE_LIBRARY
ProjectName = Mind.MDDataBase.MDDBQueryService

include(../../pri/MDDBQueryService.pri)
ProjectName =$${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDSysLogExpert.pri)
include(../../pri/MDDataSource.pri)
include(../../pri/MDDBConnect.pri)
include(../../pri/MDCommandQueueEngine.pri)

INCLUDEPATH += ../../include/MDDataArchivesInterface/


SOURCES += \
    MDDBQueryService.cpp \
    DBQueryCommand.cpp \
    MDDBQueryServiceFactory.cpp \
    MDQueryDatabaseThread.cpp \
    DBConnectionTemp.cpp \
    MDDBConnCache.cpp \
    dbqueryexpert.cpp \
    dbqueryexpertimp.cpp

HEADERS += \
    MDDBQueryService.h \
    DBQueryCommand.h \
    ../../include/MDDBQueryService/IMDDBQueryService.h \
    ../../include/MDDBQueryService/MDDBQueryServiceFactory.h \
    MDQueryDatabaseThread.h \
    DBConnectionTemp.h \
    MDDBConnCache.h \
    ../../include/MDDBQueryService/dbqueryexpert.h \
    ../../include/MDDBQueryService/mddbqueryexpert_global.h \
    dbqueryexpertimp.h \
    mddbqueryexpert_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
