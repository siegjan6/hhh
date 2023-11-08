INCLUDEPATH += ../../include/MDUtility \

LibraryName = Mind.Common.MDUtility

!contains(DEFINES, UTILITY_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
