﻿#ifndef MDACCOUNTUI_GLOBAL_H
#define MDACCOUNTUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDACCOUNTUI_LIBRARY)
#  define MDACCOUNTUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDACCOUNTUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDACCOUNTUI_GLOBAL_H