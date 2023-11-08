#加载动态库

INCLUDEPATH += ../../include/MDRedisClient \

#生成文件名称
LibraryName = Mind.RedisClient

!contains(DEFINES, MDREDISCLIENT_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
