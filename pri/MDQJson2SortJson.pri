INCLUDEPATH += ../../include/MDQJson2SortJson

LibraryName = Mind.Common.MDQJson2SortJson

!contains(DEFINES, MDQJSON2SORTJSON_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
