
INCLUDEPATH += ../../../include/MDCommandQueueEngine

LibraryName = Mind.MDCommandQueueEngine

!contains(DEFINES, MDCOMMANDQUEUEENGINE_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug
LIBS += -L../../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release
LIBS += -L../../../bin/release -l$${LibraryName}
}
}
