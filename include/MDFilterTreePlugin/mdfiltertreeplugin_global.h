﻿#ifndef MDFILTERTREEPLUGIN_GLOBAL_H
#define MDFILTERTREEPLUGIN_GLOBAL_H

/*!

\author bHVvY2hhbw==

\section 功能
1.MDFILTERTREEPLUGIMDHARED_EXPORT 宏定义，用于外部导出类。

*/

#include <QtCore/qglobal.h>

#if defined(MDFILTERTREEPLUGIN_LIBRARY)
#  define MDFILTERTREEPLUGIMDHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDFILTERTREEPLUGIMDHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MDFILTERTREEPLUGIN_GLOBAL_H
