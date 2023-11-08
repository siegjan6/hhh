#-------------------------------------------------
#
# Project created by QtCreator 2015-08-19T14:19:56
#
#-------------------------------------------------

QT       += widgets

TARGET = MDAlarmLevelUI
TEMPLATE = lib

DEFINES += MDALARMLEVELUI_LIBRARY
#-------------------------------------------------
include(../../pri/MDAlarmHelper.pri)
include(../../pri/MDEventHelper.pri)
include(../../pri/MDAlarmLevelConfiger.pri)
include(../../pri/MDAlarmConditionConfiger.pri)
include(../../pri/MDAlarmConfiger.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDObjectManagerUI.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDActionSPUI.pri)
include(../../pri/MDObjBatchOperation.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDMultiLanguage.pri)
#-------------------------------------------------
include(../../pri/MDAlarmLevelUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDAlarmLevelUI.cpp \
    MDAlarmLevelUIImp.cpp \
    MDAlarmLevelTreeWindow.cpp \
    MDAlarmLevelMainDialog.cpp \
    MDAlarmLevelGeneralDialog.cpp \
    MDAlarmLevelColorDialog.cpp \
    MDAlarmLevelForBrowser.cpp

HEADERS += MDAlarmLevelUIImp.h \
    MDAlarmLevelTreeWindow.h \
    MDAlarmLevelMainDialog.h \
    MDAlarmLevelGeneralDialog.h \
    ../../include/MDAlarmLevelUI/MDAlarmLevelUI.h \
    ../../include/MDAlarmLevelUI/MDAlarmLevelUI_Global.h \
    MDAlarmLevelColorDialog.h \
    MDStringResource.h \
    MDAlarmLevelForBrowser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    MDAlarmLevelGeneralDialog.ui \
    MDAlarmLevelColorDialog.ui
