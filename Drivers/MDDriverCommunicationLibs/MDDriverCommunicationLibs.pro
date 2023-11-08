#-------------------------------------------------
#
# Project created by QtCreator 2014-03-17T16:42:03
#
#-------------------------------------------------
CONFIG += c++17
QT       += core
QT       += network
QT       += widgets
QT +=    serialport
QT       -= gui

#TARGET = MDDriverCommunicationLibs
TEMPLATE = lib

DEFINES += MDDRIVERCOMMUNICATIONLIBS_LIBRARY
RELEASE_SEPARATE_DEBUG = $$(CUSTOM_RELEASE_SEPARATE_DEBUG)
win32:!win32-g++ {
equals(RELEASE_SEPARATE_DEBUG, "SEPARATE") {
    CONFIG(debug, debug|release){
        DEFINES += MDKVCLIENT_LIBRARY_MT
        !build_pass:message('MDDriverCommunicationLibs -->>(DEFINES, MDKVCLIENT_LIBRARY_MT)')
    }
    else{
        DEFINES += MDKVCLIENT_LIBRARY_MD
        !build_pass:message('MDDriverCommunicationLibs -->>(DEFINES, MDKVCLIENT_LIBRARY_MD)')
    }
}
else {
    DEFINES += MDKVCLIENT_LIBRARY_MT
}
}
include(../../pri/MDExtSerialPort.pri)
include(../../pri/MDDriverCommunicationLibs.pri)
include(../../pri/MDKVClient.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

DEFINES += HAS_OPCUA
_OPEN62541VER = $$(OPEN62541VER)
!build_pass:message(MDDriverCommunicationLibs open62541 VERSION=$${_OPEN62541VER})
win32{
INCLUDEPATH+= "C:/opensource/open62541.git$$(OPEN62541VER)/deps"
INCLUDEPATH+= "C:/opensource/open62541.git$$(OPEN62541VER)/plugins/include"
INCLUDEPATH+= "C:/opensource/open62541.git$$(OPEN62541VER)/arch"
INCLUDEPATH+= "C:/opensource/open62541.git$$(OPEN62541VER)/include"
INCLUDEPATH+= "C:/opensource/open62541.git/build$$(OPEN62541VER)/src_generated"
INCLUDEPATH+= "C:/opensource/Open62541CppWrapper/Open62541CppWrapper"
INCLUDEPATH+= "C:/opensource/spdlog/include"
}

unix{
INCLUDEPATH+= "/home/pi/work/open62541.git$$(OPEN62541VER)/deps"
INCLUDEPATH+= "/home/pi/work/open62541.git$$(OPEN62541VER)/plugins/include"
INCLUDEPATH+= "/home/pi/work/open62541.git$$(OPEN62541VER)/arch"
INCLUDEPATH+= "/home/pi/work/open62541.git$$(OPEN62541VER)/build/Release/src_generated"
INCLUDEPATH+= "/home/pi/work/open62541.git$$(OPEN62541VER)/include"
INCLUDEPATH+= "/usr/local/include/Open62541CppWrapper"
#INCLUDEPATH+= "/home/pi/work/spdlog/include"
#INCLUDEPATH+= "/usr/local/include/spdlog"
}

CONFIG(debug, debug|release){

    win32:!win32-g++ {
        QMAKE_CXXFLAGS_DEBUG += /MTd
        #PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/$${LibraryName}d.lib

        LIBS += -lws2_32 \
            -liphlpapi \
            -lcrypt32 \
            -ladvapi32 \
            -lmsvcrtd

        # Additional libraries to load
        LIBS += -L$$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH) \
            -lmbedcryptod \
            -lmbedtlsd \
            -lmbedx509d \
            -lopen62541d$$(OPEN62541VER) \
            -lOpen62541CppWrapperd$$(OPEN62541VER)

        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedcryptod.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedtlsd.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedx509d.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/open62541d$$(OPEN62541VER).lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/Open62541CppWrapperd$$(OPEN62541VER).lib
    }

    unix{
        # dynamic link
        LIBS += -L/home/pi/work/Open62541CppWrapper/buildso/Open62541CppWrapper -lOpen62541CppWrapper -Wl,-rpath /home/pi/work/Open62541CppWrapper/buildso/Open62541CppWrapper

        VSCMD_ARG_TGT_ARCH = x32
        LIBS += -L$$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH \
            -lmbedcryptod \
            -lmbedtlsd \
            -lmbedx509d \
            -lopen62541d$$(OPEN62541VER)
#            -lOpen62541CppWrapperd

        PRE_TARGETDEPS += $$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH/libmbedcryptod.a
        PRE_TARGETDEPS += $$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH/libmbedtlsd.a
        PRE_TARGETDEPS += $$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH/libmbedx509d.a
        PRE_TARGETDEPS += $$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH/libopen62541d$$(OPEN62541VER).a
    }
}else{


    win32:!win32-g++ {
        DEBUGPOSTFIX=""
        equals(RELEASE_SEPARATE_DEBUG, "SEPARATE") {
            QMAKE_CXXFLAGS_RELEASE += /MD
            QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO += /MD
            QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO += /FORCE:MULTIPLE
            QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO += /VERBOSE:LIB


            DEBUGPOSTFIX = "/MD"
            !build_pass:message('MDRTDBManagerUI -->>(QMAKE_CXXFLAGS_RELEASE += /MD)')

            LIBS += -lmsvcprt
            LIBS += -L$$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)$${DEBUGPOSTFIX} -lOpen62541CppWrapper$$(OPEN62541VER)
            PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)$${DEBUGPOSTFIX}/Open62541CppWrapper$$(OPEN62541VER).lib
        }
        else {
            QMAKE_CXXFLAGS_RELEASE += /MT
            LIBS += -L$$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH) -lOpen62541CppWrapper$$(OPEN62541VER)
            LIBS += -lmsvcrt
            PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/Open62541CppWrapper$$(OPEN62541VER).lib
        }
        #PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/$${LibraryName}.lib
        #win32-g++: PRE_TARGETDEPS += ../../bin/release/lib$${LibraryName}.a
        #else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += ../../bin/release/$${LibraryName}.lib
        # Additional libraries to load
        LIBS += -lws2_32 \
            -liphlpapi \
            -lcrypt32 \
            -ladvapi32 \
            -lmsvcrt

        LIBS += -L$$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH) \
            -lmbedcrypto \
            -lmbedtls \
            -lmbedx509 \
            -lopen62541$$(OPEN62541VER)

        #PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/crypt32.lib
        #PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/advapi32.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedcrypto.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedtls.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedx509.lib
        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/open62541$$(OPEN62541VER).lib

    #    message(path=$${LibraryName})
    }

    unix{
#        QMAKE_CXXFLAGS_RELEASE += /MT
        # dynamic link
        LIBS += -L/home/pi/work/Open62541CppWrapper/buildso/Open62541CppWrapper -lOpen62541CppWrapper -Wl,-rpath /home/pi/work/Open62541CppWrapper/buildso/Open62541CppWrapper
        # static link
#        LIBS += -L/usr/local/lib \
#                -Wl,-Bstatic -lOpen62541CppWrapper  \
#                -Wl,-Bstatic -lspdlog

        VSCMD_ARG_TGT_ARCH = x32
# case 1
        LIBS += -L$$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH \
            -lmbedcrypto \
            -lmbedtls \
            -lmbedx509 \
            -lopen62541$$(OPEN62541VER)

# case 2
#        LIBS += -L$$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH) \
#            -Wl,-Bstatic -lmbedcrypto \
#            -Wl,-Bstatic -lmbedtls \
#            -Wl,-Bstatic -lmbedx509 \
#            -Wl,-Bstatic -lopen62541 \
#            -Wl,-Bdynamic


        PRE_TARGETDEPS += $$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH/libmbedcrypto.a
        PRE_TARGETDEPS += $$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH/libmbedtls.a
        PRE_TARGETDEPS += $$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH/libmbedx509.a
        PRE_TARGETDEPS += $$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH/libopen62541$$(OPEN62541VER).a
#        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/libOpen62541CppWrapper.a
    }
}

SOURCES += \
    MDCommunicationComponentFactory.cpp \
    MDCommunicationComponent.cpp \
    MDOPCUaCommunicationObject.cpp \
    MDTCPCommunicationObject.cpp \
    MDSerialPortCommunicationObject.cpp \
    MDUDPCommunicationObject.cpp

HEADERS +=\
#    MDCommunicationComponentFactory.h \
    MDCommunicationComponent.h \
    MDOPCUaCommunicationObject.h \
    MDSerialPortCommunicationObject.h \
    MDTCPCommunicationObject.h \
    MDUDPCommunicationObject.h \
    ../../include/MDDriverCommunicationLibs/MDCommunicationComponentFactory.h \
    OPCUaClient.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
