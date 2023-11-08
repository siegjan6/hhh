/*!
\author Y2hhaWhq
\section 日志控件导出类
*/
#ifndef MDLOGGRID_H
#define MDLOGGRID_H

#include <QtCore/qglobal.h>
#include <QWidget>
#include "MDDataGroupGrid.h"
#include "MDDataGroupGrid/MDGroupGridDefine.h"

#if defined(MDLOGGRID_LIBRARY)
#  define MDLOGGRID_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDLOGGRID_SHARED_EXPORT Q_DECL_IMPORT
#endif

class MDLOGGRID_SHARED_EXPORT CMDLogGrid : public CMDDataGroupGrid
{
public:
    CMDLogGrid();
    ~CMDLogGrid(){}
};

#endif // MDLOGGRID_H

