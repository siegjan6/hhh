
INCLUDEPATH += ../../include/MDActionConfiger

LibraryName = Mind.MDActionConfiger

!contains(DEFINES, MDACTIONCONFIGER_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
