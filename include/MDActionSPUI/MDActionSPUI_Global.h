﻿#ifndef MDACTIONSPUI_GLOBAL_H
#define MDACTIONSPUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDACTIONSPUI_LIBRARY)
#  define MDACTIONSPUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDACTIONSPUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDACTIONSPUI_GLOBAL_H