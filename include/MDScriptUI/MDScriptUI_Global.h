﻿#ifndef MDSCRIPTUI_GLOBAL_H
#define MDSCRIPTUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDSCRIPTUI_LIBRARY)
#  define MDSCRIPTUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSCRIPTUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDSCRIPTUI_GLOBAL_H
