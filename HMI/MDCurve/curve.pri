INCLUDEPATH += $$PWD/Data \
    $$PWD/Data/Core\
    $$PWD/Data/Axis \
    $$PWD/Data/FeatureTag \
    $$PWD/Data/Tools \
    $$PWD/Data/Appearance \
    $$PWD/Data/ReferenceCurve\
    $$PWD/Data/PreviewCurve\
    $$PWD/ConfigForm \
    $$PWD/ConfigForm/Axis \
    $$PWD/ConfigForm/Appearance \
    $$PWD/ConfigForm/Basic \
    $$PWD/ConfigForm/DataSource \
    $$PWD/ConfigForm/DrawMode \
    $$PWD/ConfigForm/Tags \
    $$PWD/ConfigForm/Tools \
    $$PWD/ConfigForm/Curve \
    $$PWD/ConfigForm/UI \
    ../../include/MDColorManager \
    ../../include/interface/HMI \
    ../../include/MDSQLDataBaseProxy \
    ../../include/MDProjectExpert \
    ../../include/MDDBQueryService \
    ../../include/MDVariableArchives \
    ../../include/MDRTDBManagerUI \
    ../../include/MDDataArchivesInterface \
    ../../include/MDUtility\
    ../../include/MDDBQueryConfigure\
    ../../include/MDDBQueryEngine\
    ../../include/MDDBQueryUI


DataPath = $$PWD/Data
ConfigFormPath = $$PWD/ConfigForm

include(../../pri/MDColorManager.pri)
include(../../pri/MDDBQueryService.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDUtility.pri)
include(../../pri/MDDBQueryConfigure.pri)
include(../../pri/MDDBQueryEngine.pri)
include(../../pri/MDDBQueryUI.pri)
include(../../pri/MDSQLDataBaseProxy.pri)

SOURCES += MDCurve.cpp \
    $${DataPath}/Appearance/CAppearance.cpp \
    $${DataPath}/Appearance/CGridRegion.cpp \
    $${DataPath}/Appearance/CHeader.cpp \
    $${DataPath}/Appearance/CWarningLine.cpp \
    $${DataPath}/Tools/CAuxiliary.cpp \
    $${DataPath}/Tools/CDrawMode.cpp \
    $${DataPath}/Tools/CLabelItem.cpp \
    $${DataPath}/Tools/CLabelManager.cpp \
    $${DataPath}/Tools/CTools.cpp \
    $${DataPath}/Tools/CVernier.cpp \
    $${DataPath}/Tools/CVernierWidget.cpp \
    $${DataPath}/Axis/CXAxis.cpp \
    $${DataPath}/Axis/CXAxisManager.cpp \
    $${DataPath}/Axis/CYAxis.cpp \
    $${DataPath}/Axis/CYAxisManager.cpp \
    $${DataPath}/Core/CAbstractAxis.cpp \
    $${DataPath}/Core/CBisectrixLine.cpp \
    $${DataPath}/Core/CDataProperty.cpp \
    $${DataPath}/Core/CDataSource.cpp \
    $${DataPath}/Core/CFind.cpp \
    $${DataPath}/Core/CPoint.cpp \
    $${DataPath}/Core/Data.cpp \
    $${DataPath}/FeatureTag/CFeatureTag.cpp \
    $${DataPath}/FeatureTag/CFeatureTagCommentItem.cpp \
    $${DataPath}/FeatureTag/CRealTimeTagManager.cpp \
    $${DataPath}/FeatureTag/CFeatureTagItem.cpp \
    $${DataPath}/FeatureTag/CFeatureTagLine.cpp \
    $${DataPath}/FeatureTag/CFeatureTagTimeItem.cpp \
    $${DataPath}/FeatureTag/CFeatureTagValueItem.cpp \
    $${DataPath}/FeatureTag/CTagPosition.cpp \
    Data/CCurveObject.cpp \
    Data/CCurveObjectManager.cpp \
    Data/CCurveProperty.cpp \
    Data/CCurveWidget.cpp \
    Data/FeatureTag/CHistoryTagManager.cpp\
    Data/FeatureTag/CRealFTag.cpp \    
    Data/FeatureTag/CTagManager.cpp \
    ConfigForm/Tags/CRealTagSettingDialog.cpp \
    Data/Tools/CToolButton.cpp \
    ConfigForm/Curve/CReferenceCurveSettingForm.cpp \
    ConfigForm/Curve/CReferenceCurvePropertyDialog.cpp \
    Data/ReferenceCurve/CReferenceCurve.cpp \
    Data/ReferenceCurve/CReferenceCurveManager.cpp \
    ConfigForm/Tags/CFeatureTagConfigDialog.cpp \
    Data/PreviewCurve/CPreviewCurve.cpp \
    ConfigForm/Curve/CPreviewCurveSettingForm.cpp \
    Data/PreviewCurve/CPreviewCurveData.cpp \
    ConfigForm/DataSource/CViewFieldDialog.cpp

