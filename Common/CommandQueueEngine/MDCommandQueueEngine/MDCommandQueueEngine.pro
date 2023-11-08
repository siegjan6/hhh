#-------------------------------------------------
#
# Project created by QtCreator 2015-02-04T17:28:52
#
#-------------------------------------------------

QT       -= gui
QT      += core

TARGET = MDCommandQueueEngine
TEMPLATE = lib

DEFINES += MDCOMMANDQUEUEENGINE_LIBRARY

include(../../../pri/MDCommandQueueEngine.pri)
ProjectName = $${LibraryName}
include(../../../pri/common3.pri)

SOURCES += \
    MDCommandData.cpp \
    MDCommandQueueEngine.cpp \
    MDCommandQueueEngineHelper.cpp \
    MDCommandQueueEngineImp.cpp \
    MDCommandQueueEngineThread.cpp \
    MDProcessedCommandQueueManager.cpp \
    MDProcessingCommandQueueManager.cpp \
    NotificationHandler.cpp \
    MDCommandQueueEngineMgrImp.cpp \
    MDCommandQueueEngineMgr.cpp

HEADERS +=\
    MDCommandData.h \
    MDCommandQueueEngineHelper.h \
    MDCommandQueueEngineImp.h \
    MDCommandQueueEngineThread.h \
    ../../../include/MDCommandQueueEngine/MDCommandQueueEngine.h \
    ../../../include/MDCommandQueueEngine/MDCommandQueueEngine_Global.h \
    MDProcessedCommandQueueManager.h \
    MDProcessingCommandQueueManager.h \
    NotificationHandler.h \
    MDCommandQueueEngineMgrImp.h \
    ../../../include/MDCommandQueueEngine/MDCommandQueueEngineMgr.h

OTHER_FILES += \
    Overview_MDCommandQueueEngine.txt
