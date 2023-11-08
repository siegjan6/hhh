#-------------------------------------------------
#
# Project created by QtCreator 2014-03-10T15:01:03
#
#-------------------------------------------------

QT       += core gui
#QT +=    serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTClient
TEMPLATE = app

#QT += network
SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    ICommunicationComponent.h \
    IEventCallback.h \
    communicationlibs_global.h \
    MDCommunicationComponentFactory.h \
    MDCommunicationComponent.h \
    ICommunicationObject.h \
    ICommunicationObjectEx.h \
    MDEnums.h

FORMS    += mainwindow.ui


LIBS +=./debug/CommunicationLibs.lib
