INCLUDEPATH += $$PWD

PUBLIC_HEADERS += \
    $$PWD/qserialportglobal.h \
    $$PWD/qserialport.h \
    $$PWD/qserialportinfo.h

PRIVATE_HEADERS += \
    $$PWD/qserialport_p.h \
    $$PWD/qserialportinfo_p.h

SOURCES += \
    $$PWD/qserialport.cpp \
    $$PWD/qserialportinfo.cpp

win32 {
    PRIVATE_HEADERS += \
        $$PWD/qserialport_win_p.h

    SOURCES += \
        $$PWD/qserialport_win.cpp \
        $$PWD/qserialportinfo_win.cpp

    !wince*: {
        LIBS += -lsetupapi -ladvapi32
    } else {
        SOURCES += \
            $$PWD/qserialport_wince.cpp \
            $$PWD/qserialportinfo_wince.cpp
    }
}



HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS
