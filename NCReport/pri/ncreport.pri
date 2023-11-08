VERSION=$$APPVERSION

include(../../pri/MDDBQueryConfigure.pri)
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDDBQueryEngine.pri)

HEADERS += \
$$PWD/../ncreport/info.h \
$$PWD/../ncreport/ncreportscale.h \
$$PWD/../ncreport/ncreportdef.h \
$$PWD/../ncreport/ncreportxmlreader.h \
$$PWD/../ncreport/ncreportoutput.h \
$$PWD/../ncreport/ncreportdata.h \
$$PWD/../ncreport/ncreportsection.h \
$$PWD/../ncreport/ncreportgroup.h \
$$PWD/../ncreport/ncreportparameter.h \
$$PWD/../ncreport/ncreportvariable.h \
$$PWD/../ncreport/ncreportpageoption.h \
$$PWD/../ncreport/ncreportdatasource.h \
$$PWD/../ncreport/ncreportfielditem.h \
$$PWD/../ncreport/ncreportitem.h \
$$PWD/../ncreport/ncreportlabelitem.h \
$$PWD/../ncreport/ncreportlineitem.h \
$$PWD/../ncreport/ncreportrectitem.h \
$$PWD/../ncreport/ncreportimageitem.h \
$$PWD/../ncreport/ncreportbarcodeitem.h \
$$PWD/../ncreport/ncreportshapeitem.h \
$$PWD/../ncreport/ncreportevaluator.h \
$$PWD/../ncreport/ncreportdirector.h \
$$PWD/../ncreport/ncreport.h \
$$PWD/../ncreport/ncreportsource.h \
$$PWD/../ncreport/ncreportxmldefreader.h \
$$PWD/../ncreport/ncreporttextdatasource.h \
$$PWD/../ncreport/ncreporttxtdatasource.h \
$$PWD/../ncreport/ncreportxmlwriter.h \
$$PWD/../ncreport/ncreportxmldefwriter.h \
$$PWD/../ncreport/ncreportstringlistdatasource.h \
$$PWD/../ncreport/ncreportgraphitem.h \
$$PWD/../ncreport/ncreportgraphrenderer.h \
$$PWD/../ncreport/ncreportabstractitemrendering.h \
$$PWD/../ncreport/ncreportitemmodeldatasource.h \
$$PWD/../ncreport/ncreporttextitem.h \
$$PWD/../ncreport/ncreportsectiondirector.h \
$$PWD/../ncreport/ncreportutils.h \
$$PWD/../ncreport/ncreportbarcoderenderer.h \
$$PWD/../ncreport/ncreport_global.h \
$$PWD/../ncreport/ncreporthttpclient.h \
$$PWD/../ncreport/ncreportmarkuplanguageoutput.h \
$$PWD/../ncreport/ncreporthtmloutput.h \
$$PWD/../ncreport/ncreportdatasourcefactory.h \
$$PWD/../ncreport/ncreportdummydatasourcefactory.h \
$$PWD/../ncreport/ncreportcustomdsprovider.h \
$$PWD/../ncreport/ncreportdatafieldinfo.h \
$$PWD/../ncreport/ncreportcrosstabitemcell.h \
$$PWD/../ncreport/ncreportcrosstabitem.h \
$$PWD/../ncreport/ncreportellipseitem.h \
$$PWD/../ncreport/ncreportconditionalformat.h \
$$PWD/../ncreport/ncreportpivottablemodel.h \
$$PWD/../ncreport/ncreportimageoutput.h \
$$PWD/../ncreport/ncreportwidgetrenderer.h \
$$PWD/../ncreport/ncreporttextoutput.h \
$$PWD/../ncreport/ncreportprinteroutput.h \
$$PWD/../ncreport/ncreportpdfoutput.h \
$$PWD/../ncreport/ncreportxmldatasource.h \
$$PWD/../ncreport/ncreportuserfunction.h \
$$PWD/../ncreport/ncreportusercustomfunction.h \
$$PWD/../ncreport/ncreportexcelhtmloutput.h \
$$PWD/../ncreport/ncreportvariablemodifier.h \
$$PWD/../ncreport/ncreportdatasourcerelation.h \
$$PWD/../ncreport/ncreportdataformat.h\
$$PWD/../ncreport/ncreportdataviewdatasource.h

