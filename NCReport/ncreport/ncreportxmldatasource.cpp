/****************************************************************************
*
*  Copyright (C) 2002-2012 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport Report Generator System
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  norbert@nocisoft.com if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncreportxmldatasource.h"
#include "ncreportdef.h"
#include "ncreportevaluator.h"
#include "ncreporthttpclient.h"

#include <QXmlStreamReader>
#include <QFile>

NCReportXMLDataSource::NCReportXMLDataSource(QObject *parent) : NCReportDataSource(parent), m_state(Off)
{
    setDataSourceType(Xml);
}

NCReportXMLDataSource::~NCReportXMLDataSource()
{
}

bool NCReportXMLDataSource::open(OPEN_MODEL model)
{
    m_OpenModel = model;
    if (isOpened())
        return false;

    QXmlStreamReader reader;
    QFile *xmlFile=0;
    m_columns.clear();
    m_rows.clear();
    m_state = Off;

    switch ( locationType() ) {
    case Static:
    case Parameter:
    {
        reader.addData(specification());
        break;
    }
    case File:
    {
        xmlFile = new QFile( fileName(), this );
        if ( !xmlFile->open( QIODevice::ReadOnly ) ) {
            if (error())
                error()->setError( tr("Cannot open file %1").arg( fileName() ) );
            return false;
        }
        reader.setDevice(xmlFile);
        break;
    }
    case Http:
    {
        NCReportHttpClient http;
        http.waitForDownloadContent( fileName() );	//download by url
        reader.addData( http.content() );
        http.clear();
        break;
    }
    case Ftp:
        return false;
    }

    while (!reader.atEnd()) {
        reader.readNext();

        if (reader.isStartElement()) {
            //qDebug("Start: %s", qPrintable(reader.name().toString()) );

            if (reader.name() == "parameters") {
                m_state = ParametersTag;
            } else if (reader.name() == "datasources") {
                m_state = DataSourcesTag;
            } else if (reader.name() == "datasource") {
                m_state = DataSourceTag;
                if ( id() == reader.attributes().value("id") ) {
                    // We consider only the matches ID

                    //setID(reader.attributes().value("id").toString());
                } else {
                    reader.skipCurrentElement();
                }

            } else if (reader.name() == "columns") {
                m_state = ColumnsTag;
                m_columns.clear();
            } else if (reader.name() == "column") {
                m_state = ColumnTag;
                m_columns.append(reader.readElementText());
            } else if (reader.name() == "row") {
                m_state = RowTag;
                m_currentRow.clear();
            } else if (reader.name() == "d") {

                QString id = reader.attributes().value("id").toString();
                switch (m_state) {
                case RowTag:
                    m_currentRow.append(reader.readElementText());
                    break;
                case ParametersTag:
                    parameters()[id]=reader.readElementText();
                    break;
                default:
                    //reader.readElementText();
                    break;
                }
            }

        } else if (reader.isEndElement()) {

            //qDebug("End: %s", qPrintable(reader.name().toString()) );

            if (reader.name() == "row")
                m_rows.append(m_currentRow);
        }
    }

    bool ok = !reader.hasError();
    if ( xmlFile ) {
        xmlFile->close();
        delete xmlFile;
    }


    if (ok)
        setOpened(true);
    else
        error()->setError( reader.errorString() );

    return ok;
}

bool NCReportXMLDataSource::close()
{
    if (!isOpened())
        return false;

    setOpened(false);
    flagEnd() = false;
    m_rows.clear();
    m_columns.clear();
    m_currentRow.clear();
    m_state = Off;

    return true;
}

bool NCReportXMLDataSource::hasNext()
{
    return recno() < m_rows.count()-1;
}

bool NCReportXMLDataSource::next(bool isSamplingPageBreak)
{
    Q_UNUSED(isSamplingPageBreak);

    if (recno() < m_rows.count()-1) {
        recno()++;
        return true;
    } else {
        return false;
    }
}

int NCReportXMLDataSource::size() const
{
    return m_rows.size();
}

bool NCReportXMLDataSource::previous()
{
    recno()--;
    if ( recno() < 0 ) {
        recno() = 0;
        return false;
    }
    return true;
}

bool NCReportXMLDataSource::first()
{
    recno()=0;
    return true;
}

bool NCReportXMLDataSource::last()
{
    recno() = m_rows.count()-1;
    if (recno()>0)
        recno()--; // step to the last valid record.
    return true;
}

QVariant NCReportXMLDataSource::value(const QString & columnname, bool* ok, int /*nQTDataRole*/ ) const
{
    if (ok)
        *ok=true;

    int idx = columnIndexByName(columnname);
    if ( idx<0 ) {
        if (ok)
            *ok=false;
        return QVariant();
    } else {
        return value( idx );
    }
}

QVariant NCReportXMLDataSource::value(int column, bool* ok,  int /*nQTDataRole*/ ) const
{
    if (ok) *ok=true;
    if (m_rows.count()==0) {
        if (ok) *ok=false;
        return QVariant();
    }
    if (column > m_columns.count()-1) {
        if (ok) *ok=false;
        return QVariant();
    }
    return QVariant(m_rows[recno()].at(column));
}

bool NCReportXMLDataSource::isValid() const
{
    return (m_rows.count()>0 && (recno() < m_rows.count() && recno() > -1) );
}

int NCReportXMLDataSource::columnIndexByName(const QString &columnname) const
{
    int idx=m_columns.indexOf( columnname );
    if ( idx<0 && columnname.startsWith("col") )
        idx = columnname.mid(3).toInt();
    return idx;
}

bool NCReportXMLDataSource::read(NCReportXMLReader *r)
{
    Q_UNUSED(r);
    return true;
}

bool NCReportXMLDataSource::write(NCReportXMLWriter *w)
{
    Q_UNUSED(w);
    return true;
}

void NCReportXMLDataSource::evaluate(NCReportEvaluator *evaluator)
{
    QString fname = fileName();
    evaluator->evaluate( fname, NCReportItem::Parameter );
    setFileName( fname );
}

int NCReportXMLDataSource::rowCount() const
{
    return m_rows.count();
}

int NCReportXMLDataSource::columnCount() const
{
    return m_columns.count();
}

bool NCReportXMLDataSource::seek( int index )
{
    if ( !isOpened() )
        return false;
    if (index < -1)
        return true;

    if ( index == -1 ) {
    } else if ( index < m_rows.count()) {
        recno() = index;
    } else {
        recno() = m_rows.count()-1;
        return false;
    }
    return true;
}

QStringList NCReportXMLDataSource::keywordList()
{
    if ( columnCount()==0 )
        return QStringList();

    QStringList keywords;
    for ( int i=0; i<m_columns.count(); ++i ) {
        keywords << m_columns.at(i);
        keywords << id()+"."+m_columns.at(i);
//		keywords << "$D{"+m_columns.at(i)+"}";
//		keywords << "$D{"+id()+"."+m_columns.at(i)+"}";
    }
    return keywords;
}
QStringList NCReportXMLDataSource::columnNames()
{
    return m_columns;
}

QString NCReportXMLDataSource::columnName(int column) const
{
    if (column < columnCount())
        return m_columns.at(column);
    else
        return QString();

}
