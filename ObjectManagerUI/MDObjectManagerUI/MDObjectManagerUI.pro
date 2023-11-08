#-------------------------------------------------
#
# Project created by QtCreator 2015-08-04T10:11:09
#
#-------------------------------------------------

QT       += widgets

TARGET = MDObjectManagerUI
TEMPLATE = lib

DEFINES += MDOBJECTMANAGERUI_LIBRARY

#-------------------------------------------------
include(../../pri/MDBaseForm.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDObjBatchOperation.pri)
include(../../pri/MDMultiLanguage.pri)
#-------------------------------------------------
include(../../pri/MDObjectManagerUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += \
    MDObjectTreeModel.cpp \
    MDSelectObjectDialog.cpp \
    MDObjectTreeItemImp.cpp \
    MDObjectTreeItem.cpp \
    MDObjectTreeWindow.cpp \
    MDObjectTreeWindowParaImp.cpp \
    MDObjectTreeWindowPara.cpp \
    MDObjectPropertyDialog.cpp \
    MDObjectPropertyMainDialog.cpp \
    MDObjectPropertyDialogPara.cpp \
    MDObjectPropertyDialogParaImp.cpp \
    MDObjectPropertyMainDialogPara.cpp \
    MDObjectPropertyMainDialogParaImp.cpp

HEADERS +=\
    MDObjectTreeModel.h \
    ../../include/MDObjectManagerUI/MDObjectManagerUI_Global.h \
    MDSelectObjectDialog.h \
    MDObjectTreeItemImp.h \
    MDObjectTreeWindowParaImp.h \
    ../../include/MDObjectManagerUI/MDObjectTreeWindowPara.h \
    ../../include/MDObjectManagerUI/MDObjectTreeWindow.h \
    ../../include/MDObjectManagerUI/MDObjectTreeItem.h \
    ../../include/MDObjectManagerUI/MDObjectPropertyMainDialog.h \
    ../../include/MDObjectManagerUI/MDObjectPropertyDialog.h \
    MDStringResource.h \
    MDObjectPropertyDialogParaImp.h \
    ../../include/MDObjectManagerUI/MDObjectPropertyDialogPara.h \
    MDObjectPropertyMainDialogParaImp.h \
    ../../include/MDObjectManagerUI/MDObjectPropertyMainDialogPara.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    MDSelectObjectDialog.ui
