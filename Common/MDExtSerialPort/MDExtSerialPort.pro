#PROJECT                 = qextserialport
TARGET                  = MDExtSerialPort
TEMPLATE                = lib

CONFIG                 += debug_and_release

CONFIG                 += qt
CONFIG                 += warn_on
CONFIG                 += thread

CONFIG			+= dll
#CONFIG			+= staticlib

QT                     -= gui

DEPENDDIR               = .
INCLUDEDIR              = .
HEADERS                 = qextserialbase.h \
                          qextserialenumerator.h \
                          MDExtSerialPort.h
SOURCES                 = qextserialbase.cpp \
                          MDExtSerialPort.cpp \
                          qextserialenumerator.cpp

DEFINES += EXTSERIALPORT_LIBRARY

include(../../pri/MDExtSerialPort.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

unix:HEADERS           += posix_qextserialport.h
unix:SOURCES           += posix_qextserialport.cpp
#unix:DEFINES           += _TTY_POSIX_


win32:HEADERS          += win_qextserialport.h
win32:SOURCES          += win_qextserialport.cpp
#win32:DEFINES          += _TTY_WIN_

win32:LIBS             += -lsetupapi
win32:LIBS             += -lAdvAPI32  # for regedit 注冊表操作


