
INCLUDEPATH += ../../include/MDVariableHelper
INCLUDEPATH += ../../include/MDRTDBInterfaces
INCLUDEPATH += ../../include/interface/Common

LibraryName = Mind.MDVariableHelper

!contains(DEFINES, MDVARIABLEHELPER_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
