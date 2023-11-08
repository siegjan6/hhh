unix {
    !macx {
        target.path = /usr/local/NCReport/bin
    } else {
        QMAKE_LFLAGS += -F$$DESIGNER_LIBPATH
    }
}

CONFIG(release, debug|release) {
    win32-g++: LIBS += $$DESIGNER_LIBPATH/libNCReportDesigner2.a
    else:win32: LIBS += $$DESIGNER_LIBPATH/NCReportDesigner2.lib
    unix:!macx: LIBS += -L$$DESIGNER_LIBPATH -lNCReportDesigner
    else: LIBS += -framework NCReport
}
CONFIG(debug, debug|release) {
    win32-g++: LIBS += $$DESIGNER_LIBPATH/libNCReportDesignerDebug2.a
    else:win32: LIBS += $$DESIGNER_LIBPATH/NCReportDesignerDebug2.lib
    unix:!macx: LIBS += -L$$DESIGNER_LIBPATH -lNCReportDesignerDebug
    else: LIBS += -framework NCReportDesignerDebug
}
