﻿#ifndef MDEVENTUI_GLOBAL_H
#define MDEVENTUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDEVENTUI_LIBRARY)
#  define MDEVENTUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDEVENTUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDEVENTUI_GLOBAL_H
