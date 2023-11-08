/****************************************************************************
*
*  Copyright (C) 2002-2008 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: nszabo@helta.hu, info@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  nszabo@helta.hu if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#include "ncrdxmldefreader.h"
#include "ncrdcustomdatasource.h"
#include "ncreportitem.h"
#include "ncreportdef.h"
#include "ncreportcustomdsprovider.h"

NCRDXMLDefReader::NCRDXMLDefReader(NCReportDef * r) : NCReportXMLDefReader( r )
{
}

NCRDXMLDefReader::~ NCRDXMLDefReader()
{
}

void NCRDXMLDefReader::afterReadItem(NCReportItem *item)
{
    //Label, Field, Text, Line, Rectangle, Ellipse, Image, Barcode, Graph, Custom
    switch ( item->objectType() ) {
        case NCReportItem::Label:
        case NCReportItem::Field:
        case NCReportItem::Text:
        case NCReportItem::Line:
        case NCReportItem::Rectangle:
        case NCReportItem::Ellipse:
        case NCReportItem::Image:
        case NCReportItem::Barcode:
        case NCReportItem::Graph:
        case NCReportItem::Chart:
        case NCReportItem::CrossTab:
        case NCReportItem::Table:
        case NCReportItem::NoObject:
            break;
    }

    item->updateForEditor();
    item->saveSelectState();
}

NCReportDataSource * NCRDXMLDefReader::createCustomDataSource(const QString & dsID )
{
    NCReportDataSource *ds = 0;
    NCReportDef *rptDef = reportDef();
    NCReportCustomDSProvider *dsProvider = rptDef->customDSProvider();
    if ( dsProvider) {
        ds = dsProvider->createCustomDataSource( dsID, NCReportDataSource::Custom, rdef );
    }
    if ( !ds) {
        ds = new NCRDCustomDataSource( rdef );
    }
    return ds;
}
