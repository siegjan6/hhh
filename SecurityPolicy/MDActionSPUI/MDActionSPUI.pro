#-------------------------------------------------
#
# Project created by QtCreator 2015-08-12T11:18:59
#
#-------------------------------------------------

QT       += widgets

TARGET = MDActionSPUI
TEMPLATE = lib

DEFINES += MDACTIONSPUI_LIBRARY
#-------------------------------------------------
include(../../pri/MDActionIDHelper.pri)
include(../../pri/MDSecurityPolicyHelper.pri)
include(../../pri/MDPowerGroupConfiger.pri)
include(../../pri/MDObjectManagerUI.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDBaseForm.pri)
#-------------------------------------------------
include(../../pri/MDActionSPUI.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

SOURCES += MDActionSPUI.cpp \
    MDActionSPUIImp.cpp \
    MDActionSPListDialog.cpp \
    MDActionSPDialog.cpp \
    MDPowerGroupTreeWindowForActionSPUI.cpp \
    MDActionSPListDialogParaImp.cpp \
    MDActionSPDialogAsModal.cpp

HEADERS += MDActionSPUIImp.h\
    ../../include/MDActionSPUI/MDActionSPUI_Global.h \
    ../../include/MDActionSPUI/MDActionSPUI.h \
    MDActionSPDialog.h \
    ../../include/MDActionSPUI/MDActionSPListDialog.h \
    MDPowerGroupTreeWindowForActionSPUI.h \
    MDActionSPListDialogParaImp.h \
    MDActionSPDialogAsModal.h \
    MDStringResource.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    MDActionSPListDialog.ui \
    MDActionSPDialog.ui \
    MDActionSPDialogAsModal.ui
