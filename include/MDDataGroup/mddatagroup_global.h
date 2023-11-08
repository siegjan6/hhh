﻿#ifndef MDDATAGROUP_GLOBAL_H
#define MDDATAGROUP_GLOBAL_H

/*!

\author luyj

\section 功能
1.MDDATAGROUPSHARED_EXPORT 宏定义

*/

#include <QtCore/qglobal.h>

#if defined(MDDATAGROUP_LIBRARY)
#  define MDDATAGROUPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDATAGROUPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDDATAGROUP_GLOBAL_H
