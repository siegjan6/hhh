_QUASERVER_VER = $$(QUASERVER_VER)
!build_pass:message(MDUaServer QUaServer.pri VERSION=$${_QUASERVER_VER})

INCLUDEPATH += ../../Common/MDUaServer/QUaServer/amalgamation
INCLUDEPATH += ../../Common/MDUaServer/QUaServer/wrapper$${_QUASERVER_VER}
INCLUDEPATH += ../../include/3rdparty/mbedtls
DEPENDPATH += ../../Common/MDUaServer/QUaServer/wrapper$${_QUASERVER_VER}

LibraryName = Mind.UaServer

DEFINES += UA_ENABLE_HISTORIZING
#DEFINES += UA_DYNAMIC_LINKING UA_DYNAMIC_LINKING_EXPORT
_OPEN62541VER = $$(OPEN62541VER)
!build_pass:message(MDUaServer open62541 VERSION=$${_OPEN62541VER})
!contains(DEFINES, MDUASERVER_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d
win32:!win32-g++ {
    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/$${LibraryName}d.lib

    LIBS += -lws2_32 \
        -liphlpapi \
        -lcrypt32 \
        -ladvapi32

    # Additional libraries to load
    LIBS += -L$$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH) \
        -lmbedcryptod \
        -lmbedtlsd \
        -lmbedx509d \
        -lopen62541d$${_OPEN62541VER}

    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedcryptod.lib
    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedtlsd.lib
    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedx509d.lib
    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/open62541d$${_OPEN62541VER}.lib
}

    unix{
        VSCMD_ARG_TGT_ARCH=x32
        LIBS += -L../../bin/debug -l$${LibraryName}d
#        PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/lib$${LibraryName}d.lib
        LIBS += -L$$PWD/../libs/$$VSCMD_ARG_TGT_ARCH \
            -lmbedcryptod \
            -lmbedtlsd \
            -lmbedx509d \
            -lopen62541d$${_OPEN62541VER}

        PRE_TARGETDEPS += $$PWD/../libs/$$VSCMD_ARG_TGT_ARCH/libmbedcryptod.a
        PRE_TARGETDEPS += $$PWD/../libs/$$VSCMD_ARG_TGT_ARCH/libmbedtlsd.a
        PRE_TARGETDEPS += $$PWD/../libs/$$VSCMD_ARG_TGT_ARCH/libmbedx509d.a
        PRE_TARGETDEPS += $$PWD/../libs/$$VSCMD_ARG_TGT_ARCH/libopen62541d$${_OPEN62541VER}.a
    }

}else{



win32:!win32-g++ {
    LIBS += -L../../bin/release -l$${LibraryName}
#    LIBS += -L$$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH) -l$${LibraryName}

    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/$${LibraryName}.lib
    #win32-g++: PRE_TARGETDEPS += ../../bin/release/lib$${LibraryName}.a
    #else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += ../../bin/release/$${LibraryName}.lib
    # Additional libraries to load
    LIBS += -lws2_32 \
        -liphlpapi \
        -lcrypt32 \
        -ladvapi32

    LIBS += -L$$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH) \
        -lmbedcrypto \
        -lmbedtls \
        -lmbedx509 \
        -lopen62541$${_OPEN62541VER}

    #PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/crypt32.lib
    #PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/advapi32.lib
    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedcrypto.lib
    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedtls.lib
    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedx509.lib
    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/open62541$${_OPEN62541VER}.lib

#    message(path=$${LibraryName})
}

    unix{
        VSCMD_ARG_TGT_ARCH=x32
        LIBS += -L../../bin/release -l$${LibraryName}
#        CONFIG += static
#        PRE_TARGETDEPS += $$PWD/../libs/$$VSCMD_ARG_TGT_ARCH/lib$${LibraryName}.a

        LIBS += -L$$PWD/../libs/$$VSCMD_ARG_TGT_ARCH \
            -lmbedcrypto \
            -lmbedtls \
            -lmbedx509 \
            -lopen62541$${_OPEN62541VER}
#            \
#            -lOpen62541CppWrapper

        PRE_TARGETDEPS += $$PWD/../libs/$$VSCMD_ARG_TGT_ARCH/libmbedcrypto.a
        PRE_TARGETDEPS += $$PWD/../libs/$$VSCMD_ARG_TGT_ARCH/libmbedtls.a
        PRE_TARGETDEPS += $$PWD/../libs/$$VSCMD_ARG_TGT_ARCH/libmbedx509.a
        PRE_TARGETDEPS += $$PWD/../libs/$$VSCMD_ARG_TGT_ARCH/libopen62541$${_OPEN62541VER}.a


    }

}
}
