﻿#ifndef MDSQLDATABASEPROXY_GLOBAL_H
#define MDSQLDATABASEPROXY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDSQLDATABASEPROXY_LIBRARY)
#  define MDSQLDATABASEPROXYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSQLDATABASEPROXYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDSQLDATABASEPROXY_GLOBAL_H
