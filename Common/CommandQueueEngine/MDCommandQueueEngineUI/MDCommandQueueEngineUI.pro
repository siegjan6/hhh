#-------------------------------------------------
#
# Project created by QtCreator 2016-02-24T13:33:30
#
#-------------------------------------------------

QT       += widgets

TARGET = MDCommandQueueEngineUI
TEMPLATE = lib

DEFINES += MDCOMMANDQUEUEENGINEUI_LIBRARY

#-------------------------------------------------
INCLUDEPATH += ../../../include/interface/Common
include(../../../pri/MDCommandQueueEngine.pri)
include(../../../pri/MDMultiLanguage.pri)
#-------------------------------------------------
include(../../../pri/MDCommandQueueEngineUI.pri)
ProjectName = $${LibraryName}
include(../../../pri/common3.pri)

SOURCES += MDCommandQueueEngineUI.cpp\
    MDEngineMgrDialog.cpp \
    MDEngineGeneralDialog.cpp \
    MDCommandQueueEngineUIImp.cpp

HEADERS += \
    ../../../include/MDCommandQueueEngineUI/MDCommandQueueEngineUI.h \
    ../../../include/MDCommandQueueEngineUI/MDCommandQueueEngineUI_Global.h \
    MDEngineMgrDialog.h \
    MDEngineGeneralDialog.h \
    MDCommandQueueEngineUIImp.h

FORMS += \
    MDEngineGeneralDialog.ui
