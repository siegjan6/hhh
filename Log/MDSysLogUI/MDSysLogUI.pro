#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T15:59:25
#
#-------------------------------------------------

QT       +=  core gui widgets

TARGET = MDSysLogUI
TEMPLATE = lib

DEFINES += MDSYSLOGUI_LIBRARY
#-------------------------------------------------
include(../../pri/MDSysLogExpert.pri)
include(../../pri/MDMultiLanguage.pri)
#-------------------------------------------------
include(../../pri/MDSysLogUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDSysLogUI.cpp \
    MDSysLogUIImp.cpp \
    MDSysLoggerDialog.cpp \
    MDSysLogTableModel.cpp

HEADERS += MDSysLogUIImp.h \
    MDSysLoggerDialog.h \
    MDSysLogTableModel.h \
    ../../include/MDSysLogUI/MDSysLogUI_Global.h \
    ../../include/MDSysLogUI/MDSysLogUI.h \
    MDStringResource.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    MDSysLoggerDialog.ui
