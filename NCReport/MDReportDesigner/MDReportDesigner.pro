#-------------------------------------------------
#
# Project created by mxl 2015-05-18 09:20:37
#
#-------------------------------------------------

QT = xml sql gui core script svg network
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport widgets

contains(EXTENSIONS, highcharts) {
	QT += webkit
}

TEMPLATE = lib

INSTALLS = target headers documentation reports

DEFINES += DESIGNER_LIBRARY

DEFINES += NCREPORT_DIRECT_INTEGRATION
INCLUDEPATH += ../ncreport ../designer
macx: ICON = ../../images/ncreport.icns

include(../../pri/MDReportDesigner.pri)

ProjectName = $${LibraryName}

include(../../pri/common.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDEventHelper.pri)
include(../../pri/MDReportOutput.pri)

include(../pri/config.pri)
include(../pri/target.pri)
include(../pri/ncreport.pri)
include(../pri/designer.pri)
include(../pri/translations.pri)

HEADERS += \
    Report.h \
    NewNCReportFileDialog.h \
    ../../include/MDReport/MDReportDesigner.h \
    RenameNCReportFileDilaog.h

SOURCES += \
    Report.cpp \
    NewNCReportFileDialog.cpp \
    MDReportDesigner.cpp \
    RenameNCReportFileDilaog.cpp

FORMS += \
    newncreportfiledialog.ui \
    renamencreportfiledilaog.ui

