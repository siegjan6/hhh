#-------------------------------------------------
#
# Project created by QtCreator 2015-05-29T09:09:57
#
#-------------------------------------------------

QT       += core sql

TARGET = DBQueryEngine
TEMPLATE = lib

DEFINES += DBQUERYENGINE_LIBRARY

include(../../pri/MDDBQueryEngine.pri)
ProjectName =$${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDDBConnect.pri)
include(../../pri/MDCommandQueueEngine.pri)
include(../../pri/MDDBQueryConfigure.pri)
include(../../pri/MDSwitchFactory.pri)
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDSysLogExpert.pri)

INCLUDEPATH += ../../include


SOURCES += \
    DBQueryCommand.cpp \
    MDQueryDatabaseThread.cpp \
#    DBConnectionTemp.cpp \
    MDDBConnCache.cpp \
    DBQueryEngine.cpp \
    ViewMgr.cpp \
    DBSourcesMgr.cpp \
#    DatabaseParam.cpp \
    ViewVarMgr.cpp \
    MDDBQueryEngineFactory.cpp \
    utils.cpp

HEADERS += \
    DBQueryCommand.h \
    MDQueryDatabaseThread.h \
#    DBConnectionTemp.h \
    MDDBConnCache.h \
    DBQueryEngine.h \
    ViewMgr.h \
    DBSourcesMgr.h \
#    DatabaseParam.h \
    ViewVarMgr.h \
    utils.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
