#ifndef MDPROJECTCONFIGER_GLOBAL_H
#define MDPROJECTCONFIGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDPROJECTCONFIGER_LIBRARY)
#  define MDPROJECTCONFIGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDPROJECTCONFIGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "../../include/MDActionConfiger/MDActionConfiger.h"
#include "../../include/MDEventConfiger/MDEventConfiger.h"

#endif // MDPROJECTCONFIGER_GLOBAL_H
