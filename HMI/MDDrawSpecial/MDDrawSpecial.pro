#-------------------------------------------------
#
# Project created by QtCreator 2015-06-11T10:39:27
#
#-------------------------------------------------

QT       += core gui widgets pdfwidgets

TEMPLATE = lib

DEFINES += MDDRAWSPECIAL_LIBRARY

include(../../pri/MDDrawSpecial.pri)

ProjectName = $${LibraryName}
include(../../pri/common.pri)

include(../../pri/MDDrawObj.pri)
include(../../pri/MDReportOutput.pri)
include(../../pri/MDCurve.pri)
include(../../pri/MDGauge.pri)
include(../../pri/MDDiagram.pri)
include(../../pri/MDDataGroupGrid.pri)
include(../../pri/MDAlarmGrid.pri)
include(../../pri/MDLogGrid.pri)
include(../../pri/MDDataArchivesWidget.pri)
include(../../pri/MDDiagram.pri)
include(../../pri/MDRecipeGrid.pri)
include(../../pri/MDUtility.pri)

SOURCES += \
    DrawReport/MDDrawReport.cpp \
    DrawReport/PropertyDrawReport.cpp \
    DrawReport/DrawReportData.cpp \
    DrawCurve/DrawCurveData.cpp \
    DrawCurve/MDDrawCurve.cpp \
    DrawCurve/PropertyDrawCurve.cpp \
    DrawGauge/DrawGaugeData.cpp \
    DrawGauge/MDDrawGauge.cpp \
    DrawGauge/PropertyDrawGauge.cpp \
    DrawAlarm/DrawAlarmData.cpp \
    DrawAlarm/MDDrawAlarm.cpp \
    DrawAlarm/PropertyDrawAlarm.cpp \
    DrawLogGrid/DrawLogData.cpp \
    DrawLogGrid/MDDrawLog.cpp \
    DrawLogGrid/PropertyDrawLog.cpp \
    DrawDataArchives/DrawDataArchivesData.cpp \
    DrawDataArchives/MDDrawDataArchives.cpp \
    DrawDataArchives/PropertyDrawDataArchives.cpp \
    DrawDiagram/PropertyDrawDiagram.cpp \
    DrawDiagram/MDDrawDiagram.cpp \
    DrawDiagram/DrawDiagramData.cpp \
    DrawRecipeGrid/DrawRecipeGridData.cpp \
    DrawRecipeGrid/MDDrawRecipeGrid.cpp \
    DrawRecipeGrid/PropertyDrawRecipeGrid.cpp \
    DrawProcDocker/DrawProcDockerData.cpp \
    DrawProcDocker/MDDrawProcDocker.cpp \
    DrawProcDocker/procdockerwidget.cpp \
    DrawProcDocker/PropertyDrawProcDocker.cpp \
    DrawPdfViewer/DrawPdfViewerData.cpp \
    DrawPdfViewer/MDDrawPdfViewer.cpp \
    DrawPdfViewer/pdfviewerwidget.cpp \
    DrawPdfViewer/PropertyDrawPdfViewer.cpp \
    DrawPdfViewer/pageselector.cpp \
    DrawPdfViewer/zoomselector.cpp


HEADERS +=\
    DrawReport/PropertyDrawReport.h \
    DrawReport/DrawReportData.h \
    ../../include/MDDrawSpecial/MDDrawReport.h \
    DrawCurve/DrawCurveData.h \
    DrawCurve/PropertyDrawCurve.h \
    ../../include/MDDrawSpecial/MDDrawCurve.h \
    DrawGauge/DrawGaugeData.h \
    DrawGauge/PropertyDrawGauge.h \
    ../../include/MDDrawSpecial/MDDrawGauge.h \
    DrawAlarm/DrawAlarmData.h \
    DrawAlarm/PropertyDrawAlarm.h \
    ../../include/MDDrawSpecial/MDDrawAlarm.h \
    DrawLogGrid/DrawLogData.h \
    DrawLogGrid/PropertyDrawLog.h \
    ../../include/MDDrawSpecial/MDDrawLog.h \
    DrawDataArchives/DrawDataArchivesData.h \
    DrawDataArchives/PropertyDrawDataArchives.h \
    ../../include/MDDrawSpecial/MDDrawDataArchives.h \
    DrawDiagram/PropertyDrawDiagram.h \
    DrawDiagram/DrawDiagramData.h \
    ../../include/MDDrawSpecial/MDDrawDiagram.h \
    DrawRecipeGrid/DrawRecipeGridData.h \
    DrawRecipeGrid/PropertyDrawRecipeGrid.h \
    ../../include/MDDrawSpecial/MDDrawRecipeGrid.h \
    DrawProcDocker/DrawProcDockerData.h \
    DrawProcDocker/PropertyDrawProcDocker.h \
    DrawProcDocker/procdockerwidget.h \
    ../../include/MDDrawSpecial/MDDrawProcDocker.h \
    DrawPdfViewer/DrawPdfViewerData.h \
    DrawPdfViewer/PropertyDrawPdfViewer.h \
    DrawPdfViewer/pdfviewerwidget.h \
    ../../include/MDDrawSpecial/MDDrawPdfViewer.h \
    DrawPdfViewer/pageselector.h \
    DrawPdfViewer/zoomselector.h

FORMS += \
    DrawPdfViewer/pdfviewerwidget.ui \
    DrawProcDocker/procdockerwidget.ui

RESOURCES += \
    DrawPdfViewer/resources.qrc