SOURCES += \
$$PWD/../ncreport/ncreportscale.cpp \
$$PWD/../ncreport/ncreportdef.cpp \
$$PWD/../ncreport/ncreportxmlreader.cpp \
$$PWD/../ncreport/ncreportoutput.cpp \
$$PWD/../ncreport/ncreportdata.cpp \
$$PWD/../ncreport/ncreportsection.cpp \
$$PWD/../ncreport/ncreportgroup.cpp \
$$PWD/../ncreport/ncreportparameter.cpp \
$$PWD/../ncreport/ncreportvariable.cpp \
$$PWD/../ncreport/ncreportpageoption.cpp \
$$PWD/../ncreport/ncreportdatasource.cpp \
$$PWD/../ncreport/ncreportfielditem.cpp \
$$PWD/../ncreport/ncreportitem.cpp \
$$PWD/../ncreport/ncreportlabelitem.cpp \
$$PWD/../ncreport/ncreportlineitem.cpp \
$$PWD/../ncreport/ncreportrectitem.cpp \
$$PWD/../ncreport/ncreportimageitem.cpp \
$$PWD/../ncreport/ncreportbarcodeitem.cpp \
$$PWD/../ncreport/ncreportshapeitem.cpp \
$$PWD/../ncreport/ncreportevaluator.cpp \
$$PWD/../ncreport/ncreportdirector.cpp \
$$PWD/../ncreport/ncreport.cpp \
$$PWD/../ncreport/ncreportsource.cpp \
$$PWD/../ncreport/ncreportxmldefreader.cpp \
$$PWD/../ncreport/ncreporttextdatasource.cpp \
$$PWD/../ncreport/ncreporttxtdatasource.cpp \
$$PWD/../ncreport/ncreportxmlwriter.cpp \
$$PWD/../ncreport/ncreportxmldefwriter.cpp \
$$PWD/../ncreport/ncreportstringlistdatasource.cpp \
$$PWD/../ncreport/ncreportgraphitem.cpp \
$$PWD/../ncreport/ncreportgraphrenderer.cpp \
$$PWD/../ncreport/ncreportabstractitemrendering.cpp \
$$PWD/../ncreport/ncreportitemmodeldatasource.cpp \
$$PWD/../ncreport/ncreporttextitem.cpp \
$$PWD/../ncreport/ncreportsectiondirector.cpp \
$$PWD/../ncreport/ncreportutils.cpp \
$$PWD/../ncreport/ncreportbarcoderenderer.cpp \
$$PWD/../ncreport/ncreporthttpclient.cpp \
$$PWD/../ncreport/ncreportmarkuplanguageoutput.cpp \
$$PWD/../ncreport/ncreporthtmloutput.cpp \
$$PWD/../ncreport/ncreportdatasourcefactory.cpp \
$$PWD/../ncreport/ncreportdummydatasourcefactory.cpp \
$$PWD/../ncreport/ncreportdatafieldinfo.cpp \
$$PWD/../ncreport/ncreportcustomdsprovider.cpp \
$$PWD/../ncreport/ncreportcrosstabitemcell.cpp \
$$PWD/../ncreport/ncreportcrosstabitem.cpp \
$$PWD/../ncreport/ncreportellipseitem.cpp \
$$PWD/../ncreport/ncreportconditionalformat.cpp \
$$PWD/../ncreport/ncreportpivottablemodel.cpp \
$$PWD/../ncreport/ncreportimageoutput.cpp \
$$PWD/../ncreport/ncreportwidgetrenderer.cpp \
$$PWD/../ncreport/ncreporttextoutput.cpp \
$$PWD/../ncreport/ncreportprinteroutput.cpp \
$$PWD/../ncreport/ncreportpdfoutput.cpp \
$$PWD/../ncreport/ncreportxmldatasource.cpp \
$$PWD/../ncreport/ncreportuserfunction.cpp \
$$PWD/../ncreport/ncreportusercustomfunction.cpp \
$$PWD/../ncreport/ncreportexcelhtmloutput.cpp \
$$PWD/../ncreport/ncreportvariablemodifier.cpp \
$$PWD/../ncreport/ncreportdatasourcerelation.cpp \
$$PWD/../ncreport/ncreportdataformat.cpp\
$$PWD/../ncreport/ncreportdataviewdatasource.cpp

#-------------------------------------------------
# OPTIONAL FEATURES
#-------------------------------------------------
!contains(DISABLED_FEATURES, svg) {
        HEADERS += $$PWD/../ncreport/ncreportsvgoutput.h
        SOURCES += $$PWD/../ncreport/ncreportsvgoutput.cpp
}
!contains(DISABLED_FEATURES, sql) {
        HEADERS += $$PWD/../ncreport/ncreportsqldatasource.h
        SOURCES += $$PWD/../ncreport/ncreportsqldatasource.cpp
}
!contains(DISABLED_FEATURES, preview_window) {
	HEADERS += \
        $$PWD/../ncreport/ncreportqtpreviewoutput.h \
        $$PWD/../ncreport/ncreportpreviewoutput.h \
        $$PWD/../ncreport/ncreportpreviewwindow_p.h \
        $$PWD/../ncreport/ncreportpreviewwindow.h \
        $$PWD/../ncreport/ncreportpreviewpagecontainer.h \
        $$PWD/../ncreport/ncreportpreviewpage.h

	SOURCES += \
        $$PWD/../ncreport/ncreportpreviewoutput.cpp \
        $$PWD/../ncreport/ncreportqtpreviewoutput.cpp \
        $$PWD/../ncreport/ncreportpreviewwindow.cpp \
        $$PWD/../ncreport/ncreportpreviewpagecontainer.cpp \
        $$PWD/../ncreport/ncreportpreviewpage.cpp

	RESOURCES += \
        $$PWD/../ncreport/preview.qrc
}
!contains(DISABLED_FEATURES, email) {
	HEADERS += \
        $$PWD/../ncreport/email/mailsender.h \
        $$PWD/../ncreport/email/mailsender_p.h

	SOURCES += \
        $$PWD/../ncreport/email/mailsender.cpp \
        $$PWD/../ncreport/email/mailsender_p.cpp
}
!contains(DISABLED_FEATURES, table_view) {
        HEADERS += $$PWD/../ncreport/ncreporttableitem.h
        SOURCES += $$PWD/../ncreport/ncreporttableitem.cpp
}
contains(EXTENSIONS, highcharts) {
	HEADERS += \
        $$PWD/../ncreport/ncreportchartitem.h \
        $$PWD/../ncreport/ncreporthighchartsmanager.h

	SOURCES += \
        $$PWD/../ncreport/ncreportchartitem.cpp \
        $$PWD/../ncreport/ncreporthighchartsmanager.cpp
}
contains(EXTENSIONS, webkit) {
	QT += webkit
	greaterThan(QT_MAJOR_VERSION, 4): {
		greaterThan(QT_MINOR_VERSION, 3): QT += webkitwidgets
	}
}
contains(EXTENSIONS, webengine) {
	greaterThan(QT_MAJOR_VERSION, 4): QT += webengine
}
#contains(EXTENSIONS, markdown) {
#        include($$PWD/../3rdparty/markdown/markdown.pri)
#}

#QT += pdf

