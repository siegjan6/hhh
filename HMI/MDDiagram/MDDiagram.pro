#-------------------------------------------------
#
# Project created by QtCreator 2015-11-10T13:26:26
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport

TEMPLATE = lib

DEFINES += MDDIAGRAM_LIBRARY

include(../../pri/MDDiagram.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDUtility.pri)
include(../../pri/MDMultiLanguage.pri)

INCLUDEPATH += \
    Config\
    Data\
    Draw

SOURCES += \
    Draw/DrawWidget.cpp \
    Data/DrawData.cpp \
    Draw/DrawAxis.cpp \
    Draw/DrawHistogram.cpp \
    Draw/DrawVernier.cpp \
    Draw/DrawHistogram3D.cpp \
    MDDiagram.cpp \
    Config/DesignForm.cpp \
    Config/PrintForm.cpp

HEADERS  += \
    Draw/DrawWidget.h \
    Data/DrawData.h \
    Draw/DrawAxis.h \
    Draw/DrawHistogram.h \
    Draw/DrawVernier.h \
    Draw/DrawHistogram3D.h \
    MDDiagram.h \
    ../../include/MDDiagram/MDDiagram.h \
    Config/DesignForm.h \
    Config/PrintForm.h

FORMS    += \
    Draw/DrawWidget.ui \
    Draw/DrawVernier.ui \
    Config/DesignForm.ui \
    Config/PrintForm.ui
