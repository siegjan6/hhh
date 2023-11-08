#-------------------------------------------------
#
# Project created by QtCreator 2015-12-25T11:12:51
#
#-------------------------------------------------

QT       += core gui widgets

TEMPLATE = lib

DEFINES += MDMULTILANGUAGE_LIBRARY

include(../../pri/MDMultiLanguage.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

INCLUDEPATH += config \
language \
widget \

SOURCES +=\
    config/DictDialog.cpp \
    config/DictEditDialog.cpp \
    config/GenerateDictDialog.cpp \
    config/ItemDelegate.cpp \
    config/LanguageDialog.cpp \
    config/LanguageToolWidget.cpp \
    language/Dictionary.cpp \
    language/DictionaryManager.cpp \
    language/LanguageId.cpp \
    MDMultiLanguage.cpp \
    widget/TranslateItem.cpp \
    widget/TranslateWidgetTool.cpp

HEADERS  += \
    config/DictDialog.h \
    config/DictEditDialog.h \
    config/GenerateDictDialog.h \
    config/ItemDelegate.h \
    config/LanguageDialog.h \
    config/LanguageToolWidget.h \
    language/Dictionary.h \
    language/DictionaryManager.h \
    language/LanguageId.h \
    widget/TranslateItem.h \
    widget/TranslateWidgetTool.h \
    ../../include/interface/Common/IDictionaryManager.h \
    ../../include/interface/Common/ILanguageObject.h \
    ../../include/interface/Common/ITranslateWidgetTool.h \
    ../../include/MDMultiLanguage/MDMultiLanguage.h

FORMS    += \
    config/DictDialog.ui \
    config/DictEditDialog.ui \
    config/GenerateDictDialog.ui \
    config/LanguageDialog.ui \
    config/LanguageToolWidget.ui
