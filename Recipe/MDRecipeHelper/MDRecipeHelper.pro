#-------------------------------------------------
#
# Project created by QtCreator 2016-01-06T14:46:53
#
#-------------------------------------------------

QT       -= gui

TARGET = MDRecipeHelper
TEMPLATE = lib

DEFINES += MDRECIPEHELPER_LIBRARY
#-------------------------------------------------
include(../../pri/MDActionIDHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
#-------------------------------------------------
include(../../pri/MDRecipeHelper.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDRecipe.cpp \
    MDRecipeData.cpp \
    MDRecipePropertyData.cpp \
    MDRecipeProperty_Config.cpp \
    MDRecipeProperty_Runtime.cpp \
    MDRecipeFileData.cpp \
    MDRecipeFile.cpp

HEADERS += MDRecipeData.h \
    ../../include/MDRecipeHelper/MDRecipe.h \
    ../../include/MDRecipeHelper/MDRecipeHelper_Global.h \
    MDRecipePropertyData.h \
    ../../include/MDRecipeHelper/MDRecipeProperty_Config.h \
    ../../include/MDRecipeHelper/MDRecipeProperty_Runtime.h \
    MDRecipeFileData.h \
    ../../include/MDRecipeHelper/MDRecipeFile.h

