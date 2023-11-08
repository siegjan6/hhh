#-------------------------------------------------
#
# Project created by QtCreator 2015-03-09T14:06:44
#
#-------------------------------------------------

QT       += widgets
TEMPLATE = lib

DEFINES += MDDRAWWIDGETS_LIBRARY

include(../../pri/MDDrawWidgets.pri)

ProjectName = $${LibraryName}
include(../../pri/common.pri)
include(../../pri/MDDrawObj.pri)
include(../../pri/MDMultiLanguage.pri)
include(../../pri/MDUtility.pri)

SOURCES += \
    DrawTextEdit/MDPropertyDrawTextEdit.cpp \
    DrawTextEdit/MDDrawTextEdit.cpp \
    DrawTextEdit/MDDrawTexEditEvent.cpp \
    DrawTextEdit/DrawTextEditData.cpp \
    DrawRadioButton/MDDrawRadioButton.cpp \
    DrawRadioButton/MDPropertyDrawRadioButton.cpp \
    DrawRadioButton/DrawRadioButtonData.cpp \
    DrawRadioButton/MDDrawRadioButtonEvent.cpp \
    DrawCheckBox/MDDrawCheckBox.cpp \
    DrawCheckBox/DrawCheckBoxData.cpp \
    DrawCheckBox/MDDrawCheckBoxEvent.cpp \
    DrawCheckBox/MDPropertyDrawCheckBox.cpp \
    DrawDateTimeEdit/DrawDateTimeEditData.cpp \
    DrawDateTimeEdit/MDDrawDateTimeEdit.cpp \
    DrawDateTimeEdit/MDDrawDateTimeEditEvent.cpp \
    DrawDateTimeEdit/MDPropertyDrawDateTimeEdit.cpp \
    DrawComboBox/DrawComboBoxData.cpp \
    DrawComboBox/MDDrawComboBox.cpp \
    DrawComboBox/MDDrawComboBoxEvent.cpp \
    DrawComboBox/MDPropertyDrawComboBox.cpp \
    DrawBasicWidget/DrawBasicWidgetData.cpp \
    DrawBasicWidget/MDDrawBasicWidget.cpp \
    DrawSpinBox/MDDrawSpinBox.cpp \
    DrawSpinBox/DrawSpinBoxData.cpp \
    DrawSpinBox/MDDrawSpinBoxEvent.cpp \
    DrawSpinBox/MDPropertyDrawSpinBox.cpp \
    DrawCalendarWidget/MDDrawCalendarWidget.cpp \
    DrawCalendarWidget/DrawCalendarWidgetData.cpp \
    DrawCalendarWidget/MDDrawCalendarWidgetEvent.cpp \
    DrawCalendarWidget/MDPropertyDrawCalendarWidget.cpp \
    DrawComboBox/DrawSubitemDialog.cpp \
    DrawList/AddItemDialog.cpp \
    DrawList/MDDrawList.cpp \
    DrawList/MDPropertyDrawList.cpp \
    DrawList/MDDrawListEvent.cpp \
    DrawList/DrawListData.cpp \
    DrawBasicWidget/MDDrawBasicWidgetEvent.cpp \
    DrawBasicWidget/MDPropertyDrawBasicWidget.cpp \
    DrawTableView/DrawTableViewData.cpp \
    DrawTableView/MDDrawTableView.cpp \
    DrawTableView/ItemDelegate.cpp \
    DrawTableView/PropertyDrawTableView.cpp \
    DrawCalendarWidget/CalendarForm.cpp \
    DrawTextEdit/MDDrawTextEditFormatDialog.cpp \
    undo/DrawBasicWidgetCommand.cpp \
    undo/DrawTextEditCommand.cpp \
    undo/DrawComboboxCommand.cpp \
    undo/DrawListCommand.cpp \
    undo/DrawRadioButtonCommand.cpp \
    undo/DrawCheckBoxCommand.cpp \
    undo/DrawDateTimeCommand.cpp \
    undo/DrawCalendarWidgetCommand.cpp \
    DrawTextEdit/CDrawTextEditTextDialog.cpp


