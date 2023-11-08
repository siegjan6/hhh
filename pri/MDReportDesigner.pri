INCLUDEPATH += ../../include/MDReport/
INCLUDEPATH += ../../include/interface/Studio /
INCLUDEPATH += ../../include/MDMultiLanguage /


LibraryName = Mind.MDReport.ReportDesigner

!contains(DEFINES, DESIGNER_LIBRARY){

CONFIG(debug, debug|release){

win32:LIBS += -L../../bin/debug -l$${LibraryName}d2

unix:LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
#LIBS += -L../../bin/release -l$${LibraryName}

win32:LIBS += -L../../bin/debug -l$${LibraryName}2

unix:LIBS += -L../../bin/debug -l$${LibraryName}
}
}
