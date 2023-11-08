INCLUDEPATH += ../../include/MDQtRedisClient \

LibraryName = Mind.Common.MDQtRedisClient

!contains(DEFINES, QTREDISCLIENT_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}

