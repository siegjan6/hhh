INCLUDEPATH += ../../include/MDExtSerialPort \

LibraryName = Mind.Common.MDExtSerialPort

!contains(DEFINES, EXTSERIALPORT_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}

