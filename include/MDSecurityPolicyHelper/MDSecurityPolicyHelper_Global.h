﻿#ifndef MDSECURITYPOLICYHELPER_GLOBAL_H
#define MDSECURITYPOLICYHELPER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDSECURITYPOLICYHELPER_LIBRARY)
#  define MDSECURITYPOLICYHELPERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSECURITYPOLICYHELPERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDSECURITYPOLICYHELPER_GLOBAL_H
