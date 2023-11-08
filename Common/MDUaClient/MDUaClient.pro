#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T14:45:48
#
#-------------------------------------------------

QT       += core sql

TARGET = MDUaClient
TEMPLATE = lib


DEFINES += MDUACLIENT_LIBRARY
DEFINES += UA_ENABLE_HISTORIZING
QMAKE_CFLAGS += -std=c99

include(../../pri/MDUaClient.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

_OPEN62541VER = $$(OPEN62541VER)
!build_pass:message(MDUaClient open62541 VERSION=$${_OPEN62541VER})

CONFIG(debug, debug|release){
    win32:!win32-g++ {
    #    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/$${LibraryName}d.lib

        LIBS += -lws2_32 \
            -liphlpapi \
            -lcrypt32 \
            -ladvapi32

        # Additional libraries to load
        LIBS += -L$$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH) \
            -lmbedcryptod \
            -lmbedtlsd \
            -lmbedx509d \
            -lopen62541d$${_OPEN62541VER}

        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedcryptod.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedtlsd.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedx509d.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/open62541d$${_OPEN62541VER}.lib

    }
}else{
    win32:!win32-g++ {
    #    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/$${LibraryName}.lib
        #win32-g++: PRE_TARGETDEPS += ../../bin/release/lib$${LibraryName}.a
        #else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += ../../bin/release/$${LibraryName}.lib
        # Additional libraries to load
        LIBS += -lws2_32 \
            -liphlpapi \
            -lcrypt32 \
            -ladvapi32

        LIBS += -L$$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH) \
            -lmbedcrypto \
            -lmbedtls \
            -lmbedx509 \
            -lopen62541$${_OPEN62541VER}

        #PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/crypt32.lib
        #PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/advapi32.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedcrypto.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedtls.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedx509.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/open62541$${_OPEN62541VER}.lib

    #    message(path=$${LibraryName})
    }
}

HEADERS += ../../include/MDUaClient/MDUaClient_Global.h \
    ../MDUaServer/QUaServer/amalgamation/open62541.h \
    ../MDUaServer/QUaServer/wrapper/pch_open62541.h \
    opcua_client.h

SOURCES += opcua_client.cpp


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
    win32_DESTDIR=$$win32Path($$DESTDIR)
#    message(copy /y $$win32_DESTDIR\\$${TARGET}.lib $$win32_PWD\..\..\libs\\$${VSCMD_ARG_TGT_ARCH}\\$${TARGET}.lib)
#    QMAKE_POST_LINK += copy /y $$win32_DESTDIR\\$${TARGET}.lib $$win32_PWD\..\..\libs\\$${VSCMD_ARG_TGT_ARCH}\\$${TARGET}.lib

}
