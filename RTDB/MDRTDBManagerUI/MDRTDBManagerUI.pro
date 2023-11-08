#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T13:41:44
#
#-------------------------------------------------

QT       += widgets
QT       += xml
TARGET = MDRTDBManagerUI
TEMPLATE = lib

DEFINES += MDRTDBMANAGERUI_LIBRARY
DEFINES += MDSELECTVARIABLEWIDGETEXPERT_LIBRARY

RELEASE_SEPARATE_DEBUG = $$(CUSTOM_RELEASE_SEPARATE_DEBUG)
win32:!win32-g++ {
equals(RELEASE_SEPARATE_DEBUG, "SEPARATE") {
    CONFIG(debug, debug|release){
        DEFINES += MDKVCLIENT_LIBRARY_MT
        !build_pass:message('MDRTDBManagerUI -->>(DEFINES, MDKVCLIENT_LIBRARY_MT)')
    }
    else{
        DEFINES += MDKVCLIENT_LIBRARY_MD
        !build_pass:message('MDRTDBManagerUI -->>(DEFINES, MDKVCLIENT_LIBRARY_MD)')
    }
}
else {
    DEFINES += MDKVCLIENT_LIBRARY_MT
}
}

include(../../pri/MDActionIDHelper.pri)
include(../../pri/MDActionSPUI.pri)
include(../../pri/MDEventHelper.pri)
include(../../pri/MDVariableArchives.pri)
include(../../pri/MDDriverLibs.pri)
include(../../pri/MDExpression.pri)
include(../../pri/MDSecurityPolicyHelper.pri)

# ignore MDRTDBManager replace with MDUaExpert
#include(../../pri/MDRTDBManager.pri)
include(../../pri/MDUaExpert.pri)
include(../../pri/MDKVClient.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDActionSPUI.pri)

include(../../pri/MDRTDBManagerUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDObjBatchOperation.pri)

DEFINES += HAS_OPCUA
_OPEN62541VER = $$(OPEN62541VER)
!build_pass:message(MDRTDBManagerUI open62541 VERSION=$${_OPEN62541VER})
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
INCLUDEPATH+= "/home/pi/work/open62541.git$$(OPEN62541VER)/include"
INCLUDEPATH+= "/home/pi/work/open62541.git$$(OPEN62541VER)/build/Release/src_generated"
INCLUDEPATH+= "/home/pi/work/Open62541CppWrapper$$(OPEN62541VER)/Open62541CppWrapper"
INCLUDEPATH+= "/home/pi/work/spdlog/include"
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
        VSCMD_ARG_TGT_ARCH=x32
        LIBS += -L$$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH \
            -lmbedcryptod \
            -lmbedtlsd \
            -lmbedx509d \
            -lopen62541d$$(OPEN62541VER)
#            -lOpen62541CppWrapperd

        LIBS += -L/home/pi/work/Open62541CppWrapper/buildso/Open62541CppWrapper -lOpen62541CppWrapper -Wl,-rpath /home/pi/work/Open62541CppWrapper/buildso/Open62541CppWrapper

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
            -ladvapi32

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
        VSCMD_ARG_TGT_ARCH=x32
        LIBS += -L$$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH \
            -lmbedcrypto \
            -lmbedtls \
            -lmbedx509 \
            -lopen62541$$(OPEN62541VER)
#            \
#            -lOpen62541CppWrapper

        LIBS += -L/home/pi/work/Open62541CppWrapper/buildso/Open62541CppWrapper -lOpen62541CppWrapper -Wl,-rpath /home/pi/work/Open62541CppWrapper/buildso/Open62541CppWrapper

#        PRE_TARGETDEPS += $$PWD/../../libs/$$(VSCMD_ARG_TGT_ARCH)/libOpen62541CppWrapper.a

        PRE_TARGETDEPS += $$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH/libmbedcrypto.a
        PRE_TARGETDEPS += $$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH/libmbedtls.a
        PRE_TARGETDEPS += $$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH/libmbedx509.a
        PRE_TARGETDEPS += $$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH/libopen62541$$(OPEN62541VER).a
    }
}


