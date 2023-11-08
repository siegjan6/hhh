
INCLUDEPATH += ../../../include/MDColorManager/

LibraryName = Mind.HMI.MDColorManager

!contains(DEFINES, MDCOLORMANAGER_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
