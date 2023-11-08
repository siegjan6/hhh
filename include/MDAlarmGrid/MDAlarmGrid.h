/*!
\author luyj

\section 功能

报警控件导出类
*/
#ifndef MDALARMGRID_H
#define MDALARMGRID_H

#include <QtCore/qglobal.h>
#include <QWidget>
#include "MDDataGroupGrid.h"
#include "MDDataGroupGrid/MDGroupGridDefine.h"

#if defined(MDALARMGRID_LIBRARY)
#  define MDALARMGRID_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDALARMGRID_SHARED_EXPORT Q_DECL_IMPORT
#endif

class MDALARMGRID_SHARED_EXPORT CMDAlarmGrid : public CMDDataGroupGrid
{
public:
    CMDAlarmGrid();
    ~CMDAlarmGrid(){}
};

#endif // MDALARMGRID_H

