#-------------------------------------------------
#
# Project created by QtCreator 2016-11-17T20:04:32
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
DEFINES += MDOBJBATCHOPERATIONEXPERT_LIBRARY


include(../../pri/MDObjBatchOperation.pri)
ProjectName = $${LibraryName}

include(../../pri/common.pri)
include(../../pri/MDMultiLanguage.pri)

SOURCES +=\
    MDObjBatchOperationExpert.cpp \
    MDBatchModifyWidget.cpp \
    MDBatchAddWidget.cpp

HEADERS  += \
    ../../include/MDObjBatchOperation/MDObjBatchOperationExpert.h \
    MDBatchModifyWidget.h \
    MDBatchAddWidget.h

FORMS    += MDBatchAddWidget.ui \
    MDBatchModifyWidget.ui
