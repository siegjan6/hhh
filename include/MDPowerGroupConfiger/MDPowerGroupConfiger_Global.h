﻿#ifndef MDPOWERGROUPCONFIGER_GLOBAL_H
#define MDPOWERGROUPCONFIGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDPOWERGROUPCONFIGER_LIBRARY)
#  define MDPOWERGROUPCONFIGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDPOWERGROUPCONFIGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDPOWERGROUPCONFIGER_GLOBAL_H
