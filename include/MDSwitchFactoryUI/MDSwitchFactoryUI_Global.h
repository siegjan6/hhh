﻿#ifndef MDSWITCHFACTORYUI_GLOBAL_H
#define MDSWITCHFACTORYUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDSWITCHFACTORYUI_LIBRARY)
#  define MDSWITCHFACTORYUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDSWITCHFACTORYUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDSWITCHFACTORYUI_GLOBAL_H