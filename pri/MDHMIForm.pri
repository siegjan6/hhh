

INCLUDEPATH += ../../include/MDHMIForm \

#生成文件名称
LibraryName = Mind.HMI.MDHMIForm

!contains(DEFINES, MDHMIFORM_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
