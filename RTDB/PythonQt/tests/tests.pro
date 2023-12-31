# --------- PythonQtTest profile -------------------
# Last changed by $Author: florian $
# $Id: PythonQt.pro 35381 2006-03-16 13:05:52Z florian $
# $Source$
# --------------------------------------------------
TARGET   = PythonQtTest
TEMPLATE = app

DESTDIR    = ../lib

QT += testlib opengl

contains(QT_MAJOR_VERSION, 5) {
  QT += widgets
}

include ( ../build/common.prf )
include ( ../build/PythonQt.prf )

HEADERS +=                    \
  PythonQtTests.h

SOURCES +=                    \
  PythonQtTestMain.cpp        \
  PythonQtTests.cpp

OTHER_FILES += \
    rediscluster/__init__.py \
    rediscluster/client.py \
    rediscluster/cluster_mgt.py \
    rediscluster/connection.py \
    rediscluster/crc.py \
    rediscluster/exceptions.py \
    rediscluster/nodemanager.py \
    rediscluster/pipeline.py \
    rediscluster/pubsub.py \
    rediscluster/utils.py
