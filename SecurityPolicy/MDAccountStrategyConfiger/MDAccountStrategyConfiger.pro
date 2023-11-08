#-------------------------------------------------
#
# Project created by QtCreator 2015-07-30T16:54:29
#
#-------------------------------------------------

QT       -= gui

TARGET = MDAccountStrategyConfiger
TEMPLATE = lib

DEFINES += MDACCOUNTSTRATEGYCONFIGER_LIBRARY
#-------------------------------------------------
include(../../pri/MDSecurityPolicyHelper.pri)
#-------------------------------------------------
include(../../pri/MDAccountStrategyConfiger.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDAccountStrategyConfiger.cpp \
    MDAccountStrategyConfigerImp.cpp

HEADERS +=\
    MDAccountStrategyConfigerImp.h \
    ../../include/MDAccountStrategyConfiger/MDAccountStrategyConfiger_Global.h \
    ../../include/MDAccountStrategyConfiger/MDAccountStrategyConfiger.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
