﻿#ifndef MDUASINGLETON_GLOBAL_H
#define MDUASINGLETON_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UASINGLETON_LIBRARY)
#  define MDUASINGLETONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDUASINGLETONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDUASINGLETON_GLOBAL_H
