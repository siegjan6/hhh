#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T14:45:48
#
#-------------------------------------------------

QT       += core sql

TARGET = MDUaServer
TEMPLATE = lib



DEFINES += MDUASERVER_STATIC
DEFINES += MDUASERVER_LIBRARY
DEFINES += UA_ENABLE_HISTORIZING

include(../../pri/MDUaServer.pri)
ProjectName = $${LibraryName}
include(../../pri/common0.pri)
CONFIG -= dll
CONFIG += staticlib
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

# ignore some useless warnings on visual studio builds
win32 {
    contains(QMAKE_CC, cl) {
        QMAKE_CXXFLAGS_WARN_ON -= -w14005
        QMAKE_CXXFLAGS += -wd4005
    }
}

#CONFIG+=precompile_header
#PRECOMPILED_HEADER+=QUaServer/wrapper/quabaseobject.h
#PRECOMPILED_HEADER+=QUaServer/wrapper/quaoffnormalalarm.h
_QUASERVER_VER = $$(QUASERVER_VER)
!build_pass:message(MDUaServer QUaServer VERSION=$${_QUASERVER_VER})
HEADERS += ../../include/MDUaServer/MDUaServer_Global.h \
    QUaServer/amalgamation/open62541.h \
    QUaServer/wrapper$${_QUASERVER_VER}/pch_open62541.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quainmemoryhistorizer.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quasqlitehistorizer.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quamultisqlitehistorizer.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quahistorybackend.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quaserver_anex.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quatypesconverter.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quaacknowledgeablecondition.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quaalarmcondition.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quabasedatavariable.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quabaseevent.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quabasemodelchangeevent.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quabaseobject.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quabasevariable.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quacondition.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quaconditionvariable.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quacustomdatatypes.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quadiscretealarm.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quaexclusivelevelalarm.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quaexclusivelimitalarm.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quaexclusivelimitstatemachine.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quafinitestatemachine.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quafinitestatevariable.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quafinitetransitionvariable.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quafolderobject.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quageneralmodelchangeevent.h \
    QUaServer/wrapper$${_QUASERVER_VER}/qualimitalarm.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quanode.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quaoffnormalalarm.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quaoptionset.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quaoptionsetvariable.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quaproperty.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quarefreshendevent.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quarefreshrequiredevent.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quarefreshstartevent.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quaserver.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quastate.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quastatemachine.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quastatevariable.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quasystemevent.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quatransition.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quatransitionevent.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quatransitionvariable.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quatwostatevariable.h \
    QUaServer/wrapper$${_QUASERVER_VER}/quaenum.h

SOURCES += QUaServer/wrapper$${_QUASERVER_VER}/quaacknowledgeablecondition.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quaalarmcondition.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quabasedatavariable.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quabaseevent.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quabasemodelchangeevent.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quabaseobject.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quabasevariable.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quacondition.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quaconditionvariable.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quacustomdatatypes.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quadiscretealarm.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quaexclusivelevelalarm.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quaexclusivelimitalarm.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quaexclusivelimitstatemachine.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quafinitestatemachine.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quafinitestatevariable.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quafinitetransitionvariable.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quafolderobject.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quageneralmodelchangeevent.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quahistorybackend.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/qualimitalarm.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quanode.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quaoffnormalalarm.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quaoptionset.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quaoptionsetvariable.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quaproperty.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quarefreshendevent.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quarefreshrequiredevent.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quarefreshstartevent.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quaserver.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quaserver_anex.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quastate.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quastatemachine.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quastatevariable.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quasystemevent.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quatransition.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quatransitionevent.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quatransitionvariable.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quatwostatevariable.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quatypesconverter.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quainmemoryhistorizer.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quasqlitehistorizer.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quamultisqlitehistorizer.cpp \
    QUaServer/wrapper$${_QUASERVER_VER}/quaenum.cpp


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

    VSCMD_ARG_TGT_ARCH=x64

    win32_PWD=$$win32Path($${PWD})
    win32_DESTDIR=$$win32Path($${OUT_PWD})
    win32_RELATIVEPATH=$$win32Path($${DESTDIR})
    # copy /y C:\MindSCADA\build\build-Make_MindSCADA-Desktop_Qt_5_13_1_MSVC2017_64bit-Release\Common\MDUaServer\..\..bin\release\Mind.UaServer.lib Z:\D\checkout\MindSCADA_CPP\Common\MDUaServer\..\..\libs\\Mind.UaServer.lib
    !build_pass:message(copy /y $$win32_DESTDIR\\..\\..\\$$win32_RELATIVEPATH\\$${TARGET}.lib $$win32_PWD\\..\\..\\libs\\$${VSCMD_ARG_TGT_ARCH}\\$${TARGET}.lib)
    !build_pass:system(copy /y $$win32_DESTDIR\\..\\..\\$$win32_RELATIVEPATH\\$${TARGET}.lib $$win32_PWD\\..\\..\\libs\\$${VSCMD_ARG_TGT_ARCH}\\$${TARGET}.lib)
#    QMAKE_POST_LINK += copy /y $$win32_DESTDIR\\$${TARGET}.lib $$win32_PWD\..\..\libs\\$${VSCMD_ARG_TGT_ARCH}\\$${TARGET}.lib

}

unix {
    ARG_TGT_ARCH=x32
    # cp /home/pi/work/build/build-Make_MindSCADA-QT_5_13_1-rpix32-cm4-Release/Common/MDUaServer/../../bin/release/liblibMind.UaServer.a /media/pi/A31C-5DF8/MindSCADA_CPP/Common/MDUaServer/../../libs/x32/libMind.UaServer.a
    !build_pass:message(cp $${OUT_PWD}/$${DESTDIR}/lib$${TARGET}.a $${PWD}/../../libs/$${ARG_TGT_ARCH}/$${TARGET}.a)
    !build_pass:system(cp $${OUT_PWD}/$${DESTDIR}/lib$${TARGET}.a $${PWD}/../../libs/$${ARG_TGT_ARCH}/$${TARGET}.a)
    # cp /home/pi/work/build/build-Make_MindSCADA-QT_5_13_1-rpix32-cm4-Release/Common/MDUaServer/../../bin/release/liblibMind.UaServer.a /home/pi/work/build/build-Make_MindSCADA-QT_5_13_1-rpix32-cm4-Release/Common/MDUaServer/../../bin/release/libMind.UaServer.a
    !build_pass:message(cp $${OUT_PWD}/$${DESTDIR}/lib$${TARGET}.a $${OUT_PWD}/$${DESTDIR}/$${TARGET}.a)
    !build_pass:system(cp $${OUT_PWD}/$${DESTDIR}/lib$${TARGET}.a $${OUT_PWD}/$${DESTDIR}/$${TARGET}.a)
}
