#by luyj 2023.03.16

RELEASE_SEPARATE_DEBUG = $$(CUSTOM_RELEASE_SEPARATE_DEBUG)
#设置生成文件名和目录
CONFIG(debug, debug|release){
}else{

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

