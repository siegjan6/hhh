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

#include "ncrdtestfielddemo.h"
#include "ncrdtestfieldmainwidget.h"
#include "ncrdtestfieldmainwidget2.h"
#include "ncrdtestfieldtabwidget.h"
#include "ncreportfielditem.h"

NCRDTestFieldDemoWidgetFactory::NCRDTestFieldDemoWidgetFactory()
{
}

NCRDTestFieldDemoWidgetFactory::~NCRDTestFieldDemoWidgetFactory()
{
}

QWidget *NCRDTestFieldDemoWidgetFactory::createWidget(NCReportItem *item, WidgetType type, QWidget *parent)
{
    QWidget *w =0;
    switch (item->typeId())  {
    case 1024:
    {
        switch (type) {
        case Main: w = new NCRDTestFieldMainWidget(parent); break;
        case Tab1: w = new NCRDTestFieldTabWidget(parent); break;
        default: break;
        }
        break;
    }
    case 1025:
    {
        switch (type) {
        case Main: w = new NCRDTestFieldMainWidget2(parent); break;
        default: break;
        }
        break;
    }
    }

    if (w)
        addWidget(w);   // register

    return w;
}

void NCRDTestFieldDemoWidgetFactory::afterUpdateData(NCReportItem *item)
{
    QString field = item->parameter("fieldTxt").toString();
    if (!field.isEmpty())
        item->setText(field);
}
