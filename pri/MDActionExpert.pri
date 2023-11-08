
INCLUDEPATH += ../../include/MDActionExpert

LibraryName = Mind.MDActionExpert

!contains(DEFINES, MDACTIONEXPERT_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
