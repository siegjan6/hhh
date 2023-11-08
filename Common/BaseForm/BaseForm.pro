#-------------------------------------------------
#
# Project created by QtCreator 2015-01-21T15:23:58
#
#-------------------------------------------------

QT       += widgets

TEMPLATE = lib

DEFINES += BASEFORM_LIBRARY

include(../../pri/MDBaseForm.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)



SOURCES += \
    BaseForm.cpp \
    BaseFormData.cpp

HEADERS += \
    ../../include/MDBaseForm/BaseForm.h \
    BaseFormData.h \

RESOURCES += \
    resource.qrc
