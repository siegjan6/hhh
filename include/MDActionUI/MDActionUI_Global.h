﻿#ifndef MDACTIONUI_GLOBAL_H
#define MDACTIONUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDACTIONUI_LIBRARY)
#  define MDACTIONUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDACTIONUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDACTIONUI_GLOBAL_H
