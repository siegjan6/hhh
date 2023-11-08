INCLUDEPATH += ../../include/MDReport \
INCLUDEPATH += ../../include/interface/Studio /
INCLUDEPATH += ../../include/MDMultiLanguage /

LibraryName = Mind.MDReport.ReportOutput

!contains(DEFINES, MDREPORTOUTPUT_LIBRARY){

CONFIG(debug, debug|release){

win32:LIBS += -L../../bin/debug -l$${LibraryName}d2

unix:LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
#LIBS += -L../../bin/release -l$${LibraryName}

win32:LIBS += -L../../bin/release -l$${LibraryName}2

unix:LIBS += -L../../bin/release -l$${LibraryName}
}
}
