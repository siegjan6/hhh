#-------------------------------------------------
#
# Project created by QtCreator 2015-08-04T11:19:47
#
#-------------------------------------------------
QT       += core gui widgets sql

TEMPLATE = lib

DEFINES += MDDATAARCHIVESWIDGET_LIBRARY

include(../../pri/MDDataArchivesWidget.pri)

ProjectName = $${LibraryName}

include(../../pri/common1.pri)
include(../../pri/MDDataGroup.pri)
include(../../pri/MDDBQueryService.pri)
include(../../pri/MDDataSelectUI.pri)
include (../../pri/MDDataGroupGrid.pri)
include(../../pri/MDSQLDataBaseProxy.pri)

SOURCES += \
    MDDataArchivesWidget.cpp


HEADERS  += \
    ../../include/MDDataArchivesWidget/MDDataArchivesWidget.h \
