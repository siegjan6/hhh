#-------------------------------------------------
#
# Project created by QtCreator 2015-08-31T14:44:14
#
#-------------------------------------------------

QT       -= gui

TARGET = MDLogHelper
TEMPLATE = lib

DEFINES += MDLOGHELPER_LIBRARY
#-------------------------------------------------
include(../../pri/MDCommonHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
#-------------------------------------------------
include(../../pri/MDLogHelper.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDLogMessage.cpp \
    MDLogMessageData.cpp

HEADERS += \
    ../../include/MDLogHelper/MDLogMessage.h \
    ../../include/MDLogHelper/MDLogHelper_Global.h \
    MDLogMessageData.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
