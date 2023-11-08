/****************************************************************************
*
*  Copyright (C) 2002-2015 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2015. 02. 23. (nszabo)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#ifndef NCRDTESTFIELDTABWIDGET_H
#define NCRDTESTFIELDTABWIDGET_H

#include <QWidget>

namespace Ui {
class NCRDTestFieldTabWidget;
}

/*!

section 功能

【测试字段主窗体类】
＃＃＃暂时屏蔽测试窗体相关功能＃＃＃

*/
class NCRDTestFieldTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NCRDTestFieldTabWidget(QWidget *parent = 0);
    ~NCRDTestFieldTabWidget();

private:
    Ui::NCRDTestFieldTabWidget *ui;
};

#endif // NCRDTESTFIELDTABWIDGET_H
