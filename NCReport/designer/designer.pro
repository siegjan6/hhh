include(../config.pri)
DEFINES += NCREPORT_DIRECT_INTEGRATION
TEMPLATE = app
QT = xml sql gui core svg network script
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport widgets

contains(EXTENSIONS, highcharts) {
	QT += webkit
}

CONFIG += warn_on qt thread

CONFIG(release, debug|release) {
	TARGET = NCReportDesigner
	DEFINES += QT_NO_DEBUG_OUTPUT
	OBJECTS_DIR = release/obj
	MOC_DIR = release/moc
	UI_DIR = release/uic
}
CONFIG(debug, debug|release) {
    TARGET = NCReportDesignerDebug
	OBJECTS_DIR = debug/obj
	MOC_DIR = debug/moc
	UI_DIR = debug/uic
}

target.path = /usr/local/NCReport/bin
INSTALLS += target
#RC_FILE = designer.rc

DESTDIR = ../../bin

win32|macx {
	DEFINES += DESIGNER_SPLASH
}
macx: ICON = ../../images/designer.icns

INCLUDEPATH += ../ncreport .
contains(EXTENSIONS, pacos) {
    INCLUDEPATH += ./pacos
}

include(../target.pri)
include(../designer.pri)
include(../ncreport.pri)
include(../translations.pri)

SOURCES += main.cpp
