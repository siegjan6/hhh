#-------------------------------------------------
#
# Project created by QtCreator 2014-11-04T10:34:33
#
#-------------------------------------------------

QT       += core gui widgets

TEMPLATE = lib

DEFINES += MDPROPERTYBROWSER_LIBRARY

include(../../pri/MDPropertyBrowser.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDMultiLanguage.pri)

include(src/qtpropertybrowser.pri)



