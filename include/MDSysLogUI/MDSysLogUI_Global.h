﻿#ifndef MDSYSLOGUI_GLOBAL_H
#define MDSYSLOGUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDSYSLOGUI_LIBRARY)
#  define MDSYSLOGUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSYSLOGUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDSYSLOGUI_GLOBAL_H
