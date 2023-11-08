INCLUDEPATH +=../../include/interface/Common/
INCLUDEPATH +=../../include/MDDriverLibs/
INCLUDEPATH +=../../include/MDDriverInterfaces/
INCLUDEPATH +=../../include/MDRTDBInterfaces/
LibraryName = Mind.RTDB.MDDriverLibs

!contains(DEFINES, MDDRIVERLIBS_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
