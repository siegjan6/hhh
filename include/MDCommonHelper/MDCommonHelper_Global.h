﻿#ifndef MDCOMMONHELPER_GLOBAL_H
#define MDCOMMONHELPER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDCOMMONHELPER_LIBRARY)
#  define MDCOMMONHELPERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDCOMMONHELPERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDCOMMONHELPER_GLOBAL_H
