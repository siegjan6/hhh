#加载动态库

INCLUDEPATH += ../../include/MDProtoBuf \

#生成文件名称
LibraryName = Mind.ProtoBuf

!contains(DEFINES, MDPROTOBUF_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
