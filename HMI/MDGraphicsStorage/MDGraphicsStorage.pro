#-------------------------------------------------
#
# Project created by QtCreator 2015-07-21T13:28:45
#
#-------------------------------------------------


QT       += core gui widgets

TEMPLATE = lib

DEFINES += MDGRAPHICSSTORAGE_LIBRARY

INCLUDEPATH += ../../include/interface/Studio \

include(../../pri/MDGraphicsStorage.pri)

ProjectName = $${LibraryName}

include(../../pri/common.pri)

include(../../pri/MDHMIForm.pri)
include(../../pri/MDDrawObj.pri)
include(../../pri/MDMultiLanguage.pri)


SOURCES +=\
    GraphicsDialog.cpp \
    ObjectWidget.cpp \
    ObjectManager.cpp \
    MDGraphicsStorage.cpp \
    SelectPathDialog.cpp \
    StorageFileNode.cpp

HEADERS  += \
    GraphicsDialog.h \
    ObjectWidget.h \
    ObjectManager.h \
    SelectPathDialog.h \
    ../../include/interface/HMI/IGraphicsStorage.h \
    ../../include/MDGraphicsStorage/MDGraphicsStorage.h \
    StorageFileNode.h

FORMS    += \
    GraphicsDialog.ui \
    SelectPathDialog.ui

RESOURCES += \
    rs_mdGraphicsStorage.qrc