SOURCES += \
    $${ConfigFormPath}/Axis/CXAxisSettingForm.cpp \
    $${ConfigFormPath}/Axis/CXPropertySettingDialog.cpp \
    $${ConfigFormPath}/Axis/CYAxisSettingForm.cpp \
    $${ConfigFormPath}/Axis/CYPropertySettingDialog.cpp  \
    $${ConfigFormPath}/Appearance/CAppearanceSettingForm.cpp\
    $${ConfigFormPath}/Basic/CBisectrixLineSettingDialog.cpp \
    $${ConfigFormPath}/Basic/CPushButton.cpp \
    $${ConfigFormPath}/DataSource/CVariantDialog.cpp \
    $${ConfigFormPath}/DrawMode/CDrawModeSettingForm.cpp \
    $${ConfigFormPath}/Tags/CFeatureTagPropertySettingDialog.cpp \
    $${ConfigFormPath}/Tags/CFeatureTagSettingForm.cpp \
    $${ConfigFormPath}/Tags/CTagItemSettingDialog.cpp \
    $${ConfigFormPath}/Tools/CPageSettingForm.cpp \
    $${ConfigFormPath}/Tools/CToolsSettingForm.cpp \
    $${ConfigFormPath}/Tools/CVernierSettingForm.cpp \
    $${ConfigFormPath}/MDCurveSettingDialog.cpp \
    $${ConfigFormPath}/Curve/CCurvePropertySettingDialog.cpp \
    $${ConfigFormPath}/Curve/CCurveSettingForm.cpp \
    $${ConfigFormPath}/Curve/CCurveStisticalForm.cpp

HEADERS += \
    $${DataPath}/Appearance/CAppearance.h \
    $${DataPath}/Appearance/CGridRegion.h \
    $${DataPath}/Appearance/CHeader.h \
    $${DataPath}/Appearance/CWarningLine.h \
    $${DataPath}/Tools/CAuxiliary.h \
    $${DataPath}/Tools/CDrawMode.h \
    $${DataPath}/Tools/CLabelItem.h \
    $${DataPath}/Tools/CLabelManager.h \
    $${DataPath}/Tools/CTools.h \
    $${DataPath}/Tools/CVernier.h \
    $${DataPath}/Tools/CVernierWidget.h \
    $${DataPath}/Axis/CXAxis.h \
    $${DataPath}/Axis/CXAxisManager.h \
    $${DataPath}/Axis/CYAxis.h \
    $${DataPath}/Axis/CYAxisManager.h \
    $${DataPath}/Core/CAbstractAxis.h \
    $${DataPath}/Core/CBisectrixLine.h \
    $${DataPath}/Core/CDataProperty.h \
    $${DataPath}/Core/CDataSource.h \
    $${DataPath}/Core/CFind.h \
    $${DataPath}/Core/CPoint.h \
    $${DataPath}/Core/Data.h \
    $${DataPath}/FeatureTag/CFeatureTag.h \
    $${DataPath}/FeatureTag/CFeatureTagCommentItem.h \
    $${DataPath}/FeatureTag/CRealTimeTagManager.h \
    $${DataPath}/FeatureTag/CFeatureTagItem.h \
    $${DataPath}/FeatureTag/CFeatureTagLine.h \
    $${DataPath}/FeatureTag/CFeatureTagTimeItem.h \
    $${DataPath}/FeatureTag/CFeatureTagValueItem.h \
    $${DataPath}/FeatureTag/CTagPosition.h \
    Data/CCurveObject.h \
    Data/CCurveObjectManager.h \
    Data/CCurveProperty.h \
    Data/CCurveWidget.h \
    Data/FeatureTag/CHistoryTagManager.h \
    Data/FeatureTag/CRealFTag.h \   
    Data/FeatureTag/CTagManager.h \
    ConfigForm/Tags/CRealTagSettingDialog.h \
    Data/Tools/CToolButton.h \
    ConfigForm/CTranslator.h \
    ConfigForm/CFormToolFunc.h \
    ConfigForm/Curve/CReferenceCurveSettingForm.h \
    ConfigForm/Curve/CReferenceCurvePropertyDialog.h \
    Data/ReferenceCurve/CReferenceCurve.h \
    Data/ReferenceCurve/CReferenceCurveManager.h \
    ConfigForm/Tags/CFeatureTagConfigDialog.h \
    Data/PreviewCurve/CPreviewCurve.h \
    ConfigForm/Curve/CPreviewCurveSettingForm.h \
    Data/PreviewCurve/CPreviewCurveData.h \
    ConfigForm/DataSource/CViewFieldDialog.h

