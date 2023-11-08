/****************************************************************************
*
*  Copyright (C) 2002-2014 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: office@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
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
#ifndef NCRDGRAPHITEMDIALOGFACTORY_H
#define NCRDGRAPHITEMDIALOGFACTORY_H

#include "ncrd_global.h"
#include "ncrditemwidgetfactory.h"
#include <QHash>
#include <QVariant>

class NCReportGraphItem;
class NCRDWidgetMapper;
class QWidget;
class QDialog;

/*!
 * \brief The NCRDGraphItemDialogFactory class
 * The NCRDGraphItemDialogFactory class provides a way to create custom dialog for editing custom/graph item data (\class NCReportGraphItem) in report designer.
 */

/*!

\section 功能

报表图表控件管理对话框工厂类
【提供了用于编辑自定义/图形数据的自定义对话框的方法】
＃＃＃在代码中暂时屏蔽了此项功能＃＃＃

*/
class NCREPORTDESIGNER_SHARED_EXPORT NCRDGraphItemDialogFactory : public NCRDItemWidgetFactory
{
public:
    NCRDGraphItemDialogFactory();
    virtual ~NCRDGraphItemDialogFactory();

    virtual QDialog* createDialog( NCReportItem* item, QWidget* parent ) =0;
    virtual void afterUpdateData(NCReportItem* item);

protected:
    virtual QWidget* createWidget( NCReportItem* item, WidgetType type, QWidget* parent );
};

#endif // NCRDGRAPHITEMDIALOGFACTORY_H
