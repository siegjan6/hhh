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
#include "ncrdxmlcopywriter.h"
#include "ncreportsection.h"
#include "ncreportitem.h"

NCRDXMLCopyWriter::NCRDXMLCopyWriter( NCReportSection * sect, QString *string)
    : NCReportXMLWriter( string ), section(sect)
{
}

NCRDXMLCopyWriter::~ NCRDXMLCopyWriter()
{
}

void NCRDXMLCopyWriter::writeReport()
{
    writeStartElement("copy");
    writeItems( section );
    writeEndElement();

}


void NCRDXMLCopyWriter::writeItems( NCReportSection *sect )
{
    QList<QGraphicsItem*> list = sect->selectedItems();
    for (int i = 0; i < list.size(); ++i) {
        NCReportItem *item = (NCReportItem*)list.at(i);
        item->updateForSave();
        item->write( this );
    }
}
