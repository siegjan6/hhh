﻿#ifndef MDSCRIPTENGINE_GLOBAL_H
#define MDSCRIPTENGINE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDSCRIPTENGINE_LIBRARY)
#  define MDSCRIPTENGINESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSCRIPTENGINESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDSCRIPTENGINE_GLOBAL_H
