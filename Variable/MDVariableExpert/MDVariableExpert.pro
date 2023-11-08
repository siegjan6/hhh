#-------------------------------------------------
#
# Project created by QtCreator 2015-03-17T10:48:15
#
#-------------------------------------------------

QT       -= gui
QT       += xml

TARGET = MDVariableExpert
TEMPLATE = lib

DEFINES += MDVARIABLEEXPERT_LIBRARY
#-------------------------------------------------
include(../../pri/MDActionIDHelper.pri)
include(../../pri/MDActionHelper.pri)
# ignore MDRTDBManager replace with MDUaExpert
#include(../../pri/MDRTDBManager.pri)
include(../../pri/MDUaExpert.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDCommandQueueEngine.pri)
include(../../pri/MDSwitchFactory.pri)
#-------------------------------------------------
include(../../pri/MDVariableExpert.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDVariableExpert.cpp \
    MDVariableExpertImp.cpp \
    MDVariableContacter.cpp \
    CommandEngineForCheckVariableSP.cpp

HEADERS +=  MDVariableExpertImp.h \
    ../../include/MDVariableExpert/MDVariableExpert.h \
    ../../include/MDVariableExpert/MDVariableExpert_Global.h \
    ../../include/MDVariableExpert/MDVariableContacter.h \
    CommandEngineForCheckVariableSP.h
