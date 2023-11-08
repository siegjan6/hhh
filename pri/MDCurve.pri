
INCLUDEPATH += ../../include/MDCurve \
../../include/interface/HMI \
../../include/interface/Studio

LibraryName = Mind.HMI.MDCurve

!contains(DEFINES, MDCURVE_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
