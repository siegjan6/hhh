#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T09:49:23
#
#-------------------------------------------------

QT       += widgets sql
TEMPLATE = lib

DEFINES += MDCURVE_LIBRARY

include(../../pri/MDCurve.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)
include(./curve.pri)

