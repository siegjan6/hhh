/****************************************************************************
*
*  Copyright (C) 2002-2013 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2013.09.26. (nocisoft)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#ifndef NCRDDATASOURCETREEDOCKWIDGET_H
#define NCRDDATASOURCETREEDOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class NCRDDataSourceTreeDockWidget;
}
class NCRDDataSourceTree;

/*!

\section 功能

数据源管理器ＤockWidget类
【管理数据源停靠窗体类】
＃＃＃在代码中暂时默认隐藏了数据源管理器停靠窗体类＃＃＃

*/
class NCRDDataSourceTreeDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit NCRDDataSourceTreeDockWidget(QWidget *parent = 0);
    ~NCRDDataSourceTreeDockWidget();

    NCRDDataSourceTree* tree();

private:
    Ui::NCRDDataSourceTreeDockWidget *ui;
};

#endif // NCRDDATASOURCETREEDOCKWIDGET_H
