PROJECT                 = qtRedisClient
TEMPLATE                = lib

CONFIG                 += debug_and_release

CONFIG                 += qt
CONFIG                 += warn_on
CONFIG                 += thread

CONFIG			+= dll
#CONFIG			+= staticlib

QT                     -= gui

HEADERS                 = MDQtRedisClient.h
SOURCES                 = MDQtRedisClient.cpp

DEFINES += QTREDISCLIENT_LIBRARY
include(qredisclient.pri)
include(../../pri/MDQtRedisClient.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)


