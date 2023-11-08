#-------------------------------------------------
#
# Project created by QtCreator 2015-08-18T20:58:19
#
#-------------------------------------------------

QT       += widgets

TARGET = MDAlarmUI
TEMPLATE = lib

DEFINES += MDALARMUI_LIBRARY
#-------------------------------------------------
include(../../pri/MDAlarmHelper.pri)
include(../../pri/MDAlarmLevelConfiger.pri)
include(../../pri/MDAlarmConditionConfiger.pri)
include(../../pri/MDAlarmConfiger.pri)
include(../../pri/MDAlarmLevelUI.pri)
include(../../pri/MDAlarmConditionUI.pri)
#-------------------------------------------------
include(../../pri/MDAlarmUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDAlarmUI.cpp \
    MDAlarmUIImp.cpp \
    MDAlarmConditionUIContacterImp.cpp \
    MDAlarmParaForm.cpp

HEADERS += MDAlarmUIImp.h \
    ../../include/MDAlarmUI/MDAlarmUI.h \
    ../../include/MDAlarmUI/MDAlarmUI_Global.h \
    MDAlarmConditionUIContacterImp.h \
    ../../include/MDAlarmUI/MDAlarmParaForm.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    MDAlarmParaForm.ui
