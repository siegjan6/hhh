﻿#ifndef MDVARIABLECHANGESIMULATOR_GLOBAL_H
#define MDVARIABLECHANGESIMULATOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MDVARIABLECHANGESIMULATOR_LIBRARY)
#  define MDVARIABLECHANGESIMULATORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDVARIABLECHANGESIMULATORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDVARIABLECHANGESIMULATOR_GLOBAL_H
