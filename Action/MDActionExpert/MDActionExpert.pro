#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T15:31:20
#
#-------------------------------------------------

QT       -= gui

TARGET = MDActionExpert
TEMPLATE = lib

DEFINES += MDACTIONEXPERT_LIBRARY

#-------------------------------------------------
include(../../pri/MDCommandQueueEngine.pri)
include(../../pri/MDActionHelper.pri)
include(../../pri/MDActionConfiger.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDLogHelper.pri)
include(../../pri/MDSwitchFactory.pri)
include(../../pri/MDExpression.pri)
#include(../../pri/MDUaExpert.pri)
#-------------------------------------------------
include(../../pri/MDActionExpert.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += \
    MDActionContacter.cpp \
    MDActionEngine.cpp \
    MDActionExpert.cpp \
    MDActionExpertImp.cpp \
    MDDelayActionMgr.cpp \
    CommandEngineForCheckActionSP.cpp

HEADERS += \
    MDActionEngine.h \
    MDActionExpertImp.h \
    ../../include/MDActionExpert/MDActionContacter.h \
    ../../include/MDActionExpert/MDActionExpert.h \
    ../../include/MDActionExpert/MDActionExpert_Global.h \
    MDDelayActionMgr.h \
    CommandEngineForCheckActionSP.h
