#-------------------------------------------------
#
# Project created by QtCreator 2015-07-09T15:35:31
#
#-------------------------------------------------

QT       -= gui
QT       += core

TARGET = MDDataSource
TEMPLATE = lib

DEFINES += MDDATASOURCE_LIBRARY

include(../../pri/MDDataSource.pri)
ProjectName =$${LibraryName}
include(../../pri/common.pri)

SOURCES += \
    MDDataSource.cpp \
    MDDataField.cpp \
    MDDataSourceManager.cpp \
    MDDatabaseParam.cpp \
    MDDataSourceFactory.cpp \
    MDDataTable.cpp

HEADERS +=\
    MDDataTable.h \
    MDDataField.h \
    MDDataSourceManager.h \
    MDDataSource.h \
    ../../include/MDDataSource/MDDataSourceFactory.h \
    ../../include/MDDataSource/mddatasource_global.h \
    MDDatabaseParam.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
