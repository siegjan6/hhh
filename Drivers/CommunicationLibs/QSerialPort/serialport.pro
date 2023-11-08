QT = core

include($$PWD/serialport-lib.pri)

TARGET = QtSerialPort

load(qt_module)
TEMPLATE = lib
DESTDIR = ..\
