#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T15:16:44
#
#-------------------------------------------------

QT       -= gui
QT      += core

TARGET = MDEventExpert
TEMPLATE = lib

DEFINES += MDEVENTEXPERT_LIBRARY
#-------------------------------------------------
include(../../pri/MDEventConfiger.pri)
include(../../pri/MDCommonHelper.pri)
include(../../pri/MDEventHelper.pri)
include(../../pri/MDCommandQueueEngine.pri)
include(../../pri/MDExpression.pri)
include(../../pri/MDSysLogExpert.pri)
include(../../pri/MDLogHelper.pri)
include(../../pri/MDSwitchFactory.pri)
include(../../pri/MDActionHelper.pri)
#-------------------------------------------------
include(../../pri/MDEventExpert.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += \
    MDEventClassFactory.cpp \
    MDEventContacter.cpp \
    MDEventEngine.cpp \
    MDEventExpert.cpp \
    MDEventExpertImp.cpp \
    MDEventHandlerEx.cpp \
    MDObjectWithEvent.cpp

HEADERS += \
    MDEventClassFactory.h \
    MDEventEngine.h \
    MDEventExpertImp.h \
    MDEventHandlerEx.h \
    MDObjectWithEvent.h \
    ../../include/MDEventExpert/MDEventContacter.h \
    ../../include/MDEventExpert/MDEventExpert.h \
    ../../include/MDEventExpert/MDEventExpert_Global.h
