﻿#ifndef MDALARMCONFIGER_GLOBAL_H
#define MDALARMCONFIGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDALARMCONFIGER_LIBRARY)
#  define MDALARMCONFIGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDALARMCONFIGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDALARMCONFIGER_GLOBAL_H
