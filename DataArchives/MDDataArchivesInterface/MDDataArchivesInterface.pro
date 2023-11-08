#-------------------------------------------------
#
# Project created by QtCreator 2015-07-09T14:13:48
#
#-------------------------------------------------

QT       -= gui
QT       += core

TARGET = MDDataArchivesInterface
TEMPLATE = lib

DEFINES += MDDATAARCHIVESINTERFACE_LIBRARY

HEADERS += \
    ../../include/MDDataArchivesInterface/IMDDataArchives.h \
    ../../include/MDDataArchivesInterface/IMDDataGroup.h \
    ../../include/MDDataArchivesInterface/IMDDataItem.h \
    ../../include/MDDataArchivesInterface/MDDataArchivesInterface_global.h \
    ../../include/MDDataArchivesInterface/IMDDatabaseParam.h \
    ../../include/MDDataArchivesInterface/IMDDataField.h \
    ../../include/MDDataArchivesInterface/IMDDataSource.h \
    ../../include/MDDataArchivesInterface/IMDDataSourceManager.h \
    ../../include/MDDataArchivesInterface/IMDDataTable.h \
    ../../include/MDDataArchivesInterface/MDDataArchivesEngine.h \
    ../../include/MDDataArchivesInterface/mddataarchivesengine_global.h \
    ../../include/MDDataArchivesInterface/MDDataArchivesFactory.h \
    ../../include/MDDataArchivesInterface/MDDataArchives_global.h \
    ../../include/MDDataArchivesInterface/MDDataArchivesNotifyParam.h \
    ../../include/MDDataArchivesInterface/MDDataDefine.h \
    ../../include/MDDataArchivesInterface/MDDataSource.h \
    ../../include/MDDataArchivesInterface/MDDataSourceFactory.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#SOURCES += \
#    ../../include/MDDataArchivesInterface/MDDataSource.cpp
