INCLUDEPATH += ../../include/MDDiagram \
../../include/interface/HMI

LibraryName = Mind.HMI.MDDiagram

!contains(DEFINES, MDDIAGRAM_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
