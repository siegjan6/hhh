﻿#ifndef MDVARIABLEEXPERT_GLOBAL_H
#define MDVARIABLEEXPERT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDVARIABLEEXPERT_LIBRARY)
#  define MDVARIABLEEXPERTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDVARIABLEEXPERTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDVARIABLEEXPERT_GLOBAL_H
