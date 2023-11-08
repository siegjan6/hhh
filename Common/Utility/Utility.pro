#-------------------------------------------------
#
# Project created by QtCreator 2015-05-07T10:55:18
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Utility
TEMPLATE = lib

DEFINES += UTILITY_LIBRARY

include(../../pri/MDUtility.pri)
include(./QDeferred/src/qdeferred.pri)
include(./QDeferred/src/qlambdathreadworker.pri)
include(./QConsoleListener/src/qconsolelistener.pri)
ProjectName = $${LibraryName}

include(../../pri/common.pri)

SOURCES += \
    Utility.cpp \
    MDJsonConvert.cpp \
    qsingleinstance.cpp \
    rs_debug.cpp

HEADERS += \
    CSigleton.h \
    Utility.h \
    ../../include/MDUtility/MDJsonConvert.h \
    qfunctionutils.h \
    qsingleinstance.h \
    ../../include/MDUtility/rs_debug.h

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
    message(copy /y $$win32_PWD\Utility.h $$win32_PWD\..\..\include\MDUtility\Utility.h)
    QMAKE_POST_LINK += copy /y $$win32_PWD\Utility.h $$win32_PWD\..\..\include\MDUtility\Utility.h

}

DISTFILES += \
    QFunctionUtils \
    QSingleInstance
