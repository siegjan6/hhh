#-------------------------------------------------
#
# Project created by QtCreator 2016-12-29T15:18:40
#
#-------------------------------------------------

QT       += widgets

TARGET = MDRecipeUI
TEMPLATE = lib

DEFINES += MDRECIPEUI_LIBRARY
#-------------------------------------------------
include(../../pri/MDRecipeHelper.pri)
include(../../pri/MDEventHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDRecipeConfiger.pri)
include(../../pri/MDRecipeExpert.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDObjectManagerUI.pri)
include(../../pri/MDActionSPUI.pri)
include(../../pri/MDRTDBManagerUI.pri)
#-------------------------------------------------
include(../../pri/MDRecipeUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDRecipeUI.cpp \
    MDRecipeUIImp.cpp \
    MDDownloadDialog.cpp \
    MDRecipeFileNode.cpp \
    MDRecipeItemsDialog.cpp \
    MDRecipeDownloadParaDialog.cpp \
    MDRecipeMainDialog.cpp \
    MDRecipeSPListDialogEx.cpp

HEADERS += ../../include/MDRecipeUI/MDRecipeUI.h \
    MDRecipeUIImp.h \
    MDDownloadDialog.h \
    ../../include/MDRecipeUI/MDRecipeUI_Global.h \
    MDRecipeFileNode.h \
    MDRecipeItemsDialog.h \
    MDRecipeDownloadParaDialog.h \
    MDRecipeMainDialog.h \
    MDRecipeSPListDialogEx.h \
    MDStringResource.h

FORMS += \
    MDDownloadDialog.ui \
    MDRecipeItemsDialog.ui \
    MDRecipeDownloadParaDialog.ui

