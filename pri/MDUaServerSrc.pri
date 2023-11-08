QT     += core
CONFIG += c++11
CONFIG -= flat

INCLUDEPATH += ../../Common/MDUaServer/QUaServer/amalgamation
INCLUDEPATH += ../../Common/MDUaServer/QUaServer/wrapper
INCLUDEPATH += ../../include/3rdparty/mbedtls
DEPENDPATH += ../../Common/MDUaServer/QUaServer/wrapper

DEFINES += ua_events
DEFINES += ua_alarms_conditions
DEFINES += ua_historizing
DEFINES += ua_namespace_full

CONFIG(debug, debug|release){

    win32:!win32-g++ {

            LIBS += -lws2_32 \
                -liphlpapi \
                -lcrypt32 \
                -ladvapi32

            # Additional libraries to load
            LIBS += -L$$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH) \
                -lopen62541d \
                -lmbedcryptod \
                -lmbedtlsd \
                -lmbedx509d

            PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedcryptod.lib
            PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedtlsd.lib
            PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedx509d.lib
            PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/open62541d.lib
        }
    }
    else {

    win32:!win32-g++ {
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
            -lopen62541

        #PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/crypt32.lib
        #PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/advapi32.lib
        PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedcrypto.lib
        PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedtls.lib
        PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/mbedx509.lib
        PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/open62541.lib

    #    message(path=$${LibraryName})
    }
}


UAWRAPPERPATH = ../../Common/MDUaServer/QUaServer/wrapper

# ignore some useless warnings
win32 {
    QMAKE_CXXFLAGS_WARN_ON -= -w14005
    QMAKE_CXXFLAGS += -wd4005
}

SOURCES += \
    $$UAWRAPPERPATH/quaserver.cpp \
    $$UAWRAPPERPATH/quaserver_anex.cpp \
    $$UAWRAPPERPATH/quanode.cpp \
    $$UAWRAPPERPATH/quabasevariable.cpp \
    $$UAWRAPPERPATH/quaproperty.cpp \
    $$UAWRAPPERPATH/quabasedatavariable.cpp \
    $$UAWRAPPERPATH/quabaseobject.cpp \
    $$UAWRAPPERPATH/quafolderobject.cpp \
    $$UAWRAPPERPATH/quacustomdatatypes.cpp
#    $$UAWRAPPERPATH/quaenum.cpp

ua_events || ua_alarms_conditions {
    SOURCES += \
    $$UAWRAPPERPATH/quabaseevent.cpp \
    $$UAWRAPPERPATH/quabasemodelchangeevent.cpp \
    $$UAWRAPPERPATH/quageneralmodelchangeevent.cpp \
    $$UAWRAPPERPATH/quasystemevent.cpp
}

ua_alarms_conditions {
    SOURCES += \
    $$UAWRAPPERPATH/quaconditionvariable.cpp \
    $$UAWRAPPERPATH/quastatevariable.cpp \
    $$UAWRAPPERPATH/quafinitestatevariable.cpp \
    $$UAWRAPPERPATH/quatwostatevariable.cpp \
    $$UAWRAPPERPATH/quatransitionvariable.cpp \
    $$UAWRAPPERPATH/quafinitetransitionvariable.cpp \
    $$UAWRAPPERPATH/quastatemachine.cpp \
    $$UAWRAPPERPATH/quafinitestatemachine.cpp \
    $$UAWRAPPERPATH/quastate.cpp \
    $$UAWRAPPERPATH/quatransition.cpp \
    $$UAWRAPPERPATH/quaexclusivelimitstatemachine.cpp \
    $$UAWRAPPERPATH/quacondition.cpp \
    $$UAWRAPPERPATH/quaacknowledgeablecondition.cpp \
    $$UAWRAPPERPATH/quaalarmcondition.cpp \
    $$UAWRAPPERPATH/quadiscretealarm.cpp \
    $$UAWRAPPERPATH/quaoffnormalalarm.cpp \
    $$UAWRAPPERPATH/qualimitalarm.cpp \
    $$UAWRAPPERPATH/quaexclusivelimitalarm.cpp \
    $$UAWRAPPERPATH/quaexclusivelevelalarm.cpp \
    $$UAWRAPPERPATH/quarefreshstartevent.cpp \
    $$UAWRAPPERPATH/quarefreshendevent.cpp \
    $$UAWRAPPERPATH/quarefreshrequiredevent.cpp \
    $$UAWRAPPERPATH/quatransitionevent.cpp
}

ua_historizing {
    SOURCES += \
    $$UAWRAPPERPATH/quahistorybackend.cpp
}

ua_namespace_full || ua_events || ua_alarms_conditions {
    SOURCES += \
    $$UAWRAPPERPATH/quaoptionset.cpp \
    $$UAWRAPPERPATH/quaoptionsetvariable.cpp
}

SOURCES += \
    $$UAWRAPPERPATH/quatypesconverter.cpp

HEADERS += \
    $$UAWRAPPERPATH/quaserver.h \
    $$UAWRAPPERPATH/quaserver_anex.h \
    $$UAWRAPPERPATH/quanode.h \
    $$UAWRAPPERPATH/quabasevariable.h \
    $$UAWRAPPERPATH/quaproperty.h \
    $$UAWRAPPERPATH/quabasedatavariable.h \
    $$UAWRAPPERPATH/quabaseobject.h \
    $$UAWRAPPERPATH/quafolderobject.h \
    $$UAWRAPPERPATH/quacustomdatatypes.h
