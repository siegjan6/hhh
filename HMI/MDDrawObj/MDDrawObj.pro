#-------------------------------------------------
#
# Project created by QtCreator 2014-10-31T09:27:08
#
#-------------------------------------------------

QT       += core gui widgets

TEMPLATE = lib

DEFINES += MDDRAWOBJ_LIBRARY

include(../../pri/MDDrawObj.pri)

ProjectName = $${LibraryName}

include(../../pri/common.pri)

include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDExpression.pri)
include(../../pri/MDActionUI.pri)
include(../../pri/MDColorManager.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDUtility.pri)
include(../../pri/MDRTDBManagerUI.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDEventHelper.pri)

INCLUDEPATH += Var \
DrawObject \
DrawObject/DrawMulti \
property \
propertyObj \
module/dynamicColor \
undo \

SOURCES += \
    property/BindVarDialog.cpp \
    DrawObject/DrawData.cpp \
    DrawObject/MDDrawObj.cpp \
    DrawObject/MDDrawVector.cpp \
    MDCalculation.cpp \
    DrawObject/DrawMulti/MDDrawGroup.cpp \
    DrawObject/DrawObjData.cpp \
    DrawObject/DrawVectorData.cpp \
    DrawObject/MDDrawWidget.cpp \
    DrawObject/MDDrawWidgetEvent.cpp \
    DrawObject/DrawMulti/DrawGroupData.cpp \
    DrawObject/DrawWidgetData.cpp \
    property/CustomPropertyDialog.cpp \
    property/InputPropertyNameDialog.cpp \
    DrawObject/MDCustomProperty.cpp \
    MDHMIActionManager.cpp \
    Var/InputVariableDialog.cpp \
    module/dynamicColor/DynamicColor.cpp \
    module/dynamicColor/DynamicColorDialog.cpp \
    propertyObj/MDPropertyDrawGroup.cpp \
    propertyObj/MDPropertyDrawObj.cpp \
    propertyObj/MDPropertyDrawVector.cpp \
    property/FillDialog.cpp \
    DrawObject/MDFill.cpp \
    undo/MDDrawObjCommand.cpp \
    undo/MDUndoData.cpp \
    Var/MDExpInfo.cpp \
    Var/MDParameter.cpp \
    DrawObject/DrawMulti/MDDrawCombine.cpp \
    propertyObj/PropertyDrawCombine.cpp \
    DrawObject/DrawMulti/DrawCombineData.cpp \
    DrawObject/DrawMulti/DrawMultiData.cpp \
    DrawObject/DrawMulti/MDDrawMulti.cpp \
    DrawObject/MDDrawTool.cpp


HEADERS +=\
    ../../include/MDDrawObj/DrawObjDefine.h \
    property/PropertyDrawVector.h \
    ../../include/interface/MDDrawObj/ICustomEdit.h \
    property/BindVarDialog.h \
    ../../include/MDDrawObj/MDPropertyDrawObj.h \
    ../../include/MDDrawObj/MDPropertyDrawVector.h \
    ../../include/MDDrawObj/MDDrawObj.h \
    ../../include/MDDrawObj/MDDrawVector.h \
    ../../include/MDDrawObj/MDCalculation.h \
    ../../include/MDDrawObj/MDDrawGroup.h \
    ../../include/interface/HMI/IDrawData.h \
    DrawObject/DrawData.h \
    DrawObject/DrawMulti/DrawGroupData.h \
    DrawObject/DrawObjData.h \
    DrawObject/DrawVectorData.h \
    ../../include/MDDrawObj/MDDrawWidget.h \
    DrawObject/DrawWidgetData.h \
    ../../include/MDDrawObj/MDDrawWidgetEvent.h \
    property/CustomPropertyDialog.h \
    property/InputPropertyNameDialog.h \
    ../../include/MDDrawObj/MDCustomProperty.h \
    ../../include/MDDrawObj/MDPropertyDrawGroup.h \
    ../../include/MDDrawObj/MDHMIActionManager.h \
    Var/InputVariableDialog.h \
    module/dynamicColor/DynamicColor.h \
    module/dynamicColor/DynamicColorDialog.h \
    property/FillDialog.h \
    ../../include/MDDrawObj/MDFill.h \
    ../../include/MDDrawObj/MDDrawObjCommand.h \
    ../../include/MDDrawObj/MDUndoData.h \
    ../../include/interface/HMI/IVarObject.h \
    ../../include/MDDrawObj/MDExpInfo.h \
    ../../include/MDDrawObj/MDParameter.h \
    DrawObject/DrawMulti/DrawCombineData.h \
    ../../include/MDDrawObj/MDDrawCombine.h \
    propertyObj/PropertyDrawCombine.h \
    DrawObject/DrawMulti/DrawMultiData.h \
    ../../include/MDDrawObj/MDDrawMulti.h \
    ../../include/MDDrawObj/MDDrawTool.h


FORMS += \
    property/BindVarDialog.ui \
    property/CustomPropertyDialog.ui \
    property/InputPropertyNameDialog.ui \
    Var/InputVariableDialog.ui \
    module/dynamicColor/DynamicColorDialog.ui \
    property/FillDialog.ui







