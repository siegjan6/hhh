#-------------------------------------------------
#
# Project created by QtCreator 2015-07-09T14:13:04
#
#-------------------------------------------------

QT       -= gui
QT       += core

TARGET = MDDataArchives
TEMPLATE = lib

DEFINES += MDDATAARCHIVES_LIBRARY

include(../../pri/MDDataArchives.pri)
ProjectName =$${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDDataGroup.pri)
include(../../pri/MDDataSource.pri)

SOURCES += \
    MDDataArchivesFactory.cpp \
    MDDataArchives.cpp

HEADERS += MDDataArchives.h\
    ../../include/MDDataArchives/MDDataArchivesFactory.h \
    ../../include/MDDataArchives/MDDataArchives_global.h

INCLUDEPATH +=
    ../../include/MDDataArchivesInterface

unix {
    target.path = /usr/lib
    INSTALLS += target
}
