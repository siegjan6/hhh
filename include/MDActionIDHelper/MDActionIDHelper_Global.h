﻿#ifndef MDACTIONIDHELPER_GLOBAL_H
#define MDACTIONIDHELPER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDACTIONIDHELPER_LIBRARY)
#  define MDACTIONIDHELPERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDACTIONIDHELPERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDACTIONIDHELPER_GLOBAL_H
