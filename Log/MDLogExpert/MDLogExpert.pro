#-------------------------------------------------
#
# Project created by QtCreator 2015-08-27T18:56:01
#
#-------------------------------------------------

QT       -= gui

TARGET = MDLogExpert
TEMPLATE = lib

DEFINES += MDLOGEXPERT_LIBRARY
#-------------------------------------------------
include(../../pri/MDCommonHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDLogHelper.pri)
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDHistoryExpert.pri)
include(../../pri/MDDataArchivesEngine.pri)
include(../../pri/MDDataSource.pri)
include(../../pri/MDDataGroup.pri)
#-------------------------------------------------
include(../../pri/MDLogExpert.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDLogExpert.cpp \
    MDLogExpertImp.cpp \
    MDLogRecord.cpp \
    MDLogRecordMgr.cpp \
    MDLogContacter.cpp

HEADERS +=\
    MDLogExpertImp.h \
    ../../include/MDLogExpert/MDLogExpert.h \
    ../../include/MDLogExpert/MDLogExpert_Global.h \
    MDLogRecord.h \
    MDLogRecordMgr.h \
    ../../include/MDLogExpert/MDLogContacter.h

