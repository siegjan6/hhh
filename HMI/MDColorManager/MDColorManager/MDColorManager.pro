#-------------------------------------------------
#
# Project created by QtCreator 2015-07-03T15:38:05
#
#-------------------------------------------------
#CONFIG += c++11
CONFIG += c++11
#QT       -= gui
QT += widgets

TARGET = MDColorManager
TEMPLATE=lib

DEFINES += MDCOLORMANAGER_LIBRARY

INCLUDEPATH += lineargradient/
INCLUDEPATH += color_dialog/

include(../../../pri/MDColorManager.pri)
ProjectName = $${LibraryName}
include(../../../pri/common3.pri)

include(../../../pri/MDMultiLanguage3.pri)

SOURCES +=  \
    lineargradient/GradientRamp.cpp \
    lineargradient/GradientRampLump.cpp \
    color_dialog/abstract_widget_list.cpp \
    color_dialog/bound_color_selector.cpp \
    color_dialog/color_delegate.cpp \
    color_dialog/color_dialog.cpp \
    color_dialog/color_list_widget.cpp \
    color_dialog/color_preview.cpp \
    color_dialog/color_selector.cpp \
    color_dialog/color_wheel.cpp \
    color_dialog/gradient_slider.cpp \
    color_dialog/hue_slider.cpp \
    ColorExample.cpp \
    MDBrushManager.cpp \
    MDBrushSelectDialog.cpp \
    MDBrush_Widget.cpp \
    MDPenSelectDialog.cpp \
    MDPenManager.cpp \
    picture_dialog/FillPictureDialog.cpp

HEADERS += \
    ../../../include/MDColorManager/MDColorManager.h \
    ../../../include/MDColorManager/MDBrushManager.h\
    ../../../include/MDColorManager/MDBrushSelectDialog.h \
    ../../../include/MDColorManager/MDPenManager.h\
    ../../../include/MDColorManager/MDPenSelectDialog.h\
    lineargradient/GradientRamp.h \
    lineargradient/GradientRampLump.h \
    lineargradient/define.h \
    color_dialog/abstract_widget_list.hpp \
    color_dialog/Abstract_Widget_List \
    color_dialog/bound_color_selector.hpp \
    color_dialog/Bound_Color_Selector \
    color_dialog/color_delegate.hpp \
    color_dialog/Color_Delegate \
    color_dialog/color_dialog.hpp \
    color_dialog/Color_Dialog \
    color_dialog/color_list_widget.hpp \
    color_dialog/Color_List_Widget \
    color_dialog/colorpicker_global.hpp \
    color_dialog/color_preview.hpp \
    color_dialog/Color_Preview \
    color_dialog/color_selector.hpp \
    color_dialog/Color_Selector \
    color_dialog/color_wheel.hpp \
    color_dialog/Color_Wheel \
    color_dialog/gradient_slider.hpp \
    color_dialog/Gradient_Slider \
    color_dialog/hue_slider.hpp \
    color_dialog/Hue_Slider \
    color_dialog/paint_border.hpp \
    MDColor_Widget.h \
    ColorExample.h \
    MDBrushManager.h \
    MDBrush_Widget.h \
    MDPenSelectDialog.h \
    MDPenManager.h \
    picture_dialog/FillPictureDialog.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    color_dialog/color_dialog.ui \
    picture_dialog/FillPictureDialog.ui

RESOURCES += \
    color_dialog/pattern.qrc

OTHER_FILES += \
    color_dialog/alphaback.png \
    color_dialog/png/fillpictrue.png

