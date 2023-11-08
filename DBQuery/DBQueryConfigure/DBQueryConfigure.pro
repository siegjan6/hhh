QT       += core xml
QT       -=  gui

TEMPLATE = lib
TARGET = DBQueryConfigure

DEFINES += DBQUERYCONFIGURE_LIBRARY
include(../../pri/MDDBQueryConfigure.pri)
ProjectName =$${LibraryName}
include(../../pri/MDSwitchFactory.pri)
include(../../pri/common.pri)

SOURCES += \
    ViewVariableWriter.cpp \
    DataSourcesWriter.cpp \
    RuntimeViewVariableReader.cpp \
    RuntimeViewVariableWriter.cpp \
    ViewWriter.cpp \
    FilterOperator.cpp \
    ViewOperator.cpp \
    XMLDataSourcesStorage.cpp \
    JsonDataSourcesStorage.cpp \
    DataSourcesStorage.cpp \
    ViewStorage.cpp \
    XMLViewStorage.cpp \
    JsonViewStorage.cpp \
    ViewVariableStorage.cpp \
    XMLViewVariableStorage.cpp \
    JsonViewVariableStorage.cpp \
    RuntimeViewVarStorage.cpp \
    XMLRuntimeViewVarStorage.cpp \
    JsonRuntimeViewVarStorage.cpp \
    utils.cpp \
    GlobalWrapper.cpp \
    ViewVariableOperator.cpp

HEADERS += \
    ../../include/MDDBQueryConfigure/MDDBQueryConfigure.h \
    DataSourcesStorage.h \
    XMLDataSourcesStorage.h \
    JsonDataSourcesStorage.h \
    ViewStorage.h \
    XMLViewStorage.h \
    JsonViewStorage.h \
    ViewVariableStorage.h \
    XMLViewVariableStorage.h \
    JsonViewVariableStorage.h \
    RuntimeViewVarStorage.h \
    XMLRuntimeViewVarStorage.h \
    JsonRuntimeViewVarStorage.h \
    utils.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

