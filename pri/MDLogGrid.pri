INCLUDEPATH += ../../include/MDLogGrid \
../../include/interface/HMI

LibraryName = Mind.HMI.MDLogGrid

!contains(DEFINES, MDLOGGRID_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
