#-------------------------------------------------
#
# Project created by QtCreator 2015-04-02T10:37:39
#
#-------------------------------------------------

QT       -= gui
QT       += widgets

TARGET = RTDBInterfaces
TEMPLATE = lib

DEFINES += RTDBINTERFACES_LIBRARY

SOURCES +=

HEADERS += \
    ../../include/MDDriverInterfaces/ICommunicationComponent.h \
    ../../include/MDDriverInterfaces/IDataBlock.h \
    ../../include/MDDriverInterfaces/IDevice.h \
    ../../include/MDDriverInterfaces/IDriver.h \
    ../../include/MDDriverInterfaces/IEventCallback.h \
    ../../include/MDDriverInterfaces/IMDDriverManager.h \
    ../../include/MDDriverInterfaces/IMDDriverProperty.h \
    ../../include/MDDriverInterfaces/IMDDriverInformation.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
INCLUDEPATH +=../../include/interface/Common/
