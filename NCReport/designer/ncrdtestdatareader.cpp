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
#include "ncrdtestdatareader.h"
#include "ncreportdatasource.h"

#include <QObject>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QIODevice>
#include <QFile>

NCRDTestDataReader::NCRDTestDataReader()
{
	m_model = 0;
	basetag = "NCReportTestData";
}

NCRDTestDataReader::~ NCRDTestDataReader()
{
}

bool NCRDTestDataReader::read(QIODevice *device )
{
	if ( m_model == 0 ) {
		return false;
	}
	
	if ( device )
    	setDevice(device);
		
    while (!atEnd()) {
        readNext();

        if (isStartElement()) {
            if (name() == basetag && attributes().value("version") == "1.0")
				readContents();
            else
                raiseError(QObject::tr("The file is not an %1 version 1.0 test data file.").arg(basetag) );
        }
    }

    return !error();
}

void NCRDTestDataReader::readContents()
{
    Q_ASSERT(isStartElement() && name() == basetag);
	
    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
			if (name() == "options")
                readOptions();
            else if (name() == "data")
                readData();
            else
                readUnknownElement();
        }
    }
}

void NCRDTestDataReader::readOptions()
{
/*	Q_ASSERT(isStartElement() && name() == "options");
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "encoding")
			{
				QString stmp = readElementText();
				pageoption->encoding = stmp;
			}
            else readUnknownElement();
        }
    }*/
}

void NCRDTestDataReader::readData()
{
	Q_ASSERT(isStartElement() && name() == "data");
	
	NCRDTestData data;
	data.id = attrib("id").toString();
	
	QStringRef dtype = attrib("type");
	data.type = 0;
	if (dtype=="list") data.type =0 ;
	else if (dtype=="model") data.type=1 ;
	else if (dtype=="spar") data.type=2 ;
	
	data.enabled = (attrib("enabled") == "true");
	data.data = readElementText();
	
	appendModel( m_model, data );
}


void NCRDTestDataReader::readUnknownElement()
{
    //Q_ASSERT(isStartElement());
	qDebug( "Unknown element: %s", qPrintable(name().toString()) ) ;
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement())
            readUnknownElement();
    }
}

bool NCRDTestDataReader::isAttrib(const QString & name )
{
	return !attributes().value(name).isEmpty();
}

QStringRef NCRDTestDataReader::attrib(const QString & name )
{
	return attributes().value(name);
}

QStandardItemModel * NCRDTestDataReader::model()
{
	return m_model;
}

void NCRDTestDataReader::setModel(QStandardItemModel *m )
{
	m_model = m;
}

bool NCRDTestDataReader::read(const QString & filename)
{
	bool ok = true;
	QFile file(filename);
	if ( file.open(QIODevice::ReadOnly | QIODevice::Text)) 
		ok = read( &file );
	
	return ok;
}

void NCRDTestDataReader::appendModel(QStandardItemModel * m, const NCRDTestData & data)
{
	QStandardItem *item =0;
	int row = m->rowCount();
	// ID
	item = new QStandardItem( data.id );
	m->setItem( row, TESTDATAMODEL_COL_ID, item);
	// DS.Type
	item = new QStandardItem();
	item->setData( data.type, Qt::EditRole );
	m->setItem( row, TESTDATAMODEL_COL_DSTYPE, item );
	// enabled
	item = new QStandardItem( data.enabled ? "true" : "false" );
	m->setItem( row, TESTDATAMODEL_COL_ENABLED, item);
	
	item = new QStandardItem( data.data );
	m->setItem( row, TESTDATAMODEL_COL_TEXTDATA, item);
}

