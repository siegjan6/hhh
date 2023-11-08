#-------------------------------------------------
#
# Project created by QtCreator 2016-01-06T14:35:30
#
#-------------------------------------------------

QT       -= gui

TARGET = MDRecipeExpert
TEMPLATE = lib

DEFINES += MDRECIPEEXPERT_LIBRARY
#-------------------------------------------------
include(../../pri/MDRecipeHelper.pri)
include(../../pri/MDRecipeConfiger.pri)
include(../../pri/MDSwitchFactory.pri)
#-------------------------------------------------
include(../../pri/MDRecipeExpert.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDRecipeExpert.cpp \
    MDRecipeExpertImp.cpp \
    MDRecipeContacter.cpp \
    MDDownloadExpert.cpp \
    MDDownloadWorker.cpp \
    MDDownloadTaskQueue.cpp \
    MDDownloadTask.cpp

HEADERS += MDRecipeExpertImp.h \
    ../../include/MDRecipeExpert/MDRecipeExpert.h \
    ../../include/MDRecipeExpert/MDRecipeExpert_Global.h \
    ../../include/MDRecipeExpert/MDRecipeContacter.h \
    MDDownloadExpert.h \
    MDDownloadWorker.h \
    MDDownloadTaskQueue.h \
    MDDownloadTask.h

