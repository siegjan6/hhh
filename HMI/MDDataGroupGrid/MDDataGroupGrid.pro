#-------------------------------------------------
#
# Project created by QtCreator 2015-10-28T10:41:36
#
#-------------------------------------------------
QT       += core gui widgets sql xml

TEMPLATE = lib

DEFINES += MDDATAGROUPGRID_LIBRARY

include (../../pri/MDDataGroupGrid.pri)

ProjectName = $${LibraryName}

include(../../pri/common.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDUtility.pri)
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDDataGroup.pri)
include(../../pri/MDDBQueryService.pri)
include(../../pri/MDDataSelectUI.pri)
include(../../pri/MDDataQueryUI.pri)
include(../../pri/MDDataSource.pri)
include(../../pri/MDDBQueryConfigure.pri)
include(../../pri/MDDBQueryEngine.pri)
include(../../pri/MDDBQueryUI.pri)

INCLUDEPATH += \
    ConfigForm \
    Data

HEADERS += \
    Data/DataGroupData.h \
    Data/DataGroupModel.h \
    Data/DataGroupWidget.h \
    ConfigForm/PropertyDialog.h \
    ConfigForm/CommonForm.h \
    ConfigForm/AppearanceForm.h \
    ../../include/MDDataGroupGrid/MDDataGroupGrid.h \
    ../../include/MDDataGroupGrid/MDGroupGridDefine.h \
    ../../include/MDFilterTreePlugin/MDFilterTreePlugin.h \
    ../../include/MDDataQueryUI/MDDataQueryUI.h

SOURCES += \
    MDDataGroupGrid.cpp \
    Data/DataGroupModel.cpp \
    Data/DataGroupWidget.cpp \
    ConfigForm/PropertyDialog.cpp \
    ConfigForm/CommonForm.cpp \
    ConfigForm/AppearanceForm.cpp \
    Data/DataGroupData.cpp

FORMS += \
    Data/DataGroupWidget.ui \
    ConfigForm/PropertyDialog.ui \
    ConfigForm/CommonForm.ui \
    ConfigForm/AppearanceForm.ui

