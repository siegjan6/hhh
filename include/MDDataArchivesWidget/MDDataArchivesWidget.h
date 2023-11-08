/*!
\author Y2hhaWhq

\section 功能

数据归档控件导出类
*/
#ifndef MDDATAARCHIVESWIDGET_H
#define MDDATAARCHIVESWIDGET_H

#include <QtCore/qglobal.h>
#include "MDDataGroupGrid.h"
#include "MDDataGroupGrid/MDGroupGridDefine.h"

#if defined(MDDATAARCHIVESWIDGET_LIBRARY)
#  define MDDATAARCHIVESWIDGET_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDATAARCHIVESWIDGET_SHARED_EXPORT Q_DECL_IMPORT
#endif

class MDDATAARCHIVESWIDGET_SHARED_EXPORT CMDDataArchivesWidget : public CMDDataGroupGrid
{
public:
    CMDDataArchivesWidget();
    ~CMDDataArchivesWidget(){}
};

#endif // MDDATAARCHIVESWIDGET_H
