#-------------------------------------------------
#
# Project created by QtCreator 2016-01-12T15:16:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


#设置工程名称,注意只需要修改变量值，不要修改变量名称
ProjectName = MultiLanguageDemo
include(../../pri/common.pri)
#加载动态库
include(../../pri/MDMultiLanguage.pri)
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
