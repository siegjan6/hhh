#-------------------------------------------------
#
# Project created by QtCreator 2015-05-07T10:55:18
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = UaSingleton
TEMPLATE = lib
#msvc:QMAKE_CXXFLAGS += -std=c++11
#msvc:QMAKE_CXXFLAGS += /NOENTRY

DEFINES += UASINGLETON_LIBRARY
#DEFINES += MDUASERVER_STATIC
DEFINES += UA_ENABLE_HISTORIZING
include(../../pri/MDUaServer.pri)
#-------------------------------------------------
include(../../pri/MDUaSingleton.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += CSigleton.cpp
#    UaSingleton.cpp
    

HEADERS += \
    CSigleton.h \
    ../../include/MDUaSingleton/MDUaSingleton_Global.h
#    UaSingleton.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {

    defineReplace(win32Path) {#定义替换
        win32:1 ~= s|/|\\\\|g#将会替换任何与指定的值的正则表达式匹配的任何值
        contains(1, ^\\.*):pfx = \\#如果value存在于一个被叫做variablename的变量的值的列表中，那么成功，否则失败。
        else:pfx =
        segs = $$split(1, \\)#字符串分割
        out =
        for(seg, segs) {
            equals(seg, ..):out = $$member(out, 0) #文档中member函数是只有两个参数的，返回函数的值列表中指定位置的值
            else:!equals(seg, .):out += $$seg
        }
        return($$join(out, \\, $$pfx))
    }

    win32_PWD=$$win32Path($$PWD)
    !build_pass:message(copy /y $$win32_PWD\CSigleton.h $$win32_PWD\..\..\include\MDUaSingleton\CSigleton.h)
    QMAKE_POST_LINK += copy /y $$win32_PWD\CSigleton.h $$win32_PWD\..\..\include\MDUaSingleton\CSigleton.h

}

#DISTFILES += \
#    QFunctionUtils \
#    QSingleInstance
