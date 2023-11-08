#-------------------------------------------------
#
# Project created by QtCreator 2015-06-11T09:57:51
#
#-------------------------------------------------

QT       += core gui widgets

TEMPLATE = lib

DEFINES += MDDRAWNODE_LIBRARY


include(../../pri/MDDrawNode.pri)

ProjectName = $${LibraryName}
include(../../pri/common.pri)

include(../../pri/MDDrawObj.pri)
include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDExpression.pri)
include(../../pri/MDUtility.pri)
include(../../pri/MDColorManager.pri)

SOURCES += MDDrawNode.cpp \
    MDPropertyDrawNode.cpp \
    MDNodeStream.cpp \
    MDDrawLine.cpp \
    MDDrawBezier.cpp \
    MDDrawFoldLine.cpp \
    CLineRegion.cpp \
    MyTools.cpp \
    MDDrawPipe.cpp \
    MDDrawPolygon.cpp \
    undo/UndoDataNode.cpp \
    undo/DrawPipeCommand.cpp \
    MDDrawClosedBezier.cpp

HEADERS += MDPropertyDrawNode.h \
    MDNodeStream.h \
    CLineRegion.h \
    ../../include/MDDrawNode/MDDrawNode.h \
    MyTools.h \
    ../../include/MDDrawNode/MDDrawFoldLine.h \
    ../../include/MDDrawNode/MDDrawLine.h \
    ../../include/MDDrawNode/MDDrawBezier.h \
    ../../include/MDDrawNode/MDDrawPipe.h \
    ../../include/MDDrawNode/MDDrawPolygon.h \
    undo/UndoDataNode.h \
    undo/DrawPipeCommand.h \
    ../../include/MDDrawNode/MDDrawClosedBezier.h
    ../../include/interface/HMI/INodeEdit.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

