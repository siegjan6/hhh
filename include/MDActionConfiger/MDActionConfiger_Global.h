﻿#ifndef MDACTIONCONFIGER_GLOBAL_H
#define MDACTIONCONFIGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDACTIONCONFIGER_LIBRARY)
#  define MDACTIONCONFIGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDACTIONCONFIGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDACTIONCONFIGER_GLOBAL_H
