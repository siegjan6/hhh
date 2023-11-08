/****************************************************************************
*
*  Copyright (C) 2002-2014 Helta Ltd. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: office@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*  Created: 2014.08.06. (nocisoft)
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  office@nocisoft.com if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#ifndef NCRDDEMODATA_H
#define NCRDDEMODATA_H

#include <QStringList>

class NCReportDataSource;
class QAbstractItemModel;

/*!

\section 功能

报表设计器Ｄemo数据类
＃＃＃在代码中暂时屏蔽了此项功能＃＃＃

*/
class NCRDDemoData
{
public:
    NCRDDemoData();

#ifdef NCRD_STRINGLIST_DEMO
    static QStringList createStringList1();
    static QStringList createStringList2();
    static QStringList createStringListPB();
#endif
#ifdef NCRD_CUSTOM_DATASOURCE_DEMO
    static NCReportDataSource* createCustomTestData(QObject* parent);
#endif
#ifdef NCRD_ITEMMODEL_DEMO
    static QAbstractItemModel* createDemoItemModel();
#endif
};

#endif // NCRDDEMODATA_H
