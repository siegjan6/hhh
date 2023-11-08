#-------------------------------------------------
#
# Project created by QtCreator 2017-01-24T13:42:46
#
#-------------------------------------------------

QT       -= gui

TARGET = MDRecipeConfiger
TEMPLATE = lib

DEFINES += MDRECIPECONFIGER_LIBRARY

#-------------------------------------------------
include(../../pri/MDRecipeHelper.pri)
#-------------------------------------------------
include(../../pri/MDRecipeConfiger.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDRecipeConfiger.cpp \
    MDRecipeConfigerImp.cpp

HEADERS += MDRecipeConfigerImp.h \
    ../../include/MDRecipeConfiger/MDRecipeConfiger.h \
    ../../include/MDRecipeConfiger/MDRecipeConfiger_Global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
