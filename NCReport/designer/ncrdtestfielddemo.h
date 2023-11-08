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

#ifndef NCRDTESTFIELDDEMO_H
#define NCRDTESTFIELDDEMO_H

#include "ncrditemwidgetfactory.h"

/*!

\section 功能

【测试数据demo类】
＃＃＃暂时屏蔽测试数据窗体相关功能＃＃＃

*/
class NCRDTestFieldDemoWidgetFactory : public NCRDItemWidgetFactory
{
public:
    NCRDTestFieldDemoWidgetFactory();
    virtual ~NCRDTestFieldDemoWidgetFactory();

    QWidget* createWidget( NCReportItem* item, WidgetType type, QWidget* parent );
    void afterUpdateData(NCReportItem* item);

};

#endif // NCRDTESTFIELDDEMO_H
