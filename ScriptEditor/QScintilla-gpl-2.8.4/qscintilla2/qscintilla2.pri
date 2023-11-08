LibraryName = qscintilla2

!contains(DEFINES, QSCINTILLA_MAKE_DLL){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../../bin/debug -l$${LibraryName}
}
}
