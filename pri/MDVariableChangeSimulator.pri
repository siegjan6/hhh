
include(MDVariableHelper.pri)

INCLUDEPATH += ../../include/MDVariableChangeSimulator

LibraryName = Mind.MDVariableChangeSimulator

!contains(DEFINES, MDVARIABLECHANGESIMULATOR_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
