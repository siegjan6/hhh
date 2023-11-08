#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T10:47:41
#
#-------------------------------------------------
CONFIG += c++11

QT       += printsupport

QT       -= gui

QT       += xml

TARGET = MDRecipeGrid
TEMPLATE = lib

DEFINES += MDRECIPEGRID_LIBRARY

include (../../pri/MDRecipeGrid.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDUtility.pri)
include(../../pri/MDMultiLanguage.pri)



SOURCES += \
    MDRecipeGrid.cpp \
    RecipeGridDelegate.cpp \
    RecipeGridModel.cpp \
    RecipeGridView.cpp \
    PrintCloneView.cpp \
    ShareTool.cpp \
    GridItems/CheckBoxItem.cpp \
    GridItems/ComboBoxItem.cpp \
    GridItems/GridBaseItem.cpp \
    GridItems/ImageItem.cpp \
    GridItems/RadioButtonItem.cpp \
    FrozenSubViews/AbstractFrozenView.cpp \
    FrozenSubViews/ColumnFrozenView.cpp \
    FrozenSubViews/FixedFrozenView.cpp \
    FrozenSubViews/RowFrozenView.cpp \
    ConfigDialogs/AlignStyleDialog.cpp \
    ConfigDialogs/DeleteRCDialog.cpp \
    ConfigDialogs/FrameDialog.cpp \
    ConfigDialogs/FrozenSetDialog.cpp \
    ConfigDialogs/InsertRCDialog.cpp \
    ConfigDialogs/ItemCfgDialog.cpp \
    ConfigDialogs/PrintConfigDialog.cpp \
    ConfigDialogs/PropertyDialog.cpp \
    ConfigDialogs/ReplaceTextDialog.cpp \
    ConfigDialogs/SetHWDialog.cpp \
    ConfigDialogs/SetRecipeNameDialog.cpp \
    ConfigDialogs/SetDataSourceDialog.cpp \
    ConfigDialogs/MulitItemCfgDialog.cpp \
    ValueMap/ValueMapManager.cpp \
    ValueMap/EditValueMapDialog.cpp \
    ValueMap/ValueMapDelegate.cpp \
    ValueMap/ValueMapModel.cpp \
    ValueMap/ValueMapView.cpp \
    RecipeGridStyle.cpp \
    GridItems/GridComboBox.cpp \
    GridItems/GridCheckBox.cpp \
    GridItems/GridRadioGroup.cpp \
    ConfigDialogs/SetRecipeTemplateDialog.cpp

HEADERS += \
    RecipeGridDelegate.h \
    RecipeGridModel.h \
    RecipeGridView.h \
    PrintCloneView.h \
    ShareTool.h \
    GridItems/CheckBoxItem.h \
    GridItems/ComboBoxItem.h \
    GridItems/GridBaseItem.h \
    GridItems/GridItemDef.h \
    GridItems/ImageItem.h \
    GridItems/RadioButtonItem.h \
    FrozenSubViews/AbstractFrozenView.h \
    FrozenSubViews/ColumnFrozenView.h \
    FrozenSubViews/FixedFrozenView.h \
    FrozenSubViews/FrozenViewDef.h \
    FrozenSubViews/RowFrozenView.h \
    ConfigDialogs/AlignStyleDialog.h \
    ConfigDialogs/ConfigDlgDef.h \
    ConfigDialogs/DeleteRCDialog.h \
    ConfigDialogs/FrameDialog.h \
    ConfigDialogs/FrozenSetDialog.h \
    ConfigDialogs/InsertRCDialog.h \
    ConfigDialogs/ItemCfgDialog.h \
    ConfigDialogs/PrintConfigDialog.h \
    ConfigDialogs/PropertyDialog.h \
    ConfigDialogs/ReplaceTextDialog.h \
    ConfigDialogs/SetHWDialog.h \
    ../../include/MDRecipeGrid/MDRecipeGrid.h \
    ../../include/MDRTDBManagerUI/IMDRTDBManagerUI.h \
    ConfigDialogs/SetRecipeNameDialog.h \
    ConfigDialogs/SetDataSourceDialog.h \
    ConfigDialogs/MulitItemCfgDialog.h \
    ValueMap/ValueMapManager.h \
    ValueMap/EditValueMapDialog.h \
    ValueMap/ValueMapDelegate.h \
    ValueMap/ValueMapModel.h \
    ValueMap/ValueMapView.h \
    RecipeGridStyle.h \
    GridItems/GridComboBox.h \
    GridItems/GridCheckBox.h \
    GridItems/GridRadioGroup.h \
    ConfigDialogs/SetRecipeTemplateDialog.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES +=

RESOURCES += \
    src.qrc

FORMS += \
    ConfigDialogs/AlignStyleDialog.ui \
    ConfigDialogs/DeleteRCDialog.ui \
    ConfigDialogs/FrameDialog.ui \
    ConfigDialogs/FrozenSetDialog.ui \
    ConfigDialogs/InsertRCDialog.ui \
    ConfigDialogs/ItemCfgDialog.ui \
    ConfigDialogs/PrintConfigDialog.ui \
    ConfigDialogs/PropertyDialog.ui \
    ConfigDialogs/ReplaceTextDialog.ui \
    ConfigDialogs/SetHWDialog.ui \
    ConfigDialogs/SetRecipeNameDialog.ui \
    ConfigDialogs/SetDataSourceDialog.ui \
    ConfigDialogs/MulitItemCfgDialog.ui \
    ValueMap/EditValueMapDialog.ui \
    ConfigDialogs/SetRecipeTemplateDialog.ui
