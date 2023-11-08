QT = xml sql gui core script svg network
CONFIG += warn_on qt thread
CONFIG += debug
#CONFIG += release
CONFIG(debug, debug|release) {
	TARGET = NCReportDesigner_debug
} else {
	TARGET = NCReportDesigner
	DEFINES += QT_NO_DEBUG_OUTPUT
}

TEMPLATE = app
OBJECTS_DIR = obj
MOC_DIR = moc
UI_DIR = uic
target.path = /usr/local/ncreport/bin
INSTALLS = target
RC_FILE = designer.rc
macx {
	ICON = ../../ncreport2designer.icns
}
DESTDIR = ../../bin
RESOURCES = ncrd.qrc

win32|macx {
	DEFINES += DESIGNER_SPLASH
}
SOURCES += main.cpp

INCLUDEPATH += ../ncreport ../preview .
DEPENDPATH += ../ncreport ../preview


include(../config.pri)
include(../designer.pri)
include(../preview.pri)
include(../target.pri)
include(../translations.pri)

HEADERS += info.h \
    ncreportscale.h \
    ncreportdef.h \
    ncreportxmlreader.h \
    ncreportoutput.h \
    ncreportdata.h \
    ncreportsection.h \
    ncreportgroup.h \
    ncreportparameter.h \
    ncreportvariable.h \
    ncreportpageoption.h \
    ncreportdatasource.h \
    ncreportfielditem.h \
    ncreportitem.h \
    ncreportlabelitem.h \
    ncreportlineitem.h \
    ncreportrectitem.h \
    ncreportimageitem.h \
    ncreportbarcodeitem.h \
    ncreportshapeitem.h \
    ncreportsqldatasource.h \
    ncreportevaluator.h \
    ncreportdirector.h \
    ncreport.h \
    ncreportsource.h \
    ncreportxmldefreader.h \
    ncreporttxtdatasource.h \
    ncreportxmlwriter.h \
    ncreportxmldefwriter.h \
    ncreportstringlistdatasource.h \
    ncreportgraphitem.h \
    ncreportabstractitemrendering.h \
    ncreportpreviewoutput.h \
    ncreportitemmodeldatasource.h \
    ncreporttextitem.h \
    ncreportsectiondirector.h \
    ncreportsvgoutput.h \
    ncreportqtpreviewoutput.h \
    utils.h \
    ncreportbarcoderenderer.h \
    sharedlib_global.h \
    ncreporthttpclient.h \
	ncreporthtmloutput.h

SOURCES += ncreportscale.cpp \
    ncreportdef.cpp \
    ncreportxmlreader.cpp \
    ncreportoutput.cpp \
    ncreportdata.cpp \
    ncreportsection.cpp \
    ncreportgroup.cpp \
    ncreportparameter.cpp \
    ncreportvariable.cpp \
    ncreportpageoption.cpp \
    ncreportdatasource.cpp \
    ncreportfielditem.cpp \
    ncreportitem.cpp \
    ncreportlabelitem.cpp \
    ncreportlineitem.cpp \
    ncreportrectitem.cpp \
    ncreportimageitem.cpp \
    ncreportbarcodeitem.cpp \
    ncreportshapeitem.cpp \
    ncreportsqldatasource.cpp \
    ncreportevaluator.cpp \
    ncreportdirector.cpp \
    ncreport.cpp \
    ncreportsource.cpp \
    ncreportxmldefreader.cpp \
    ncreporttxtdatasource.cpp \
    ncreportxmlwriter.cpp \
    ncreportxmldefwriter.cpp \
    ncreportstringlistdatasource.cpp \
    ncreportgraphitem.cpp \
    ncreportabstractitemrendering.cpp \
    ncreportpreviewoutput.cpp \
    ncreportitemmodeldatasource.cpp \
    ncreporttextitem.cpp \
    ncreportsectiondirector.cpp \
    ncreportsvgoutput.cpp \
    ncreportqtpreviewoutput.cpp \
    utils.cpp \
    ncreportbarcoderenderer.cpp \
    ncreporthttpclient.cpp \
	ncreporthtmloutput.cpp
