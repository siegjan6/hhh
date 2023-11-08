#-------------------------------------------------
#
# Project created by QtCreator 2015-04-02T10:37:39
#
#-------------------------------------------------

QT       -= gui
QT       += xml
TARGET = RTDBInterfaces
TEMPLATE = lib

DEFINES += RTDBINTERFACES_LIBRARY

SOURCES +=

HEADERS += \
    ../../include/MDRTDBInterfaces/IMDChangeVariable.h \
    ../../include/MDRTDBInterfaces/IMDRTDBVariable.h \
    ../../include/MDRTDBInterfaces/IMDVariable.h \
    ../../include/MDRTDBInterfaces/IMDTreeItem.h \
    ../../include/MDRTDBInterfaces/IMDRTDBManager.h \
    ../../include/MDRTDBInterfaces/IMDDeviceManager.h \
    ../../include/MDRTDBInterfaces/IMDVariableGroup.h \
    ../../include/MDRTDBInterfaces/IMDVariableManager.h \
    ../../include/MDRTDBInterfaces/IMDTreeModel.h \
    ../../include/MDRTDBInterfaces/IMDVariableChangeNotifier.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH +=../../include/interface/Common/
INCLUDEPATH +=../../include/MDDriverInterfaces/
INCLUDEPATH +=../../include/MDVariableArchives/

