#-------------------------------------------------
#
# Project created by QtCreator 2015-07-30T16:21:01
#
#-------------------------------------------------

QT       -= gui

TARGET = MDPowerGroupConfiger
TEMPLATE = lib

DEFINES += MDPOWERGROUPCONFIGER_LIBRARY
#-------------------------------------------------
include(../../pri/MDSecurityPolicyHelper.pri)
#-------------------------------------------------
include(../../pri/MDPowerGroupConfiger.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDPowerGroupConfiger.cpp \
    MDPowerGroupConfigerImp.cpp

HEADERS +=\
    MDPowerGroupConfigerImp.h \
    ../../include/MDPowerGroupConfiger/MDPowerGroupConfiger_Global.h \
    ../../include/MDPowerGroupConfiger/MDPowerGroupConfiger.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
