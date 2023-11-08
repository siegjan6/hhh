#-------------------------------------------------
#
# Project created by QtCreator 2015-06-29T13:41:36
#
#-------------------------------------------------
QT       += core gui widgets

TEMPLATE = lib

DEFINES += MDALARMGRID_LIBRARY

include(../../pri/MDAlarmGrid.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDDataGroup.pri)
include(../../pri/MDDBQueryService.pri)
include (../../pri/MDDataGroupGrid.pri)
include(../../pri/MDSQLDataBaseProxy.pri)

SOURCES += \
    MDAlarmGrid.cpp


HEADERS  += \
    ../../include/MDAlarmGrid/MDAlarmGrid.h




