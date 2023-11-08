INCLUDEPATH += ../../include/MDGauge \
../../include/interface/HMI

LibraryName = Mind.HMI.MDGauge

!contains(DEFINES, MDGAUGE_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