HEADERS += \
    ../../include/MDDrawWidgets/MDPropertyDrawList.h \
    DrawTextEdit/DrawTextEditData.h \
    ../../include/MDDrawWidgets/MDDrawTextEdit.h \
    ../../include/MDDrawWidgets/MDPropertyDrawTextEdit.h \
    ../../include/MDDrawWidgets/MDDrawTexEditEvent.h \
    DrawRadioButton/DrawRadioButtonData.h \
    ../../include/MDDrawWidgets/MDDrawRadioButton.h \
    ../../include/MDDrawWidgets/MDPropertyDrawRadioButton.h \
    ../../include/MDDrawWidgets/MDDrawRadioButtonEvent.h \
    DrawCheckBox/DrawCheckBoxData.h \
    ../../include/MDDrawWidgets/MDPropertyDrawCheckBox.h \
    ../../include/MDDrawWidgets/MDDrawCheckBoxEvent.h \
    ../../include/MDDrawWidgets/MDDrawCheckBox.h \
    ../../include/MDDrawWidgets/MDDrawDateTimeEditEvent.h \
    ../../include/MDDrawWidgets/MDPropertyDrawDateTimeEdit.h \
    ../../include/MDDrawWidgets/MDDrawDateTimeEdit.h \
    DrawDateTimeEdit/DrawDateTimeEditData.h \
    ../../include/MDDrawWidgets/MDDrawComboBox.h \
    DrawComboBox/DrawComboBoxData.h \
    ../../include/MDDrawWidgets/MDDrawComboBoxEvent.h \
    ../../include/MDDrawWidgets/MDPropertyDrawComboBox.h \
    DrawBasicWidget/DrawBasicWidgetData.h \
    ../../include/MDDrawWidgets/MDDrawBasicWidget.h \
    DrawSpinBox/DrawSpinBoxData.h \
    ../../include/MDDrawWidgets/MDDrawSpinBox.h \
    ../../include/MDDrawWidgets/MDDrawSpinBoxEvent.h \
    ../../include/MDDrawWidgets/MDPropertyDrawSpinBox.h \
    ../../include/MDDrawWidgets/MDDrawCalendarWidgetEvent.h \
    ../../include/MDDrawWidgets/MDDrawCalendarWidget.h \
    ../../include/MDDrawWidgets/MDPropertyDrawCalendarWidget.h \
    DrawCalendarWidget/DrawCalendarWidgetData.h \
    DrawComboBox/DrawSubitemDialog.h \
    DrawList/AddItemDialog.h \
    ../../include/MDDrawWidgets/MDDrawList.h \
    ../../include/MDDrawWidgets/MDDrawListEvent.h \
    DrawList/DrawListData.h \
    ../../include/MDDrawWidgets/MDDrawBasicWidgetEvent.h \
    ../../include/MDDrawWidgets/MDPropertyDrawBasicWidget.h \
    DrawTableView/DrawTableViewData.h \
    DrawTableView/ItemDelegate.h \
    ../../include/MDDrawWidgets/MDDrawTableView.h \
    DrawTableView/PropertyDrawTableView.h \
    DrawCalendarWidget/CalendarForm.h \
    DrawTextEdit/MDDrawTextEditFormatDialog.h \
    undo/DrawBasicWidgetCommand.h \
    undo/DrawTextEditCommand.h \
    undo/DrawComboboxCommand.h \
    undo/DrawListCommand.h \
    undo/DrawRadioButtonCommand.h \
    undo/DrawCheckBoxCommand.h \
    undo/DrawDateTimeCommand.h \
    undo/DrawCalendarWidgetCommand.h \
    DrawTextEdit/CDrawTextEditTextDialog.h

FORMS += \
    DrawComboBox/DrawSubitemDialog.ui \
    DrawList/AddItemDialog.ui \
    DrawCalendarWidget/CalendarForm.ui \
    DrawTextEdit/MDDrawTextEditFormatDialog.ui \
    DrawTextEdit/CDrawTextEditTextDialog.ui

