#-------------------------------------------------
#
# Project created by QtCreator 2015-11-03T10:14:03
#
#-------------------------------------------------

QT       += widgets

QT       -= core gui

TARGET = MDFilterTreePlugin
TEMPLATE = lib

DEFINES += MDFILTERTREEPLUGIN_LIBRARY

include(../../pri/MDFilterTreePlugin.pri)
ProjectName =$${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDDataArchives.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDDataSource.pri)

SOURCES += \
    MDFilterTreePlugin.cpp \
    MDFilterConditionSetting.cpp \
    FilterTreePlugin.cpp

HEADERS += \
    ../../include/MDFilterTreePlugin/mdfiltertreeplugin_global.h \
    ../../include/MDFilterTreePlugin/MDFilterTreePlugin.h \
    MDFilterConditionSetting.h \
    FilterTreePlugin.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    filterResource.qrc

