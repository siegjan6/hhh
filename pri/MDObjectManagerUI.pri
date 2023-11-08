INCLUDEPATH += ../../include/MDObjectManagerUI

LibraryName = Mind.MDObjectManagerUI

!contains(DEFINES, MDOBJECTMANAGERUI_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
