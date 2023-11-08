INCLUDEPATH +=../../include/MDDriverCommunicationLibs
INCLUDEPATH +=../../include/MDDriverInterfaces
INCLUDEPATH +=../../include/MDExtSerialPort



LibraryName = Mind.RTDB.MDDriverCommunicationLibs

!contains(DEFINES, MDDRIVERCOMMUNICATIONLIBS_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}

