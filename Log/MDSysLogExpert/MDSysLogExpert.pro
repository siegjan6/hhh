#-------------------------------------------------
#
# Project created by QtCreator 2015-04-07T09:26:30
#
#-------------------------------------------------

QT       -= gui
QT      += core

TARGET = MDSysLogExpert
TEMPLATE = lib

DEFINES += MDSYSLOGEXPERT_LIBRARY
#-------------------------------------------------
include(../../pri/MDCommandQueueEngine.pri)
#-------------------------------------------------
include(log4qt/log4qt.pri)

include(../../pri/MDSysLogExpert.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDSysLogExpert.cpp \
    MDSysLogData.cpp \
    MDSysLogDataImp.cpp \
    MDSysLogExpertImp.cpp \
    log4qt_for_MindSCADA/layout_MindSCADA.cpp \
    log4qt_for_MindSCADA/listappender_MindSCADA.cpp \
    log4qt_for_MindSCADA/rollingfileappender_MindSCADA.cpp \
    MDSysLoggerImp.cpp \
    MDSysLogger.cpp \
    MDSysLogEngine.cpp

HEADERS +=\
    ../../include/MDSysLogExpert/MDSysLogData.h \
    ../../include/MDSysLogExpert/MDSysLogExpert.h \
    ../../include/MDSysLogExpert/MDSysLogExpert_Global.h \
    MDSysLogDataImp.h \
    MDSysLogExpertImp.h \
    log4qt_for_MindSCADA/layout_MindSCADA.h \
    log4qt_for_MindSCADA/listappender_MindSCADA.h \
    log4qt_for_MindSCADA/rollingfileappender_MindSCADA.h \
    MDSysLoggerImp.h \
    ../../include/MDSysLogExpert/MDSysLogger.h \
    MDSysLogEngine.h

