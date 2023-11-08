#-------------------------------------------------
#
# Project created by QtCreator 2014-10-31T10:46:00
#
#-------------------------------------------------

QT       += widgets printsupport

TEMPLATE = lib

DEFINES += MDHMIFORM_LIBRARY

include(../../pri/MDHMIForm.pri)

ProjectName = $${LibraryName}
include(../../pri/common.pri)

include(../../pri/MDDrawObj.pri)
include(../../pri/MDDrawVectors.pri)
include(../../pri/MDDrawWidgets.pri)
include(../../pri/MDDrawNode.pri)
include(../../pri/MDDrawSpecial.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDScriptEngine.pri)
#win32{
include(../../pri/MDScriptEditor.pri)
#}
include(../../pri/MDJavaScriptCodeEditor.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDActionSPUI.pri)
include(../../pri/MDUtility.pri)
include(../../pri/MDActionUI.pri)
include(../../pri/MDColorManager.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDEventHelper.pri)


INCLUDEPATH += StudioEnvironment \
StudioEnvironment/ControlPoint \
StudioEnvironment/undo \
StudioEnvironment/config \
RunEnvironment \
../MDScriptObject \

SOURCES += \
    Environment.cpp \
    StudioEnvironment/ControlPoint/ControlPointContainer.cpp \
    StudioEnvironment/ControlPoint/FramePoint.cpp \
    StudioEnvironment/ControlPoint/SelectObjectManager.cpp \
    StudioEnvironment/Studio.cpp \
    StudioEnvironment/ControlPoint/PropertySelectObject.cpp \
    StudioEnvironment/ControlPoint/CustomPoint.cpp \
    RunEnvironment/Run.cpp \
    StudioEnvironment/ObjName.cpp \
    LayerConfigDialog.cpp \
    MDHMIForm.cpp \
    StudioEnvironment/ControlPoint/FormPoint.cpp \
    Tool.cpp \
    StudioEnvironment/CreateDrawObject.cpp \
    StudioEnvironment/ControlPoint/NodePoint.cpp \
    ../MDScriptObject/MDDrawObjWrapper.cpp \
    ../MDScriptObject/MDDrawTextWrapper.cpp \
    ../MDScriptObject/MDDrawVectorWrapper.cpp \
    DrawCreator.cpp \
    ../MDScriptObject/MDDrawCalendarWidgetWrapper.cpp \
    ../MDScriptObject/MDDrawCheckBoxWrapper.cpp \
    ../MDScriptObject/MDDrawComboBoxWrapper.cpp \
    ../MDScriptObject/MDDrawDateTimeEditWrapper.cpp \
    ../MDScriptObject/MDDrawListWrapper.cpp \
    ../MDScriptObject/MDDrawRadioButtonWrapper.cpp \
    ../MDScriptObject/MDDrawTextEditWrapper.cpp \
    ../MDScriptObject/MDDrawWidgetWrapper.cpp \
    StudioEnvironment/FormTypeDialog.cpp \
    ../MDScriptObject/DrawReportWrapper.cpp \
    ../MDScriptObject/DrawCurveWrapper.cpp \
    ../MDScriptObject/MDDrawSpinBoxWrapper.cpp \
    PrintConfigDialog.cpp \
    StudioEnvironment/undo/SelectObjectCommand.cpp \
    StudioEnvironment/undo/FormCommand.cpp \
    StudioEnvironment/undo/ObjectChangedCommand.cpp \
    ../MDScriptObject/HMIFormWrapper.cpp \
    HMIFormData.cpp \
    ../MDScriptObject/DrawTableViewWrapper.cpp \
    ../MDScriptObject/DrawDataArchivesWrapper.cpp \
    StudioEnvironment/config/MDDrawObjectConfig.cpp \
    StudioEnvironment/config/HMIConfigData.cpp \
    ../MDScriptObject/DrawProcDockerWrapper.cpp \
    ../MDScriptObject/DrawPdfViewerWrapper.cpp

HEADERS +=\
    Environment.h \
    StudioEnvironment/ControlPoint/ControlPointContainer.h \
    StudioEnvironment/ControlPoint/FramePoint.h \
    StudioEnvironment/ControlPoint/SelectObjectManager.h \
    StudioEnvironment/Studio.h \
    StudioEnvironment/ControlPoint/PropertySelectObject.h \
    StudioEnvironment/ControlPoint/CustomPoint.h \
    RunEnvironment/Run.h \
    ../../include/interface/HMI/IEnvironment.h \
    ../../include/interface/HMI/IHMIForm.h \
    ../../include/interface/HMI/IRun.h \
    ../../include/interface/HMI/IStudio.h \
    StudioEnvironment/ObjName.h \
    ../../include/interface/HMI/IObjName.h \
    LayerConfigDialog.h \
    HMIFormData.h \
    ../../include/MDHMIForm/MDHMIForm.h \
    StudioEnvironment/ControlPoint/FormPoint.h \
    Tool.h \
    StudioEnvironment/CreateDrawObject.h \
    ../../include/interface/HMI/INodeEdit.h \
    StudioEnvironment/ControlPoint/NodePoint.h \
    ../MDScriptObject/MDDrawObjWrapper.h \
    ../MDScriptObject/MDDrawTextWrapper.h \
    ../MDScriptObject/MDDrawVectorWrapper.h \
    DrawCreator.h \
    ../../include/interface/HMI/IDrawEnvironment.h \
    ../MDScriptObject/MDDrawCalendarWidgetWrapper.h \
    ../MDScriptObject/MDDrawCheckBoxWrapper.h \
    ../MDScriptObject/MDDrawComboBoxWrapper.h \
    ../MDScriptObject/MDDrawDateTimeEditWrapper.h \
    ../MDScriptObject/MDDrawListWrapper.h \
    ../MDScriptObject/MDDrawRadioButtonWrapper.h \
    ../MDScriptObject/MDDrawTextEditWrapper.h \
    ../MDScriptObject/MDDrawWidgetWrapper.h \
    StudioEnvironment/FormTypeDialog.h \
    ../../include/MDHMIForm/MDFormDefine.h \
    ../MDScriptObject/DrawReportWrapper.h \
    ../MDScriptObject/DrawCurveWrapper.h \
    ../MDScriptObject/MDDrawSpinBoxWrapper.h \
    PrintConfigDialog.h \
    StudioEnvironment/undo/SelectObjectCommand.h \
    StudioEnvironment/undo/FormCommand.h \
    StudioEnvironment/undo/ObjectChangedCommand.h \
    ../MDScriptObject/HMIFormWrapper.h \
    ../MDScriptObject/DrawTableViewWrapper.h \
    ../MDScriptObject/DrawDataArchivesWrapper.h \
    StudioEnvironment/config/HMIConfigData.h \
    ../../include/MDHMIForm/MDDrawObjectConfig.h \
    ../MDScriptObject/DrawProcDockerWrapper.h \
    ../MDScriptObject/DrawPdfViewerWrapper.h



FORMS += \
    HMIForm.ui \
    LayerConfigDialog.ui \
    StudioEnvironment/FormTypeDialog.ui \
    PrintConfigDialog.ui

RESOURCES += \
    rs_hmiForm.qrc
