#-------------------------------------------------
#
# Project created by QtCreator 2015-06-14T10:04:59
#
#-------------------------------------------------

QT       += core widgets

TARGET = MDEventActionUI
TEMPLATE = lib

DEFINES += MDEVENTACTIONUI_LIBRARY
#-------------------------------------------------
include(../../pri/MDEventHelper.pri)
include(../../pri/MDEventUI.pri)
include(../../pri/MDActionUI.pri)
include(../../pri/MDProjectConfiger.pri)
#-------------------------------------------------
include(../../pri/MDEventActionUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDEventActionUI.cpp \
    MDEventUIContacterImp.cpp \
    MDEventActionUIImp.cpp \
    MDActionUIContacterImp.cpp \
    MDSelectHistoryGroupDialog.cpp

HEADERS += \
    ../../include/MDEventActionUI/MDEventActionUI.h \
    ../../include/MDEventActionUI/MDEventActionUI_Global.h \
    MDEventUIContacterImp.h \
    MDEventActionUIImp.h \
    MDActionUIContacterImp.h \
    MDSelectHistoryGroupDialog.h

FORMS += \
    MDSelectHistoryGroupDialog.ui

