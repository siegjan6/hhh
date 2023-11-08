#-------------------------------------------------
#
# Project created by QtCreator 2015-07-28T15:13:25
#
#-------------------------------------------------

QT       -= gui
QT += sql
TARGET = MDSQLDataBaseProxy
TEMPLATE = lib
DEFINES += MDSQLDATABASEPROXY_LIBRARY


include(../../pri/MDSQLDataBaseProxy.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDSQLDataBaseProxy.cpp \
    MDFields.cpp \
    MDRowData.cpp \
    MDDatabaseConnection.cpp \
    MDDatabaseConnectionPrivate.cpp \
    MDDatabaseRecordset.cpp \
    MDDatabaseRecordsetPrivate.cpp

HEADERS += MDSQLDataBaseProxy.h\
    MDFields.h \
    MDRowData.h \
    MDDatabaseConnectionPrivate.h \
    ../../include/MDSQLDataBaseProxy/MDDatabaseConnection.h \
    ../../include/MDSQLDataBaseProxy/mdsqldatabaseproxy_global.h \
    ../../include/MDSQLDataBaseProxy/MDDatabaseRecordset.h \
    MDDatabaseRecordsetPrivate.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
