#-------------------------------------------------
#
# Project created by QtCreator 2015-12-15T10:52:40
#
#-------------------------------------------------

QT       += widgets sql

TARGET = MDDataQueryUI
TEMPLATE = lib

DEFINES += MDDATAQUERYUI_LIBRARY


include(../../pri/MDDataQueryUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)


include(src/dataQueryUI.pri)

