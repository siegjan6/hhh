
INCLUDEPATH += ../../include/MDEventConfiger

LibraryName = Mind.MDEventConfiger

!contains(DEFINES, MDEVENTCONFIGER_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
