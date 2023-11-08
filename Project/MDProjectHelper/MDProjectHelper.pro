#-------------------------------------------------
#
# Project created by QtCreator 2015-09-04T20:34:48
#
#-------------------------------------------------

QT       -= gui

TARGET = MDProjectHelper
TEMPLATE = lib

DEFINES += MDPROJECTHELPER_LIBRARY
#-------------------------------------------------
include(../../pri/MDSecurityPolicyHelper.pri)
#-------------------------------------------------
include(../../pri/MDProjectHelper.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDProjectPara.cpp \
    MDProjectParaData.cpp

HEADERS += MDProjectParaData.h \
    ../../include/MDProjectHelper/MDProjectPara.h \
    ../../include/MDProjectHelper/MDProjectHelper_Global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
