include(../config.pri)

QT = xml gui core script network
!contains(DISABLED_FEATURES, svg): QT += svg
!contains(DISABLED_FEATURES, sql): QT += sql
contains(EXTENSIONS, highcharts): QT += webkit
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport widgets

TEMPLATE = lib
#CONFIG += debug_and_release
#CONFIG += build_all

MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = uic

CONFIG(release, debug|release) {
	message(Release build!)
    TARGET = NCReport
	DEFINES += QT_NO_DEBUG_OUTPUT
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,/release)
	MOC_DIR = $$join(MOC_DIR,,,/release)
	UI_DIR = $$join(UI_DIR,,,/release)
}
CONFIG(debug, debug|release) {
	message(Debug build!)
    TARGET = NCReportDebug
	OBJECTS_DIR = $$join(OBJECTS_DIR,,,/debug)
	MOC_DIR = $$join(MOC_DIR,,,/debug)
	UI_DIR = $$join(UI_DIR,,,/debug)
}
win32|win64 {
	win32-msvc*:contains(QMAKE_TARGET.arch, x86_64): {
		message( "Building for 64 bit")
	} else {
		message( "Building for 32 bit")
	}
}
INSTALLS = target headers documentation reports
RC_FILE = ncreport.rc
DEFINES += NCREPORT_LIBRARY

DESTDIR = ../../lib
macx: ICON = ../../images/ncreport.icns

include(../ncreport.pri)
include(../target.pri)
include(../translations.pri)


