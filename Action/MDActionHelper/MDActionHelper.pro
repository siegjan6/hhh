#-------------------------------------------------
#
# Project created by QtCreator 2015-02-10T15:10:57
#
#-------------------------------------------------

QT       -= gui

TARGET = MDActionHelper
TEMPLATE = lib

DEFINES += MDACTIONHELPER_LIBRARY

#-------------------------------------------------
include(../../pri/MDActionIDHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
#-------------------------------------------------
include(../../pri/MDActionHelper.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += \
    MDAction.cpp \
    MDActionData.cpp \
    MDActionProperty_Config.cpp \
    MDActionProperty_Runtime.cpp \
    MDActionPropertyData.cpp \
    APC_VariableWriteValue.cpp \
    APC_PlayTTS.cpp \
    APC_WindowOperation.cpp \
    APC.cpp \

HEADERS +=\
    MDActionData.h \
    MDActionPropertyData.h \
    ../../include/MDActionHelper/MDAction.h \
    ../../include/MDActionHelper/MDActionConstant.h \
    ../../include/MDActionHelper/MDActionHelper_Global.h \
    ../../include/MDActionHelper/MDActionProperty_Config.h \
    ../../include/MDActionHelper/MDActionProperty_Runtime.h \
    ../../include/MDActionHelper/APC_WindowOperation.h \
    ../../include/MDActionHelper/APC_VariableWriteValue.h \
    ../../include/MDActionHelper/APC_PlayTTS.h \
    ../../include/MDActionHelper/APC.h


