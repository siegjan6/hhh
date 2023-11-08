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

#include "ncrdgraphitemdialogfactory.h"
#include "ncrdwidgetmapper.h"
#include "ncreportgraphitem.h"

#include <QDialog>

NCRDGraphItemDialogFactory::NCRDGraphItemDialogFactory()
{
}

NCRDGraphItemDialogFactory::~NCRDGraphItemDialogFactory()
{
    qDebug("delete NCRDGraphItemDialogFactory");
}

void NCRDGraphItemDialogFactory::afterUpdateData(NCReportItem *item)
{
    // COPY SOME PROPERTY PARAMETERS
    NCReportGraphItem *graphItem = (NCReportGraphItem*)item;
    QString classId = item->parameter("classID").toString();
    if (!classId.isEmpty())
        graphItem->setClassID(classId);
}

QWidget *NCRDGraphItemDialogFactory::createWidget(NCReportItem *item, WidgetType type, QWidget *parent)
{
    Q_UNUSED(item)
    Q_UNUSED(parent)
    Q_UNUSED(type)
    return NULL;
}

