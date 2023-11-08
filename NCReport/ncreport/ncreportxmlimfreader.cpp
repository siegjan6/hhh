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
#include "ncreportxmlimfreader.h"

#include <QStringList>
#include <QTextStream>

NCReportXMLImfReader::NCReportXMLImfReader()
{
	init();
}

void NCReportXMLImfReader::init()
{
	basetag = "NCReportIMF";
}

NCReportXMLImfReader::~NCReportXMLImfReader()
{
}

void NCReportXMLImfReader::setPageXMLList(QStringList * list)
{
	pageList = list;
}

bool NCReportXMLImfReader::read(QIODevice *device, const QString& def )
{
	if ( device )
    	setDevice(device);
	else if ( !def.isNull() )
		addData( def );
		
    while (!atEnd()) {
        readNext();

        if (isStartElement()) {
            if (name() == basetag && attributes().value("version") == "2.0")
                readReport();
            else
                raiseError(QObject::tr("The file is not an %1 version 2.0 file.").arg(basetag) );
        }
    }

    return !error();
}

void NCReportXMLImfReader::readReport()
{
    Q_ASSERT(isStartElement() && name() == basetag);
    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;
		
        if (isStartElement()) {
			
			//qDebug( "Name: %s Row: %i", qPrintable( name().toString() ), (int)lineNumber() );
			
			if (name() == "options")
                readOptions();
            else if (name() == "htmltext")
                readHtmlText();
            else if (name() == "page")
                readPage();
            else
                readUnknownElement();
        }
    }
}

void NCReportXMLImfReader::readHtmlText()
{
}

void NCReportXMLImfReader::readPage()
{
	Q_ASSERT(isStartElement() && name() == "page");
	
	//NCReportDataSourceData ds;
	//ds.id = attribToString("id");
	QString contents;
	QTextStream ts( &contents, QIODevice::WriteOnly );
	ts << "<page id=\"" << attribToString("id") << "\">\n";
	
	while (!atEnd()) {
		readNext();
		if (isEndElement())
			break;
		
		if (isStartElement()) {
			//qDebug( "readPage() Name: %s Row: %i", qPrintable( name().toString() ), (int)lineNumber() );
			writeItem( ts );
		}
	}
	
	ts << "</page>";
	pageList->append( contents );
	//qDebug( qPrintable(contents) );
}

void NCReportXMLImfReader::readItems( NCReportSection* section )
{
	Q_UNUSED( section);
}

void NCReportXMLImfReader::writeItem( QTextStream& ts )
{
	ts << "<" << name().toString();
	for (int i = 0; i < attributes().size(); ++i)
		ts << " " << attributes().at(i).name().toString() << "=\"" << attributes().at(i).value().toString() << "\"";
	ts << ">";
	ts << readElementText();
	ts << "</" << name().toString() << ">\n";
	//qDebug( qPrintable(s) );
}

