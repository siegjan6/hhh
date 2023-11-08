#-------------------------------------------------
#
# Project created by QtCreator 2016-11-07T17:16:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBQueryUITestor
TEMPLATE = app

include(../../pri/MDDataArchives.pri)
include(../../pri/MDDataSource.pri)
include(../../pri/MDDBQueryConfigure.pri)
include(../../pri/MDDBQueryEngine.pri)

DESTDIR = ../../bin/debug

SOURCES += main.cpp\
        TestorDialog.cpp \
    RuntimeFilterDialog.cpp \
    temp.cpp

HEADERS  += TestorDialog.h \
    RuntimeFilterDialog.h

FORMS    += TestorDialog.ui \
    RuntimeFilterDialog.ui
