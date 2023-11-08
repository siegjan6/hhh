
INCLUDEPATH += ../../include/MDEventHelper
INCLUDEPATH += ../../include/MDRTDBInterfaces
INCLUDEPATH += ../../include/MDDriverInterfaces
LibraryName = Mind.MDEventHelper

!contains(DEFINES, MDEVENTHELPER_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
