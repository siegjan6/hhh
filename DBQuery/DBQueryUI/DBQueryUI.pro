#-------------------------------------------------
#
# Project created by meixl 2016-11-29T09:31:39
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBQueryUI
TEMPLATE = lib

DEFINES += DBQUERYUI_LIBRARY

include(../../pri/MDDBQueryUI.pri)
ProjectName =$${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDDataArchives.pri)
include(../../pri/MDDataSource.pri)
include(../../pri/MDDBQueryConfigure.pri)
include(../../pri/MDDBQueryEngine.pri)
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDEventHelper.pri)
include(../../pri/MDDBConnect.pri)

INCLUDEPATH += ../../include

SOURCES += main.cpp\
        mainwindow.cpp \
    ViewListDialog.cpp \
    DBQueryUIAPI.cpp \
    ViewFieldsDialog.cpp \
    SourceTreeDialog.cpp \
    RuntimeSourceDialog.cpp \
    NewDataSourceDialog.cpp \
    NewDataBaseDialog.cpp \
    NewFieldDialog.cpp \
    NewViewDialog.cpp \
    ViewVarEnumValueDialog.cpp \
    ViewVarTableDialog.cpp \
    NewViewVarDialog.cpp \
    MDDBQueryUI.cpp \
    DBQueryUI.cpp \
    ViewFieldTableDialog.cpp \
    ViewDBSourceDialog.cpp \
    ViewFilterTreeDialog.cpp \
    ViewFilterFieldDialog.cpp \
    ViewFilterAndOrNodeDialog.cpp \
    RuntimeFilterDialog.cpp \
    FilterTreeWidget.cpp \
    DBConnectionTemp.cpp

HEADERS  += mainwindow.h \
    ViewListDialog.h \
    ViewFieldsDialog.h \
    SourceTreeDialog.h \
    RuntimeSourceDialog.h \
    NewDataSourceDialog.h \
    NewDataBaseDialog.h \
    NewFieldDialog.h \
    NewViewDialog.h \
    ViewVarEnumValueDialog.h \
    ViewVarTableDialog.h \
    NewViewVarDialog.h\
    ../../include/MDDBQueryUI/MDDBQueryUI.h \
    DBQueryUI.h \
    ViewFieldTableDialog.h \
    ViewDBSourceDialog.h \
    ViewFilterTreeDialog.h \
    ViewFilterFieldDialog.h \
    ViewFilterAndOrNodeDialog.h \
    RuntimeFilterDialog.h \
    FilterTreeWidget.h \
    ui_ViewFilterTreeDialog.h \
    DBConnectionTemp.h

FORMS    += mainwindow.ui \
    ViewListDialog.ui \
    ViewFieldsDialog.ui \
    SourceTreeDialog.ui \
    RuntimeSourceDialog.ui \
    NewDataSourceDialog.ui \
    NewDataBaseDialog.ui \
    NewFieldDialog.ui \
    NewViewDialog.ui \
    ViewVarEnumValueDialog.ui \
    ViewVarTableDialog.ui \
    NewViewVarDialog.ui \
    ViewFieldTableDialog.ui \
    ViewDBSourceDialog.ui \
    ViewFilterFieldDialog.ui \
    ViewFilterAndOrNodeDialog.ui \
    RuntimeFilterDialog.ui

RESOURCES += \
    DBQueryUI.qrc
