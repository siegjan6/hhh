INCLUDEPATH += ../../include/MDProjectExpert

LibraryName = Mind.MDProjectExpert

!contains(DEFINES, MDPROJECTEXPERT_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
