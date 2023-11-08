#-------------------------------------------------
#
# Project created by QtCreator 2015-05-26T10:17:34
#
#-------------------------------------------------

QT       += core gui widgets

TEMPLATE = lib

DEFINES += MDGAUGE_LIBRARY

include(../../pri/MDGauge.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)
include (../../pri/MDUtility.pri)

#加载动态库
include(../../pri/MDMultiLanguage.pri)

INCLUDEPATH+= Config\
Draw \
../../include/interface/Studio \

SOURCES += \
    Draw/DrawData.cpp \
    Draw/DrawGauge.cpp \
    Draw/DrawItem.cpp \
    Config/CapDialog.cpp \
    Config/LayerDialog.cpp \
    Config/ModuleSelectDialog.cpp \
    Config/NeedleDialog.cpp \
    Config/ColorDialog.cpp \
    Config/RangeDialog.cpp \
    Config/ScaleTextDialog.cpp \
    Config/ScaleDialog.cpp \
    Config/ScaleBackDialog.cpp \
    Config/BackDialog.cpp \
    Config/HalfBackDialog.cpp \
    Config/LabelDialog.cpp \
    Config/LedDialog.cpp \
    Draw/MDGauge.cpp \
    Config/DesignDialog.cpp \
    Config/StyleDialog.cpp \
    Config/TemperatureDialog.cpp \
    Config/RulerMarkerDialog.cpp \
    Config/RulerNeedleDialog.cpp \
    Config/RulerRangeDialog.cpp \
    Config/RulerScaleDialog.cpp \
    Config/RulerScaleBackDialog.cpp \
    Config/RulerScaleTextDialog.cpp \
    Config/Ccommon.cpp \
    Config/PaintButton.cpp


HEADERS  += \
    Draw/DrawData.h \
    Draw/DrawDefine.h \
    Draw/DrawGauge.h \
    Draw/DrawItem.h  \
    Config/CapDialog.h \
    Config/LayerDialog.h \
    Config/ModuleSelectDialog.h \
    Config/NeedleDialog.h \
    Config/ColorDialog.h \
    Config/RangeDialog.h \
    Config/ScaleTextDialog.h \
    Config/ScaleDialog.h \
    Config/ScaleBackDialog.h \
    Config/BackDialog.h \
    Config/HalfBackDialog.h \
    Config/LabelDialog.h \
    Config/LedDialog.h \
    ../../include/MDGauge/MDGauge.h \
    Config/DesignDialog.h \
    Config/TemperatureDialog.h \
    Config/StyleDialog.h \
    Config/RulerScaleTextDialog.h \
    Config/RulerScaleDialog.h \
    Config/RulerScaleBackDialog.h \
    Config/RulerRangeDialog.h \
    Config/RulerMarkerDialog.h \
    Config/RulerNeedleDialog.h \
    Config/Ccommon.h \
    Config/PaintButton.h \
    Config/CMessage.h


FORMS    += \
    Config/CapDialog.ui \
    Config/LayerDialog.ui \
    Config/ModuleSelectDialog.ui \
    Config/NeedleDialog.ui \
    Config/ColorDialog.ui \
    Config/RangeDialog.ui \
    Config/ScaleTextDialog.ui \
    Config/ScaleDialog.ui \
    Config/ScaleBackDialog.ui \
    Config/BackDialog.ui \
    Config/HalfBackDialog.ui \
    Config/LabelDialog.ui \
    Config/LedDialog.ui \
    Config/DesignDialog.ui \
    Config/StyleDialog.ui \
    Config/TemperatureDialog.ui \
    Config/RulerMarkerDialog.ui \
    Config/RulerNeedleDialog.ui \
    Config/RulerRangeDialog.ui \
    Config/RulerScaleDialog.ui \
    Config/RulerScaleBackDialog.ui \
    Config/RulerScaleTextDialog.ui

RESOURCES += \
    imagebmp.qrc \
    led.qrc
