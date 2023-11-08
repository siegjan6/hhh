INCLUDEPATH += ../../include/MDRTDBManager/
INCLUDEPATH +=../../include/MDRTDBInterfaces/
INCLUDEPATH +=../../include/MDDriverInterfaces/
INCLUDEPATH +=../../include/interface/Common/

LibraryName = Mind.RTDB.MDRTDBManager

!contains(DEFINES, MDRTDBMANAGER_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
