
    DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
    SOURCES += $$PWD/qtpropertybrowser.cpp \
            $$PWD/qtpropertymanager.cpp \
            $$PWD/qteditorfactory.cpp \
            $$PWD/qtvariantproperty.cpp \
            $$PWD/qttreepropertybrowser.cpp \
            $$PWD/qtbuttonpropertybrowser.cpp \
            $$PWD/qtgroupboxpropertybrowser.cpp \
            $$PWD/qtpropertybrowserutils.cpp \
            $$PWD/PropertyBrowser.cpp \
            $$PWD/DialogManager.cpp \
            $$PWD/DialogEditFactory.cpp \
            $$PWD/VariantManager.cpp \
            $$PWD/VariantFactory.cpp \
            $$PWD/DialogEdit.cpp \
                        $$PWD/LineEdit.cpp
    HEADERS +=  $$PWD/qtpropertybrowserutils_p.h \
                $$PWD/VariantFactory.h \
                $$PWD/DialogEdit.h \
                $$PWD/VariantManager.h \
                $$PWD/qtvariantproperty.h \
                $$PWD/DialogEditFactory.h \
                $$PWD/PropertyBrowser_p.h \
                $$PWD/DialogManager.h \
                $$PWD/qttreepropertybrowser.h \
                $$PWD/qtpropertymanager.h \
                $$PWD/qtpropertybrowser.h \
                $$PWD/qtgroupboxpropertybrowser.h \
                $$PWD/qteditorfactory.h \
                $$PWD/qtbuttonpropertybrowser.h \
                $$PWD/LineEdit.h \
    ../../include/MDPropertyBrowser/PropertyBrowser.h


    RESOURCES += $$PWD/qtpropertybrowser.qrc

