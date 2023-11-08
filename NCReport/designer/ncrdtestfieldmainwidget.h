/****************************************************************************
*
*  Copyright (C) 2002-2015 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2015. 02. 21. (nszabo)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#ifndef NCRDTESTFIELDMAINWIDGET_H
#define NCRDTESTFIELDMAINWIDGET_H

#include <QWidget>

namespace Ui {
class NCRDTestFieldMainWidget;
}

/*!

section 功能

【测试字段主窗体类】
＃＃＃暂时屏蔽测试窗体相关功能＃＃＃

*/
class NCRDTestFieldMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NCRDTestFieldMainWidget(QWidget *parent = 0);
    ~NCRDTestFieldMainWidget();

private:
    Ui::NCRDTestFieldMainWidget *ui;
};

#endif // NCRDTESTFIELDMAINWIDGET_H
