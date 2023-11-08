#-------------------------------------------------------------
# BUILD MODES
#-------------------------------------------------------------
contains(BUILD_MODES, eval_ver) {
	DEFINES += NCREPORT_EVAL
	message("Evaluation version mode enabled")
} else {
        !build_pass:message("NCReport Commercial version mode enabled")
}
contains(BUILD_MODES, debug_verbose) {
	DEFINES += NCREPORT_DEBUG_OUTPUT
	message("Extended debug output mode enabled")
}
contains(BUILD_MODES, nopixmap) {
	DEFINES += USE_QIMAGE_INSTEAD_OF_QPIXMAP
	message("Use QImage instead of QPixmap")
}
contains(BUILD_MODES, translate_labels) {
	DEFINES += LABEL_QT_TRANSLATOR_INTEGRATION
	message("Qt translator integration for labels")
}
#-------------------------------------------------------------
# DESIGNER TESTS
#-------------------------------------------------------------
contains(DESIGNER_TESTS, db_mode_demo) {
	DEFINES += NCRD_DATABASE_MODE_DEMO
	message("Load report from database demo enabled")
}
contains(DESIGNER_TESTS, custom_ds_demo) {
	DEFINES += NCRD_CUSTOM_DATASOURCE_DEMO
	message("Custom data source demo enabled")
}
contains(DESIGNER_TESTS, itemmodel_demo) {
	DEFINES += NCRD_ITEMMODEL_DEMO
	message("Item model data source demo enabled")
}
contains(DESIGNER_TESTS, reportsource_demo) {
	DEFINES += NCRD_CUSTOM_REPORTSOURCE_DEMO
	message("Custom report source demo enabled")
}
contains(DESIGNER_TESTS, testdatamenu) {
	DEFINES += NCRD_TESTDATAMENU
	message("Test data menu is enabled")
}
contains(DESIGNER_TESTS, dragdrop_lf) {
	DEFINES += NCRD_DRAGDROP_LABEL_AS_FIELD
	message("Drag and drob label as field in data source tree")
}
contains(DESIGNER_TESTS, graphitemdialog_demo) {
	DEFINES += NCRD_GRAPHITEMDIALOG_DEMO
	message("Demo of using custom graph item with private setting dialog enabled")
}
contains(DESIGNER_TESTS, customfielddialog_demo) {
	DEFINES += NCRD_CUSTOMFIELDITEMDIALOG_DEMO
	message("Demo of using customized field settings dialog")
}
contains(DESIGNER_TESTS, stringlist_demo) {
	DEFINES += NCRD_STRINGLIST_DEMO
	message("Demo of using string list datasources enabled")
}

#-------------------------------------------------------------
# DISABLED FEATURES
#-------------------------------------------------------------
contains(DISABLED_FEATURES, svg) {
	DEFINES += NCREPORT_NO_SVG
	message(Building without SVG support)
}
contains(DISABLED_FEATURES, sql) {
	DEFINES += NCREPORT_NO_SQL
	message(Building without SQL support)
}
contains(DISABLED_FEATURES, preview_window) {
	DEFINES += NCREPORT_NO_PREVIEW_WINDOW
	message(Building without preview support)
}
contains(DISABLED_FEATURES, email) {
	DEFINES += NCREPORT_NO_EMAIL
	message(Building without email support)
}
contains(DISABLED_FEATURES, table_view) {
	DEFINES += NCREPORT_NO_TABLE_VIEW
	message(Building without TableView support)
}
contains(DISABLED_FEATURES, print_dialog) {
	DEFINES += NCREPORT_NO_PRINT_DIALOG
	message(Building without print dialog support)
}
contains(DISABLED_FEATURES, cursor) {
	DEFINES += NCREPORT_NO_CURSOR
	message(Building without cursor support)
}

#-------------------------------------------------------------
# SPECIAL EXTENSIONS
#-------------------------------------------------------------
contains(EXTENSIONS, highcharts) {
	DEFINES += HIGHCHARTS_INTEGRATION
	message("HighCharts integration is enabled")
}
contains(EXTENSIONS, markdown) {
	DEFINES += MARKDOWN_INTEGRATION
	message("Markdown integration is enabled")
}
contains(EXTENSIONS, pacos) {
	DEFINES += PACOS_INTEGRATION
	message("PACOS integration is enabled")
}
contains(EXTENSIONS, webkit) {
	DEFINES += WEBKIT_INTEGRATION
	message("Qt webkit module enabled")
}
contains(EXTENSIONS, webengine) {
	DEFINES += WEBENGINE_INTEGRATION
	message("Qt5 webengine module enabled")
}
