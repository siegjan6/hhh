#-------------------------------------------------
#
# Project created by QtCreator 2014-08-08T10:17:46
#
#-------------------------------------------------

QT       -= gui
QT = core
#TARGET = QSerialPort
TEMPLATE = lib
DEFINES += QT_BUILD_SERIALPORT_LIB

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

   # !wince*: {
        LIBS += -lsetupapi -ladvapi32
   # } else {
    #     SOURCES += \
     #        $$PWD/qserialport_wince.cpp \
      #       $$PWD/qserialportinfo_wince.cpp
    # }
}



HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS \
    private/qringbuffer_p.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

CONFIG +=debug_and_release
CONFIG(debug,debug|release){
DESTDIR = ../../../bin/debug/
TARGET = QSerialPort
}
else{
DESTDIR = ../../../bin/release
TARGET = QSerialPort
}


