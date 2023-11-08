QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mind.SCADA
TEMPLATE = app

DEFINES += QBREAKPAD

ProjectName = Mind.Studio
win32{
QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"
}

#GIT_COMMIT_SHA1 = $$system(git log -n 1 | head -n 1 | cut -d \' \' -f 2)
#!build_pass:message( studio git version hash ($$GIT_COMMIT_SHA1) )

GIT_BRANCH   = $$system(git rev-parse --abbrev-ref HEAD)
GIT_HASH     = $$system(git show --oneline --format=\"%H%n%ci\" -s HEAD)
PC_VERSION = "$${GIT_BRANCH}-$${GIT_HASH}"
!build_pass:message( studio git branch version ($$PC_VERSION) )
DEFINES += PC_VERSION=\"\\\"$$PC_VERSION\\\"\"

include(../../pri/common.pri)
CONFIG -= dll

win32{

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
    CONFIG(debug, debug|release){
    !build_pass:message(copy /y $$win32_DESTDIR\\..\\..\\$$win32_RELATIVEPATH\\$${TARGET}.exe $$win32_DESTDIR\\..\\..\\$$win32_RELATIVEPATH\\MindSCADASvcd.windows.tools.64.exe)
    !build_pass:system(copy /y $$win32_DESTDIR\\..\\..\\$$win32_RELATIVEPATH\\$${TARGET}.exe $$win32_DESTDIR\\..\\..\\$$win32_RELATIVEPATH\\MindSCADASvcd.windows.tools.64.exe)

    }else{
    !build_pass:message(copy /y $$win32_DESTDIR\\..\\..\\$$win32_RELATIVEPATH\\$${TARGET}.exe $$win32_DESTDIR\\..\\..\\$$win32_RELATIVEPATH\\MindSCADASvc.windows.opt.tools.64.exe)
    !build_pass:system(copy /y $$win32_DESTDIR\\..\\..\\$$win32_RELATIVEPATH\\$${TARGET}.exe $$win32_DESTDIR\\..\\..\\$$win32_RELATIVEPATH\\MindSCADASvc.windows.opt.tools.64.exe)

    }
}

include(../../pri/MDUtility.pri)
include(../../pri/MDCommonHelper.pri)
include(../../pri/MDExpression.pri)
include(../../pri/MDDBQueryEngine.pri)
include(../../pri/MDDBConnect.pri)
include(../../pri/MDCommandQueueEngine.pri)
include(../../pri/MDSysLogExpert.pri)
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDMObjFactory.pri)
include(../../pri/MDSwitchFactory.pri)
include(../../pri/MDDBQueryUI.pri)
include(../../pri/MDQJson2SortJson.pri)

include(../../pri/MDActionIDHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDLogHelper.pri)
include(../../pri/MDProjectHelper.pri)
include(../../pri/MDMultiLanguage.pri)

include(../../pri/MDExtSerialPort.pri)
include(../../pri/MDDriverCommunicationLibs.pri)
include(../../pri/MDDriverLibs.pri)
include(../../pri/MDVariableArchives.pri)
# ignore MDRTDBManager replace with MDUaExpert
#include(../../pri/MDRTDBManager.pri)
include(../../pri/MDUaExpert.pri)

include(../../pri/MDScriptEngine.pri)

include(../../pri/MDActionHelper.pri)
include(../../pri/MDEventHelper.pri)
include(../../pri/MDAlarmHelper.pri)
include(../../pri/MDRecipeHelper.pri)

include(../../pri/MDActionConfiger.pri)
include(../../pri/MDEventConfiger.pri)
include(../../pri/MDAccountConfiger.pri)
include(../../pri/MDAccountStrategyConfiger.pri)
include(../../pri/MDPowerGroupConfiger.pri)
include(../../pri/MDSecurityPolicyConfiger.pri)
include(../../pri/MDDataGroup.pri)
include(../../pri/MDDataSource.pri)
include(../../pri/MDDataArchives.pri)
include(../../pri/MDDataArchivesEngine.pri)
include(../../pri/MDAlarmLevelConfiger.pri)
include(../../pri/MDAlarmConditionConfiger.pri)
include(../../pri/MDAlarmConfiger.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDRecipeConfiger.pri)
include(../../pri/MDRecipeExpert.pri)
include(../../pri/MDDBQueryService.pri)
include(../../pri/MDDBQueryConfigure.pri)
include(../../pri/MDSQLDataBaseProxy.pri)
include(../../pri/MDDBQueryEngine.pri)

#开始编译界面库
#---------------------------------------------------------------------
include(../../pri/MDSysLogUI.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDFilterTreePlugin.pri)
include(../../pri/MDDataQueryUI.pri)
include(../../pri/MDObjBatchOperation.pri)

include(../../pri/MDObjectManagerUI.pri)
include(../../pri/MDActionSPUI.pri)
include(../../pri/MDActionUI.pri)

include(../../pri/MDRTDBManagerUI.pri)
include(../../pri/MDDataSelectUI.pri)
include(../../pri/MDDataArchivesUI.pri)

include(../../pri/MDColorManager.pri)
include(../../pri/MDDrawObj.pri)
include(../../pri/MDDrawNode.pri)
include(../../pri/MDCurve.pri)
include(../../pri/MDRecipeGrid.pri)
include(../../pri/MDGauge.pri)
include(../../pri/MDDiagram.pri)
include(../../pri/MDDataGroupGrid.pri)
include(../../pri/MDAlarmGrid.pri)
include(../../pri/MDLogGrid.pri)
include(../../pri/MDReportOutput.pri)
include(../../pri/MDDataArchivesWidget.pri)
include(../../pri/MDDrawSpecial.pri)
include(../../pri/MDDrawVectors.pri)
include(../../pri/MDDrawWidgets.pri)
include(../../pri/MDJavaScriptCodeEditor.pri)
include(../../pri/MDScriptEditor.pri)
include(../../pri/MDHMIForm.pri)
include(../../pri/MDGraphicsStorage.pri)
include(../../pri/MDHMIFramework.pri)

include(../../pri/MDSecurityPolicyExpert.pri)
include(../../pri/MDHistoryExpert.pri)

include(../../pri/MDReportDesigner.pri)
#win32{
include(../../pri/MDScriptUI.pri)
#}
include(../../pri/MDEventUI.pri)
include(../../pri/MDEventActionUI.pri)
include(../../pri/MDAccountUI.pri)
include(../../pri/MDPowerGroupUI.pri)
include(../../pri/MDSecurityPolicyUI.pri)
include(../../pri/MDAlarmLevelUI.pri)
include(../../pri/MDAlarmConditionUI.pri)
include(../../pri/MDAlarmUI.pri)
include(../../pri/MDRuntimeUI.pri)
include(../../pri/MDRecipeUI.pri)

include(../../pri/MDSingleApplication.pri)

unix{
include(../../pri/MDVariableExpert.pri)
include(../../pri/MDActionExpert.pri)
include(../../pri/MDEventExpert.pri)
include(../../pri/MDScriptExpert.pri)
include(../../pri/MDLogExpert.pri)
include(../../pri/MDRedisClient.pri)
include(../../pri/MDDeviceCloudExpert.pri)
#include(../../pri/MDUaServer.pri)

contains(DEFINES, QBREAKPAD){
# link qBreakpad library
include(../../third_libs/breakpad/breakpadsrc.pri)
}

CONFIG(debug, debug|release){
#LIBS += -L/home/pi/work/build/build-Make_MindSCADA-QT_5_13_1-rpix32-Release/bin/debug -llibMind.UaServer

}else{
#LIBS += -L/home/pi/work/build/build-Make_MindSCADA-QT_5_13_1-rpix32-cm4-Release/bin/release -llibMind.UaServer
#LIBS += -L/home/pi/work/Open62541CppWrapper/buildso/Open62541CppWrapper -lOpen62541CppWrapper -llibMind.UaServer

#        VSCMD_ARG_TGT_ARCH=x32
#        LIBS += -L$$PWD/../../libs/$$VSCMD_ARG_TGT_ARCH \

}
}

win32:!win32-g++ {
    LIBS += -lws2_32
}

INCLUDEPATH += config \

SOURCES +=\
    DevProjectItem.cpp \
    DevProjectManager.cpp \
    Main.cpp \
    MainWindow.cpp \
    NewProjectDialog.cpp \
    FormScrollArea.cpp \
    config/ConfigData.cpp \
    config/DockConfig.cpp

HEADERS  += \
    DevProjectItem.h \
    DevProjectManager.h \
    MainWindow.h \
    NewProjectDialog.h \
    FormScrollArea.h \
    ../../version.h \
    config/ConfigData.h \
    config/DockConfig.h \
    ../../include/interface/Studio/IStudioModule.h \
    ../../include/interface/Studio/IStudioInterface.h

FORMS    += MainWindow.ui \
    NewProjectDialog.ui

RESOURCES += \
    Resource.qrc
