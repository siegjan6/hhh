INCLUDEPATH +=../../include/MDRTDBManagerUI \
../../include/MDRTDBInterfaces

LibraryName = Mind.RTDB.MDRTDBManagerUI

!contains(DEFINES, MDRTDBMANAGERUI_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