HEADERS += \
    $${ConfigFormPath}/Axis/CXAxisSettingForm.h \
    $${ConfigFormPath}/Axis/CXPropertySettingDialog.h \
    $${ConfigFormPath}/Axis/CYAxisSettingForm.h \
    $${ConfigFormPath}/Axis/CYPropertySettingDialog.h\
    $${ConfigFormPath}/Appearance/CAppearanceSettingForm.h\
    $${ConfigFormPath}/Basic/CBisectrixLineSettingDialog.h \
    $${ConfigFormPath}/Basic/CPushButton.h \
    $${ConfigFormPath}/DataSource/CVariantDialog.h \
    $${ConfigFormPath}/DrawMode/CDrawModeSettingForm.h \
    $${ConfigFormPath}/Tags/CFeatureTagPropertySettingDialog.h \
    $${ConfigFormPath}/Tags/CFeatureTagSettingForm.h \
    $${ConfigFormPath}/Tags/CTagItemSettingDialog.h \
    $${ConfigFormPath}/Tools/CPageSettingForm.h \
    $${ConfigFormPath}/Tools/CToolsSettingForm.h \
    $${ConfigFormPath}/Tools/CVernierSettingForm.h \
    $${ConfigFormPath}/MDCurveSettingDialog.h  \
    $${ConfigFormPath}/Curve/CCurvePropertySettingDialog.h \
    $${ConfigFormPath}/Curve/CCurveSettingForm.h \
    $${ConfigFormPath}/Curve/CCurveStisticalForm.h

HEADERS += ../../include/MDCurve/MDCurve.h \
    ../../include/interface/HMI/IDrawEnvironment.h

RESOURCES += \
    icons.qrc

FORMS += \   
    $${ConfigFormPath}/UI/MDCurveSettingDialog.ui \
    $${ConfigFormPath}/UI/CAppearanceSettingForm.ui \
    $${ConfigFormPath}/UI/CCurveSettingForm.ui \
    $${ConfigFormPath}/UI/CXAxisSettingForm.ui \
    $${ConfigFormPath}/UI/CYAxisSettingForm.ui \
    $${ConfigFormPath}/UI/CVernierSettingForm.ui \
    $${ConfigFormPath}/UI/CFeatureTagSettingForm.ui \
    $${ConfigFormPath}/UI/CDrawModeSettingForm.ui \
    $${ConfigFormPath}/UI/CToolsSettingForm.ui \
    $${ConfigFormPath}/UI/CCurveStisticalForm.ui \
    $${ConfigFormPath}/UI/CPageSettingForm.ui \
    $${ConfigFormPath}/UI/CBisectrixLineSettingDialog.ui \
    $${ConfigFormPath}/UI/CYPropertySettingDialog.ui \
    $${ConfigFormPath}/UI/CXPropertySettingDialog.ui \
    $${ConfigFormPath}/UI/CCurvePropertySettingDialog.ui \
    $${ConfigFormPath}/UI/CFeatureTagPropertySettingDialog.ui \
    $${ConfigFormPath}/UI/CTagItemSettingDialog.ui \
    $${ConfigFormPath}/UI/CVariantDialog.ui \
    ConfigForm/Tags/CRealTagSettingDialog.ui \
    ConfigForm/Curve/CReferenceCurveSettingForm.ui \
    ConfigForm/Curve/CReferenceCurvePropertyDialog.ui \
    ConfigForm/Tags/CFeatureTagConfigDialog.ui \
    ConfigForm/Curve/CPreviewCurveSettingForm.ui \
    ConfigForm/DataSource/CViewFieldDialog.ui




