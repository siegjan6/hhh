#-------------------------------------------------
#
# Project created by QtCreator 2014-10-31T10:24:08
#
#-------------------------------------------------

QT       += core gui widgets

TEMPLATE = lib

DEFINES += MDDRAWVECTORS_LIBRARY

include(../../pri/MDDrawVectors.pri)

ProjectName = $${LibraryName}
include(../../pri/common.pri)

include(../../pri/MDDrawObj.pri)
include(../../pri/MDUtility.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDColorManager.pri)

INCLUDEPATH += undo \

SOURCES += \
    MDDrawRect.cpp \
    MDDrawText.cpp \
    property/MDPropertyDrawText.cpp \
    DrawRectData.cpp \
    DrawTextData.cpp \
    DrawArc/DrawBasicArcData.cpp \
    DrawArc/MDDrawBasicArc.cpp \
    DrawArc/MDDrawArc.cpp \
    DrawArc/MDDrawSector.cpp \
    DrawArc/MDDrawShear.cpp \
    DrawEllipse/MDDrawEllipse.cpp \
    DrawArc/MDPropertyDrawBasicArc.cpp \
    property/PropertyDrawRect.cpp \
    DrawText/FormatDialog.cpp \
    undo/UndoDataRect.cpp \
    undo/DrawTextCommand.cpp \
    undo/DrawBasicArcCommand.cpp \
    undo/UndoDataBasicArc.cpp \
    DrawButton/MDDrawButton.cpp

HEADERS +=\
    ../../include/MDDrawVectors/MDDrawRect.h \
    ../../include/MDDrawVectors/MDDrawText.h \
    DrawRectData.h \
    DrawTextData.h \
    ../../include/MDDrawVectors/MDPropertyDrawText.h \
    DrawArc/DrawBasicArcData.h \
    ../../include/MDDrawVectors/MDDrawArc.h \
    ../../include/MDDrawVectors/MDDrawBasicArc.h \
    ../../include/MDDrawVectors/MDDrawSector.h \
    ../../include/MDDrawVectors/MDDrawShear.h \
    ../../include/MDDrawVectors/MDDrawEllipse.h \
    ../../include/MDDrawVectors/MDPropertyDrawBasicArc.h \
    property/PropertyDrawRect.h \
    DrawText/FormatDialog.h \
    undo/UndoDataRect.h \
    undo/DrawTextCommand.h \
    undo/DrawBasicArcCommand.h \
    undo/UndoDataBasicArc.h \
    ../../include/MDDrawVectors/MDDrawButton.h

FORMS += \
    DrawText/FormatDialog.ui
