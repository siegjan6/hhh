#-------------------------------------------------
#
# Project created by QtCreator 2014-11-05T09:59:23
#
#-------------------------------------------------

QT       += widgets gui testlib

TEMPLATE = lib

DEFINES += MDHMIFRAMEWORK_LIBRARY

include(../../pri/MDHMIFramework.pri)

ProjectName = $${LibraryName}
include(../../pri/common.pri)

include(../../pri/MDPropertyBrowser.pri)
include(../../pri/MDHMIForm.pri)
include(../../pri/MDProjectConfiger.pri)
include(../../pri/MDGraphicsStorage.pri)
include(../../pri/MDDrawObj.pri)
include(../../pri/MDBaseForm.pri)
include(../../pri/MDSwitchFactory.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDEventHelper.pri)


INCLUDEPATH += ./property \
./formPreview \

SOURCES += \
	HMIFramework.cpp \
	FormOperation.cpp \
	HMIStudio.cpp \
	FileNode.cpp \
	ObjectTreeDialog.cpp \
	MDHMICreator.cpp \
	property/LaunchConfigWidget.cpp \
	LaunchConfig.cpp \
	ToolKitDialog.cpp \
    formPreview/PreviewDialog.cpp \
    formPreview/PreviewNode.cpp \
    formPreview/FormObject.cpp \
    formPreview/FormObjectContainer.cpp \
    formPreview/PreviewPropertyWidget.cpp \
    test/formPreview/FormPreviewTest.cpp \
    CreateTool.cpp \
    property/FormTreeWidget.cpp

HEADERS +=\
	FileNode.h \
	../../include/interface/HMI/IHMIStudio.h \
	FormOperation.h \
	HMIFramework.h \
	HMIStudio.h \
	ObjectTreeDialog.h \
	../../include/interface/HMI/IObjectChangedEvent.h \
	../../include/interface/HMI/IHMIFramework.h \
	../../include/interface/HMI/IFormOperation.h \
	../../include/MDHMIFramework/MDHMICreator.h \
	property/LaunchConfigWidget.h \
	LaunchConfig.h \
	../../include/interface/HMI/IHMIWidget.h \
	ToolKitDialog.h \
    formPreview/PreviewDialog.h \
    formPreview/PreviewNode.h \
    formPreview/FormObject.h \
    formPreview/FormObjectContainer.h \
    formPreview/PreviewPropertyWidget.h \
    test/formPreview/FormPreviewTest.h \
    CreateTool.h \
    property/FormTreeWidget.h


OTHER_FILES +=

RESOURCES += \
    rs_hmiFramework.qrc

FORMS += \
	ObjectTreeDialog.ui \
	property/LaunchConfigWidget.ui \
	ToolKitDialog.ui \
    formPreview/PreviewDialog.ui \
    formPreview/PreviewPropertyWidget.ui \
    property/FormTreeWidget.ui

