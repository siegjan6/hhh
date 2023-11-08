#-------------------------------------------------
#
# Project created by mxl 2015-06-14 09:39:27
#
#-------------------------------------------------

QT = xml sql gui core script svg network
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport  widgets

contains(EXTENSIONS, highcharts) {
	QT += webkit
}

TEMPLATE = lib

DEFINES += MDREPORTOUTPUT_LIBRARY

DEFINES += DESIGNER_LIBRARY
DEFINES += NCREPORT_DIRECT_INTEGRATION

INCLUDEPATH += ../ncreport
DEPENDPATH = ../ncreport
macx: ICON = ../../images/ncreport.icns

include(../../pri/MDReportOutput.pri)

ProjectName = $${LibraryName}

include(../pri/config.pri)
include(../pri/ncreport.pri)
include(../../pri/common.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDDBQueryUI.pri)

HEADERS += \
    ReportOutput.h \
    ../../include/MDReport/MDReportOutput.h

SOURCES += \
    ReportOutput.cpp \
    MDReportOutput.cpp

DISTFILES +=

RESOURCES += \
    res.qrc
