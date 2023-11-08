INCLUDEPATH += ../../include/MDDataSelectUI/

LibraryName =  Mind.DataArchives.MDDataSelectUI

!contains(DEFINES, MDDATASELECTUI_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
