_OPEN62541VER = $$(OPEN62541VER)
!build_pass:message(MDUaClient open62541 VERSION=$${_OPEN62541VER})

INCLUDEPATH += ../../Common/MDUaServer/QUaServer/amalgamation
INCLUDEPATH += ../../Common/MDUaServer/QUaServer/wrapper$${_OPEN62541VER}
INCLUDEPATH += ../../include/3rdparty/mbedtls

LibraryName = Mind.UaClient

DEFINES += UA_ENABLE_HISTORIZING
#DEFINES += UA_DYNAMIC_LINKING UA_DYNAMIC_LINKING_EXPORT
!contains(DEFINES, MDUACLIENT_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

win32:!win32-g++ {
#    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/$${LibraryName}d.lib

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
}else{
#LIBS += -L$$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH) -l$${LibraryName}
LIBS += -L../../bin/release -l$${LibraryName}

win32:!win32-g++ {
#    PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/$${LibraryName}.lib
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
}
}
