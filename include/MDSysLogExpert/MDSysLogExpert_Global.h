﻿#ifndef MDSYSLOGEXPERT_GLOBAL_H
#define MDSYSLOGEXPERT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDSYSLOGEXPERT_LIBRARY)
#  define MDSYSLOGEXPERTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSYSLOGEXPERTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDSYSLOGEXPERT_GLOBAL_H
