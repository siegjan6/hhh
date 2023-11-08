#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T16:45:07
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


#设置工程名称,注意只需要修改变量值，不要修改变量名称
ProjectName = ExpTest
include(../../../pri/common3.pri)
#加载动态库

#include(MDProjectExpert.pri)

INCLUDEPATH += ../../include/MDExpression \

LibraryName = Mind.Common.MDExpression

!contains(DEFINES, MDEXPRESSION_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}









SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
