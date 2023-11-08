#-------------------------------------------------
#
# Project created by QtCreator 2016-04-25T09:23:16
#
#-------------------------------------------------

QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

include(../../pri/MDHMIFramework_make_all.pri)
include(../../pri/MDHMIFramework_make.pri)

#生成文件名称
LibraryName = Mind.HMI.HMIFrameworkTest
ProjectName = $${LibraryName}
include(../../pri/common.pri)

INCLUDEPATH += ../MDHMIFramework/test \


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
