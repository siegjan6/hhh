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
#include "ncrdxmlcopyreader.h"
#include "ncreportitem.h"
#include "ncrdsectionscene.h"
#include "ncrdcommands.h"
#include "ncrddocument.h"

NCRDXMLCopyReader::NCRDXMLCopyReader( NCRDSectionScene *sect )
    : m_section(sect)
{
    basetag = "NCReport";

}

NCRDXMLCopyReader::~NCRDXMLCopyReader()
{
}

void NCRDXMLCopyReader::init()
{

}

void NCRDXMLCopyReader::readReport()
{
    m_section->clearSelection();

    Q_ASSERT(isStartElement() && name() == basetag);
    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            if (name() == "copy")
                readItems( m_section );
            else
                readUnknownElement();
        }
    }
}

void NCRDXMLCopyReader::afterReadItem(NCReportItem *item)
{
    item->setId("%");
    item->updateForEditor();
}

void NCRDXMLCopyReader::addItem(NCReportItem *item, NCReportSection* )
{
    NCRDInsertItemCommand *command = new NCRDInsertItemCommand( item, m_section, item->objectType(), item->type(), item->pos() );
    m_section->document()->undoStack()->push( command );
}

void NCRDXMLCopyReader::afterAddItem(NCReportItem *item)
{
    item->offsetPastePos();
    item->setSelected(true);
    item->saveSelectState();
}


/*
*/
