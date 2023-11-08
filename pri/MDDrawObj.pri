INCLUDEPATH += ../../include/MDDrawObj \
../../include/interface/HMI \
../../include/interface/Common \
../../include/interface/Studio

LibraryName = Mind.HMI.MDDrawObj

!contains(DEFINES, MDDRAWOBJ_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
