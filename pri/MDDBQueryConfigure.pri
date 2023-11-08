
INCLUDEPATH += ../../include/MDDBQueryConfigure/

LibraryName = Mind.MDDataBase.MDDBQueryConfigure

!contains(DEFINES, DBQUERYCONFIGURE_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