#    $$UAWRAPPERPATH/quaenum.h

ua_events || ua_alarms_conditions {
    HEADERS += \
    $$UAWRAPPERPATH/quabaseevent.h \
    $$UAWRAPPERPATH/quabasemodelchangeevent.h \
    $$UAWRAPPERPATH/quageneralmodelchangeevent.h \
    $$UAWRAPPERPATH/quasystemevent.h
}

ua_alarms_conditions {
    HEADERS += \
    $$UAWRAPPERPATH/quaconditionvariable.h \
    $$UAWRAPPERPATH/quastatevariable.h \
    $$UAWRAPPERPATH/quafinitestatevariable.h \
    $$UAWRAPPERPATH/quatwostatevariable.h \
    $$UAWRAPPERPATH/quatransitionvariable.h \
    $$UAWRAPPERPATH/quafinitetransitionvariable.h \
    $$UAWRAPPERPATH/quastatemachine.h \
    $$UAWRAPPERPATH/quafinitestatemachine.h \
    $$UAWRAPPERPATH/quastate.h \
    $$UAWRAPPERPATH/quatransition.h \
    $$UAWRAPPERPATH/quaexclusivelimitstatemachine.h \
    $$UAWRAPPERPATH/quacondition.h \
    $$UAWRAPPERPATH/quaacknowledgeablecondition.h \
    $$UAWRAPPERPATH/quaalarmcondition.h \
    $$UAWRAPPERPATH/quadiscretealarm.h \
    $$UAWRAPPERPATH/quaoffnormalalarm.h \
    $$UAWRAPPERPATH/qualimitalarm.h \
    $$UAWRAPPERPATH/quaexclusivelimitalarm.h \
    $$UAWRAPPERPATH/quaexclusivelevelalarm.h \
    $$UAWRAPPERPATH/quarefreshstartevent.h \
    $$UAWRAPPERPATH/quarefreshendevent.h \
    $$UAWRAPPERPATH/quarefreshrequiredevent.h \
    $$UAWRAPPERPATH/quatransitionevent.h
}

ua_historizing {
    HEADERS += \
    $$UAWRAPPERPATH/quahistorybackend.h
}

ua_namespace_full || ua_events || ua_alarms_conditions {
    HEADERS += \
    $$UAWRAPPERPATH/quaoptionset.h \
    $$UAWRAPPERPATH/quaoptionsetvariable.h
}

HEADERS += \
    $$UAWRAPPERPATH/quatypesconverter.h

DISTFILES += \
    $$UAWRAPPERPATH/QUaServer \
    $$UAWRAPPERPATH/QUaNode \
    $$UAWRAPPERPATH/QUaBaseVariable \
    $$UAWRAPPERPATH/QUaProperty \
    $$UAWRAPPERPATH/QUaBaseDataVariable \
    $$UAWRAPPERPATH/QUaBaseObject \
    $$UAWRAPPERPATH/QUaFolderObject \
    $$UAWRAPPERPATH/QUaCustomDataTypes

ua_events || ua_alarms_conditions {
    DISTFILES += \
    $$UAWRAPPERPATH/QUaBaseEvent \
    $$UAWRAPPERPATH/QUaBaseModelChangeEvent \
    $$UAWRAPPERPATH/QUaGeneralModelChangeEvent \
    $$UAWRAPPERPATH/QUaSystemEvent
}

ua_alarms_conditions {
    DISTFILES += \
    $$UAWRAPPERPATH/QUaConditionVariable \
    $$UAWRAPPERPATH/QUaStateVariable \
    $$UAWRAPPERPATH/QUaFiniteStateVariable \
    $$UAWRAPPERPATH/QUaTwoStateVariable \
    $$UAWRAPPERPATH/QUaTransitionVariable \
    $$UAWRAPPERPATH/QUaFiniteTransitionVariable \
    $$UAWRAPPERPATH/QUaStateMachine \
    $$UAWRAPPERPATH/QUaFiniteStateMachine \
    $$UAWRAPPERPATH/QUaState \
    $$UAWRAPPERPATH/QUaTransition \
    $$UAWRAPPERPATH/QUaExclusiveLimitStateMachine \
    $$UAWRAPPERPATH/QUaCondition \
    $$UAWRAPPERPATH/QUaAcknowledgeableCondition \
    $$UAWRAPPERPATH/QUaAlarmCondition \
    $$UAWRAPPERPATH/QUaDiscreteAlarm \
    $$UAWRAPPERPATH/QUaOffNormalAlarm \
    $$UAWRAPPERPATH/QUaLimitAlarm \
    $$UAWRAPPERPATH/QUaExclusiveLimitAlarm \
    $$UAWRAPPERPATH/QUaExclusiveLevelAlarm \
    $$UAWRAPPERPATH/QUaRefreshStartEvent \
    $$UAWRAPPERPATH/QUaRefreshEndEvent \
    $$UAWRAPPERPATH/QUaRefreshRequiredEvent \
    $$UAWRAPPERPATH/QUaTransitionEvent
}

ua_historizing {
    DISTFILES += \
    $$UAWRAPPERPATH/QUaHistoryBackend
}

ua_namespace_full || ua_events || ua_alarms_conditions {
    DISTFILES += \
    $$UAWRAPPERPATH/QUaOptionSet \
    $$UAWRAPPERPATH/QUaOptionSetVariable
}

DISTFILES += \
    $$UAWRAPPERPATH/QUaTypesConverter
