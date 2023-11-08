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
#include "ncrdtestdatawriter.h"
#include "ncrdtestdatareader.h"

#include <QStandardItemModel>
#include <QFile>

NCRDTestDataWriter::NCRDTestDataWriter()
{
	setAutoFormatting(true);
	basetag = "NCReportTestData";
	m_model = 0;
}


NCRDTestDataWriter::~NCRDTestDataWriter()
{
}

bool NCRDTestDataWriter::write(const QString & fname)
{
	QFile file(fname);
	if (!file.open(QFile::WriteOnly | QFile::Text))
		return false;
	
	bool ok = write( &file );
	return ok;
}

bool NCRDTestDataWriter::write(QIODevice * device)
{
	if ( device )
		setDevice(device);
	if ( !m_model )
		return false;

	writeStartDocument();
	writeStartElement(basetag);
	writeAttribute("version", "1.0");
	
	writeContent();
	
	writeEndDocument();
	return true;
	
}

void NCRDTestDataWriter::writeContent()
{
	for (int row = 0; row <  m_model->rowCount(); ++row) {
		writeStartElement("data");
		
		writeAttribute("id", m_model->item(row, TESTDATAMODEL_COL_ID )->text() );
		int typeNo = m_model->item(row, TESTDATAMODEL_COL_DSTYPE )->data(Qt::EditRole).toInt();
		QString typeTxt;
		switch ( typeNo ) {
			case 0: typeTxt="list"; break;
			case 1: typeTxt="model"; break;
			case 2: typeTxt="spar"; break;
		}
		writeAttribute("type", typeTxt );
		writeAttribute("enabled", m_model->item(row, TESTDATAMODEL_COL_ENABLED )->text() );
		writeCharacters( m_model->item(row, TESTDATAMODEL_COL_TEXTDATA )->text() );
		writeEndElement();	
	}
}

QStandardItemModel * NCRDTestDataWriter::model()
{
	return m_model;
}


void NCRDTestDataWriter::setModel(QStandardItemModel *m)
{
	m_model = m;
}


