#加载动态库

INCLUDEPATH += ../../include/MDPythonAMQClient \

#生成文件名称
LibraryName = Mind.PythonAMQClient

!contains(DEFINES, MDPYTHONAMQCLIENT_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
