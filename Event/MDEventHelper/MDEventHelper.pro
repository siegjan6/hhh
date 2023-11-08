#-------------------------------------------------
#
# Project created by QtCreator 2015-02-10T14:47:57
#
#-------------------------------------------------

QT       -= gui

TARGET = MDEventHelper
TEMPLATE = lib

DEFINES += MDEVENTHELPER_LIBRARY
#-------------------------------------------------
# ignore MDRTDBManager replace with MDUaExpert
#include(../../pri/MDRTDBManager.pri)
include(../../pri/MDUaExpert.pri)
include(../../pri/MDVariableArchives.pri)
include(../../pri/MDExpression.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
#-------------------------------------------------
include(../../pri/MDEventHelper.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += \
    MDEvent.cpp \
    MDEventData.cpp \
    MDEventHandler.cpp \
    MDEventHelper.cpp \
    MDEventHelperImp.cpp \
    MDEventProperty_Config.cpp \
    MDEventProperty_Runtime.cpp \
    MDEventPropertyData.cpp \
    MDRawEvent.cpp \
    EPC_VariableValueChangedInRange.cpp \
    EPC_VariableQualityChanged.cpp \
    EPC_VariableValueWrite.cpp \
    EPC_TimeCycle.cpp \
    EPC.cpp \
    EPC_Combined.cpp

HEADERS += \
    MDEventData.h \
    MDEventHelperImp.h \
    MDEventPropertyData.h \
    ../../include/MDEventHelper/MDEvent.h \
    ../../include/MDEventHelper/MDEventConstant.h \
    ../../include/MDEventHelper/MDEventHandler.h \
    ../../include/MDEventHelper/MDEventHelper.h \
    ../../include/MDEventHelper/MDEventHelper_Global.h \
    ../../include/MDEventHelper/MDEventProperty_Config.h \
    ../../include/MDEventHelper/MDEventProperty_Runtime.h \
    ../../include/MDEventHelper/MDRawEvent.h \
    ../../include/MDEventHelper/EPC_VariableValueWrite.h \
    ../../include/MDEventHelper/EPC_VariableValueChangedInRange.h \
    ../../include/MDEventHelper/EPC_VariableQualityChanged.h \
    ../../include/MDEventHelper/EPC_TimeCycle.h \
    ../../include/MDEventHelper/EPC.h \
    ../../include/MDEventHelper/EPC_Combined.h \
    MDStringResource.h



