#-------------------------------------------------
#
# Project created by QtCreator 2015-04-26T09:26:28
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = DBStudio
TEMPLATE = lib

DEFINES += MDDATAARCHIVESUI_LIBRARY
include(../../pri/MDDataArchivesUI.pri)
ProjectName = Mind.DataArchives.MDDataArchivesUI
include(../../pri/common.pri)

include(../../pri/MDDataSource.pri)
include(../../pri/MDDataGroup.pri)
include(../../pri/MDDBConnect.pri)
include(../../pri/MDUtility.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDRTDBManagerUI.pri)
include(../../pri/MDSysLogExpert.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDEventHelper.pri)


SOURCES += \
    DBTableDialog.cpp \
    DBstudioFramework.cpp \
    NewDBsourceDialog.cpp \
    DbTableFieldDialog.cpp \
    DBSourceParamWidget.cpp \
    MDDataSrcTabWidget.cpp \
    MDDataGroupTableWidget.cpp \
    MDDataArchivesUI.cpp \
    DataArchivesUI.cpp \
    DataGroupFramework.cpp \
    DataArchivesTabModel.cpp \
    MDNewDataGroup.cpp \
    MDDataArchivesTableWidget.cpp

HEADERS += \
    DBTableDialog.h \
    DBstudioFramework.h \
    NewDBsourceDialog.h \
    DbTableFieldDialog.h \
    DBSourceParamWidget.h \
    MDDataSrcTabWidget.h \
    MDDataGroupTableWidget.h \
    ../../include/MDDataArchivesUI/MDDataArchivesUI.h \
    DataArchivesUI.h \
    DataGroupFramework.h \
    DataArchivesTabModel.h \
    MDNewDataGroup.h \
    MDDataArchivesTableWidget.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    DBStudioResource.qrc

FORMS += \
    mainwindow.ui \
    newdbsourcedialog.ui \
    dbtablefielddialog.ui
