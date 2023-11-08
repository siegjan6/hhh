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

#include "ncrditemwidgetfactory.h"
#include "ncrdwidgetmapper.h"
#include "ncreportitem.h"

NCRDItemWidgetFactory::NCRDItemWidgetFactory()
{
}

NCRDItemWidgetFactory::~NCRDItemWidgetFactory()
{
    qDebug("delete NCRDItemWidgetFactory");
}

NCRDWidgetMapper *NCRDItemWidgetFactory::createMapper(NCReportItem *item, QObject *parent)
{
    Q_UNUSED(item)
    return new NCRDWidgetMapper(parent);
}

void NCRDItemWidgetFactory::updateWidget(QWidget *widget, NCReportItem *item)
{
    NCRDWidgetMapper *mapper = createMapper(0);
    mapper->setWidget(widget);
    mapper->setDataContainer(&item->parametersRef());
    mapper->updateWidget();
    delete mapper;
}

//void NCRDItemWidgetFactory::updateWidget(QWidget *widget, NCReportItem *item)
//{
//    update(DataToWidget,widget,item->parameters(),item);
//}

void NCRDItemWidgetFactory::updateData(QWidget *widget, NCReportItem *item)
{
    NCRDWidgetMapper *mapper = createMapper(0, NULL);
    mapper->setWidget(widget);
    mapper->setDataContainer(&item->parametersRef());
    mapper->updateData();
    delete mapper;
}

/*!
 * \brief NCRDItemWidgetFactory::reset
 * Resets the widget registry
 */
void NCRDItemWidgetFactory::reset()
{
    m_widgets.clear();
}

//void NCRDItemWidgetFactory::updateData(QWidget *widget, NCReportItem *item)
//{
//    update(WidgetToData,widget,item->parameters(),item);
//}

void NCRDItemWidgetFactory::afterUpdateData(NCReportItem *item)
{
    Q_UNUSED(item)
}

void NCRDItemWidgetFactory::updateWidget(NCReportItem *item)
{
    for (int i=0; i<m_widgets.count(); i++)
        updateWidget(m_widgets.at(i), item);
}

void NCRDItemWidgetFactory::updateData(NCReportItem *item)
{
    for (int i=0; i<m_widgets.count(); i++)
        updateData(m_widgets.at(i), item);
}

void NCRDItemWidgetFactory::addWidget(QWidget *widget)
{
    m_widgets.append(widget);
}


