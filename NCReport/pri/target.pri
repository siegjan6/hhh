unix {
	macx {
		CONFIG += lib_bundle
		FRAMEWORK_HEADERS.version = Versions
		FRAMEWORK_HEADERS.files = $$HEADERS
		FRAMEWORK_HEADERS.path = Headers
		QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
	} else {
        target.path = /usr/local/NCReport/lib
        headers.path = /usr/local/NCReport/include
		headers.files = $$HEADERS
        documentation.path = /usr/share/doc/NCReport
        documentation.files = ../../doc/*
        reports.path = /usr/local/NCReport/reports
        reports.files = ../../reports/*
    }
}
win32 {
	target.path = $$(WINDOWS_INSTALL_PATH)/bin
	headers.path = $$(WINDOWS_INSTALL_PATH)/include
	headers.files = $$HEADERS
	documentation.path = $$(WINDOWS_INSTALL_PATH)/doc
        documentation.files = ../../doc/*
	reports.path = $$(WINDOWS_INSTALL_PATH)/reports
        reports.files = ../../reports/*
}


