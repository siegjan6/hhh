﻿#ifndef MDSECURITYPOLICYEXPERT_GLOBAL_H
#define MDSECURITYPOLICYEXPERT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDSECURITYPOLICYEXPERT_LIBRARY)
#  define MDSECURITYPOLICYEXPERTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSECURITYPOLICYEXPERTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDSECURITYPOLICYEXPERT_GLOBAL_H
