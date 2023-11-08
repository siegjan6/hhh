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

#ifndef NCRDITEMWIDGETFACTORY_H
#define NCRDITEMWIDGETFACTORY_H

#include "ncrd_global.h"
#include <QHash>
#include <QVariant>

class NCReportItem;
class NCRDWidgetMapper;
class QWidget;
class QDialog;

/*!
 * \brief The NCRDItemWidgetFactory class
 * The NCRDItemWidgetFactory class provides a way to create custom widget for editing any item data in some dialog of report designer.
 */

/*!

\section 功能

工厂类，提供了在某些对话框中编辑任何项目数据的方法来创建自定义控件

*/
class NCREPORTDESIGNER_SHARED_EXPORT NCRDItemWidgetFactory
{
public:
    NCRDItemWidgetFactory();
    virtual ~NCRDItemWidgetFactory();

    enum WidgetType { Dialog, Main=0, Tab1, Tab2, Options1, Options2 };

    void updateWidget(NCReportItem *item );
    void updateData(NCReportItem *item);

    void updateWidget(QWidget *widget, NCReportItem *item );
    void updateData(QWidget *widget, NCReportItem *item);

    void reset();

public:
    /*!
     * \brief createWidget
     * \param item
     * \param type
     * \param parent
     * \return
     * Creates a widget for custom handling parameters in designer dialogs and setter widgets.
     */
    virtual QWidget* createWidget( NCReportItem* item, WidgetType type, QWidget* parent ) =0;
    virtual NCRDWidgetMapper* createMapper( NCReportItem* item=0, QObject* parent=0 );
    virtual void afterUpdateData(NCReportItem* item);

protected:
    void addWidget(QWidget* widget);

private:
    QList<QWidget*> m_widgets;
};



#endif // NCRDITEMWIDGETFACTORY_H
