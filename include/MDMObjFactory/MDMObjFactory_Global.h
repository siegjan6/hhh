﻿#ifndef MDMOBJFACTORY_GLOBAL_H
#define MDMOBJFACTORY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDMOBJFACTORY_LIBRARY)
#  define MDMOBJFACTORYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDMOBJFACTORYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDMOBJFACTORY_GLOBAL_H