SOURCES += MDRTDBManagerUI.cpp \
    MDRTDBVariablesWindow.cpp \
    MDRTDBManagerUIFactory.cpp \
    MDRTDBDevicesWindow.cpp \
    AddVarGroupDialog.cpp \
    VariableAlarmDialog.cpp \
    VariableGeneralDialog.cpp \
    VariableInitializeDialog.cpp \
    VariableOtherDialog.cpp \
    VariableDataConversionDialog.cpp \
    VariablePropertyDialog.cpp \
    VariableWriteProtectDialog.cpp \
    AddDeviceDialog.cpp \
    MDDriverConfigDialog.cpp \
    SelectVariableDialog.cpp \
    MDSelectVariableWidget.cpp \
    MDRTDBRuntimeUI.cpp \
    MDRTDBVariablesWidget.cpp \
    VariableWriteValueDialog.cpp \
    VariableArchivesDialog.cpp \
    MDRTDBDevicesWidget.cpp \
    MDRTDBRuntimeDialog.cpp \
    VariableSecurityPolicyDialog.cpp \
    DatablockMonitorDialog.cpp \
    MDRTDBBatchChangeVariable.cpp \
    VariablesBatchChangeDialog.cpp \
    MDRTDBVariableProperty.cpp \
    MDSelectAddressDialog.cpp \
    MDVariableGroupProperty.cpp \
    CustomEvent.cpp \
    TreeViewEx.cpp


HEADERS += MDRTDBManagerUI.h\
    MDRTDBVariablesWindow.h \
    ../../include/MDRTDBManagerUI/IMDRTDBManagerUI.h \
    ../../include/MDRTDBManagerUI/mdrtdbmanagerui_global.h \
    ../../include/MDRTDBManagerUI/MDRTDBManagerUIFactory.h \
    MDRTDBDevicesWindow.h \
    AddVarGroupDialog.h \
    OPCUaClient.h \
    VariableAlarmDialog.h \
    VariableGeneralDialog.h \
    VariableInitializeDialog.h \
    VariableOtherDialog.h \
    VariableDataConversionDialog.h \
    VariablePropertyDialog.h \
    VariableWriteProtectDialog.h \
    AddDeviceDialog.h \
    MDDriverConfigDialog.h \
    SelectVariableDialog.h \
    ../../include/MDRTDBManagerUI/MDSelectVariableWidget.h \
    ../../include/MDRTDBManagerUI/IMDRTDBRuntimeUI.h \
    ../../include/MDMultiLanguage/MDMultiLanguage.h \
    MDRTDBRuntimeUI.h \
    MDRTDBVariablesWidget.h \
    VariableWriteValueDialog.h \
    VariableArchivesDialog.h \
    MDRTDBDevicesWidget.h \
    MDRTDBRuntimeDialog.h \
    VariableSecurityPolicyDialog.h \
    DatablockMonitorDialog.h \
    MDRTDBBatchChangeVariable.h \
    VariablesBatchChangeDialog.h \
    MDRTDBVariableProperty.h \
    MDSelectAddressDialog.h \
    MDVariableGroupProperty.h \
    CustomEvent.h \
    TreeViewEx.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    images/open.png

RESOURCES += \
    resource.qrc

FORMS += \
    AddVarGroupDialog.ui \
    VariableAlarmDialog.ui \
    VariableGeneralDialog.ui \
    VariableInitializeDialog.ui \
    VariableOtherDialog.ui \
    VariableDataConversionDialog.ui \
    VariableWriteProtectDialog.ui \
    AddDeviceDialog.ui \
    MDDriverConfigDialog.ui \
    SelectVariableDialog.ui \
    SelectVariableWidget.ui \
    MDRTDBRuntimeWidget.ui \
    VariableWriteValueDialog.ui \
    VariableArchivesDialog.ui \
    VariableSecurityPolicyDialogui.ui \
    DatablockMonitorDialog.ui \
    VariablesBatchChangeDialog.ui \
    SelectAddressDialog.ui
