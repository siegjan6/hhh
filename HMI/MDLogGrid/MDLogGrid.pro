#-------------------------------------------------
#
# Project created by QtCreator 2015-06-29T13:41:36
#
#-------------------------------------------------
QT       += core gui widgets

TEMPLATE = lib

DEFINES += MDLOGGRID_LIBRARY

include(../../pri/MDLogGrid.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDDataGroup.pri)
include(../../pri/MDDBQueryService.pri)
include (../../pri/MDDataGroupGrid.pri)

SOURCES += \
    MDLogGrid.cpp

HEADERS  += \
    ../../include/MDLogGrid/MDLogGrid.h \
