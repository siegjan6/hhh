﻿#ifndef MDALARMUI_GLOBAL_H
#define MDALARMUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDALARMUI_LIBRARY)
#  define MDALARMUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDALARMUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDALARMUI_GLOBAL_H
