#-------------------------------------------------
#
# Project created by QtCreator 2016-06-01T15:36:49
#
#-------------------------------------------------

QT       += core gui widgets script xml sql

TEMPLATE = app
include(../../pri/DataArchiveTestor.pri)
#设置生成文件名和目录
ProjectName =  Mind.DataArchives.MDDataArchivesTestor
include(../../pri/common.pri)
include(../../pri/MDLogExpert.pri)
INCLUDEPATH +=../../include/Testor

TEMPLATE = app

DEFINES += FRAMEWORK_TESTOR

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug

}else{
LIBS += -L../../bin/release
}




SOURCES += main.cpp\
        mainwidget.cpp \
    contentqsplitter.cpp \
    maintreewidget.cpp \
    globalinst.cpp \
    NewDBsourceDialog.cpp \
    dbdatagrouptreecreator.cpp \
    MDNewDataGroup.cpp \
    containerwidget.cpp \
    testdialog.cpp \
    MDDataGroupTableWidget.cpp \
    MDDataArchivesTableWidget.cpp \
    testdataeditor.cpp \
    scripteditor.cpp \
    newfiledialog.cpp \
    testdatagroupmgr.cpp \
    jsoutputwindow.cpp \
    jsdataarchivetestor.cpp \
    JSScriptExcutor.cpp \
    CaseGroupMgr.cpp \
    scriptcasegroupmgr.cpp \
    DBSourceParamWidget.cpp \
    MDDataSrcTabWidget.cpp \
    DbTableFieldDialog.cpp \
    DataArchivesTabModel.cpp \
    DBTableDialog.cpp \
    newprojectdialog.cpp \
    tsqlexecutor.cpp

HEADERS  += mainwidget.h \
    contentqsplitter.h \
    maintreewidget.h \
    globalinst.h \
    NewDBsourceDialog.h \
    dbdatagrouptreecreator.h \
    MDNewDataGroup.h \
    containerwidget.h \
    testdialog.h \
    MDDataGroupTableWidget.h \
    MDDataArchivesTableWidget.h \
    testdataeditor.h \
    scripteditor.h \
    newfiledialog.h \
    testdatagroupmgr.h \
    jsoutputwindow.h \
    jsdataarchivetestor.h \
    JSScriptExcutor.h \
    CaseGroupMgr.h \
    scriptcasegroupmgr.h \
    DBSourceParamWidget.h \
    MDDataSrcTabWidget.h \
    DbTableFieldDialog.h \
    DataArchivesTabModel.h \
    DBTableDialog.h \
    newprojectdialog.h \
    tsqlexecutor.h \
    JSDBParameters.h

FORMS    += mainwidget.ui \
    newdbsourcedialog.ui \
    testdialog.ui \
    testdataeditor.ui \
    scripteditor.ui \
    newfiledialog.ui \
    dbtablefielddialog.ui \
    newprojectdialog.ui

RESOURCES += \
    resource.qrc
