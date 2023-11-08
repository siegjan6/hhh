INCLUDEPATH += ../../include/MDVariableArchives /
INCLUDEPATH +=../../include/interface/Common/
INCLUDEPATH +=../../include/MDRTDBInterfaces/
INCLUDEPATH +=../../include/MDDriverInterfaces/

LibraryName = Mind.RTDB.MDVariableArchives

!contains(DEFINES, MDVARIABLEARCHIVES_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
