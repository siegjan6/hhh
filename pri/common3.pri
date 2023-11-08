#by luyj 2021.02.27
#调用本文件前需要设置ProjectName变量的值

#支持c++11,可以支持类似Enum::enumValue之类的语法
CONFIG += c++11
CONFIG += dll

INCLUDEPATH += ../../include \
../../include/interface

contains(TEMPLATE, app){

}else{
#去除lib前缀和版本号后缀
CONFIG += plugin no_plugin_name_prefix
#lib前缀还是不能少的
unix:ProjectName = lib$${ProjectName}
}

RELEASE_SEPARATE_DEBUG = $$(CUSTOM_RELEASE_SEPARATE_DEBUG)
#设置生成文件名和目录
CONFIG(debug, debug|release){
TARGET = $${ProjectName}d
DESTDIR = ../../../bin/debug

}else{
TARGET = $${ProjectName}
DESTDIR = ../../../bin/release

equals(RELEASE_SEPARATE_DEBUG, "SEPARATE") {
    # separate debug info like xxx.debug or xxx.so.debug or
    CONFIG += force_debug_info
    CONFIG += separate_debug_info
    }
}

# 解决msvc编译后中文乱码的问题
msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8
msvc:QMAKE_CXXFLAGS_WARN_ON += -wd4819

equals(RELEASE_SEPARATE_DEBUG, "SEPARATE") {
    # release with debug symbol info
    * {
        QMAKE_CFLAGS_RELEASE += -g
        QMAKE_CXXFLAGS_RELEASE += -g
        QMAKE_CFLAGS_RELEASE -= -O2
        QMAKE_CXXFLAGS_RELEASE -= -O2
        #QMAKE_LFLAGS_RELEASE = -mthreads -Wl
    }
}
