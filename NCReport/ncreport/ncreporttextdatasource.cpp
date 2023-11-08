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
#include "ncreporttextdatasource.h"
#include "ncreportdef.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportevaluator.h"
#include "ncreporthttpclient.h"

#include <QFile>
#include <QTextCodec>
#include <QTextStream>

NCReportTextDataSource::NCReportTextDataSource(QObject * parent) : NCReportDataSource( parent ),
    m_delimiterChar('\t'),
    m_delimiter(Tab),
    m_dataFile(0),
    m_ts(0),
    m_hasColumnHeader(false)
{
    setDataSourceType(Text);
    setLocationType(Static);
}

NCReportTextDataSource::~ NCReportTextDataSource()
{
}

bool NCReportTextDataSource::open(OPEN_MODEL model)
{
    m_OpenModel = model;
    if ( !openStream() )
        return false;

    initColumns();

    setOpened(true);
    return true;
}

bool NCReportTextDataSource::close()
{
    closeStream();
    if ( m_dataFile ) {
        m_dataFile->close();
        delete m_dataFile;
        m_dataFile =0;
    }
    setOpened(false);
    flagEnd() = false;
    m_columns.clear();
    m_rows.clear();
    return true;
}

bool NCReportTextDataSource::openStream()
{
    switch ( locationType() ) {
        case Static:
        case Parameter:
        {
            m_ts = new QTextStream( &specificationRef(), QIODevice::ReadOnly );
            break;
        }
        case File:
        {
            m_dataFile = new QFile( fileName(), this );
            if ( !m_dataFile->open( QIODevice::ReadOnly ) ) {
                if (error())
                    error()->setError( tr("Cannot open file %1").arg( fileName() ) );
                return false;
            }

            m_ts = new QTextStream( m_dataFile );
            m_ts->setDevice( m_dataFile );
            if ( !encoding().isNull() )	{ // set encoding
                QTextCodec *c = QTextCodec::codecForName( encoding().toLatin1() );
                if ( c ) m_ts->setCodec( c );
            }
            break;
        }
        case Http:
        {
            NCReportHttpClient http;
            http.waitForDownloadContent( fileName() );	//download by url
            setSpecification(http.content());
            http.clear();
            m_ts = new QTextStream( &specificationRef(), QIODevice::ReadOnly );
            break;
        }
        case Ftp:
            return false;
    }

    return true;
}

void NCReportTextDataSource::closeStream()
{
    if ( m_ts ) {
        m_ts->setDevice(0);
        delete m_ts;
        m_ts =0;
    }
}

void NCReportTextDataSource::initColumns()
{
    m_columns.clear();
    if ( !m_ts->atEnd() ) {
        if ( m_hasColumnHeader ) { // 1st row is column header
            QString columns = m_ts->readLine();
            m_columns = columns.split( m_delimiterChar, QString::KeepEmptyParts );
            next();	// we must step into the 1st "record"
        } else {
            next();	// we must step into the 1st "record" initially
            if (m_rows.count()>0) {
                int numCols = m_rows[0].count(m_delimiterChar)+1;
                for (int i=0; i<numCols; ++i )
                    m_columns << QString("col%1").arg(i);
            }
        }
    }
}

bool NCReportTextDataSource::hasNext()
{
    return recno() < m_rows.count()-1 || !m_ts->atEnd();
}

bool NCReportTextDataSource::next(bool isSamplingPageBreak)
{
    Q_UNUSED(isSamplingPageBreak);

    if (!m_ts)
        return false;

    if ( m_ts->atEnd() ) {
        // stream ended.
        if (recno() < m_rows.count()-1) {
            recno()++;
            return true;
        } else {
            //recno++;
            //m_rows << QString();
            return false;
        }
    } else {
        //we have more rows in file
        recno()++;
        m_rows << m_ts->readLine();
    }
    return true;
}

int NCReportTextDataSource::size() const
{
    return -1;
}

bool NCReportTextDataSource::previous()
{
    recno()--;
    if ( recno() < 0 ) {
        recno() = 0;
        return false;
    }
    return true;
}

bool NCReportTextDataSource::first()
{
    recno()=0;
    return true;
}

bool NCReportTextDataSource::last()
{
    recno() = m_rows.count()-1;
    if (recno()>0 && m_rows[recno()].isEmpty())
        recno()--; // step to the last valid record.
    return true;
}

QVariant NCReportTextDataSource::value(const QString & columnname, bool* ok, int /*nQTDataRole*/ ) const
{
    int idx = columnIndexByName(columnname);
    return value(idx,ok);
}

QVariant NCReportTextDataSource::value(int column, bool* ok,  int /*nQTDataRole*/ ) const
{
    if (ok)
        *ok=true;
    if (m_rows.count()==0) {
        if (ok)
            *ok=false;
        return QVariant();
    }
    QString val = m_rows[recno()].section( m_delimiterChar, column, column );
    return QVariant(val);
}

QVariant NCReportTextDataSource::value( const QString& columnname, const QString& filter) const
{
    int idx = columnIndexByName(columnname);
    QStringList filterList = filter.split("=");
    int len = filterList.length();
//    for(int i = 0 ; i < len; i++)
//    {
//        filterList[i] = filterList[i].trimmed();
//    }
    if(len == 2)
    {
        QString k = filterList[0];
        QString v = filterList[1];

        if(const_cast<NCReportTextDataSource*>(this)->first())
        {
            //int kidx = columnIndexByName(k);
            do
            {
                if(v == value(k).toString())
                {
                    return value(idx);
                }
            }
            while(const_cast<NCReportTextDataSource*>(this)->next());
        }
//        int row_len = m_rows.count();
//        if(row_len >0)
//        {
//            for (int i = 0; i< row_len; i++)
//            {
//                int kidx = columnIndexByName(k);
//                QString val = m_rows[i].section( m_delimiterChar, kidx, kidx );
//                if(val == v)
//                {
//                    val = m_rows[i].section( m_delimiterChar, idx, idx );
//                    return QVariant(val);
//                }
//            }
//        }
    }
    return QVariant();
}

bool NCReportTextDataSource::isValid() const
{
    return (m_rows.count()>0 && (recno() < m_rows.count() && recno() > -1) );
}

int NCReportTextDataSource::columnIndexByName(const QString &columnname) const
{
    int idx=-1;
    if ( m_hasColumnHeader ) {
        idx = m_columns.indexOf( columnname );
    } else {
        if ( columnname.startsWith("col") )
            idx = columnname.mid(3).toInt();
        else
            idx = columnname.toInt();
    }
    return idx;
}

bool NCReportTextDataSource::read(NCReportXMLReader *reader)
{
    if ( reader->isAttrib("columnDelimiter") ) {
        QStringRef dl = reader->attrib("columnDelimiter");
        if ( dl == "tab" )
            setColumnDelimiter( Tab );
        else if ( dl == "comma" )
            setColumnDelimiter( Comma );
        else if ( dl == "semicolon" )
            setColumnDelimiter( SemiColon );
        else if ( dl == "space" )
            setColumnDelimiter( Space );
        else if ( dl == "vbar" )
            setColumnDelimiter( VBar );
        else if ( dl == "custom" ) {
            setColumnDelimiter( Custom );
            if ( reader->isAttrib("customDelimChar") )
                setCustomDelimiterCharacter( reader->attribToString("customDelimChar")[0].toLatin1() );
        }
    }
    if ( reader->isAttrib("hasColumnHeader") )
        setHasColumnHeader( reader->attribToBool("hasColumnHeader") );
    if ( reader->isAttrib("encoding") )
        setEncoding( reader->attribToString("encoding"));

    return true;
}

bool NCReportTextDataSource::write(NCReportXMLWriter *writer)
{
    QString dl;
    switch (m_delimiter) {
        case Tab: dl="tab"; break;
        case Comma: dl="comma"; break;
        case SemiColon: dl="semicolon"; break;
        case Space: dl="space"; break;
        case VBar: dl="vbar"; break;
        case Custom: dl="custom"; break;
    }
    writer->writeAttribute("columnDelimiter", dl );
    if ( m_delimiter == Custom )
        writer->writeAttribute("customDelimChar", QString(m_delimiterChar) );

    if ( hasColumnHeader() )
        writer->writeAttribute( "hasColumnHeader", "true" );

    if ( !encoding().isEmpty() )
         writer->writeAttribute( "encoding", encoding() );

    return true;
}

void NCReportTextDataSource::evaluate(NCReportEvaluator *evaluator)
{
    QString fname = fileName();
    evaluator->evaluate( fname, NCReportItem::Parameter );
    setFileName( fname );
}

NCReportTextDataSource::Delimiter NCReportTextDataSource::columnDelimiter() const
{
    return m_delimiter;
}

void NCReportTextDataSource::setColumnDelimiter( const Delimiter delim )
{
    m_delimiter = delim;
    switch (delim) {
        case Tab: m_delimiterChar='\t'; break;
        case Comma: m_delimiterChar=','; break;
        case SemiColon: m_delimiterChar=';'; break;
        case Space: m_delimiterChar=' '; break;
        case VBar: m_delimiterChar='|'; break;
        case Custom: break;
    }
}

void NCReportTextDataSource::setCustomDelimiterCharacter(const char d )
{
    m_delimiterChar = d;
}

char NCReportTextDataSource::delimiterCharacter() const
{
    return m_delimiterChar;
}


int NCReportTextDataSource::rowCount() const
{
    return NCReportDataSource::rowCount();
}

int NCReportTextDataSource::columnCount() const
{
    return m_columns.count();
}

bool NCReportTextDataSource::seek( int index )
{
    if ( !isOpened() )
        return false;
    if (index < -1)
        return true;

    if ( index == -1 ) {
        //ts->reset();
        //ts->seek(0);
        recno() = -1;
    } else if ( index < m_rows.count()) {
        recno() = index;
    } else {
        recno() = m_rows.count()-1;
        return false;
    }

    return true;
}

bool NCReportTextDataSource::hasColumnHeader() const
{
    return m_hasColumnHeader;
}

void NCReportTextDataSource::setHasColumnHeader( bool set )
{
    m_hasColumnHeader = set;
}

QStringList NCReportTextDataSource::keywordList()
{
    if (openStream()) {
        initColumns();
        closeStream();
    } else {
        return QStringList();
    }

    QStringList keywords;
    for ( int i=0; i<m_columns.count(); ++i ) {
        keywords << m_columns.at(i);
        keywords << id()+"."+m_columns.at(i);
//		keywords << "$D{"+m_columns.at(i)+"}";
//		keywords << "$D{"+id()+"."+m_columns.at(i)+"}";
    }
    return keywords;
}
QStringList NCReportTextDataSource::columnNames()
{
    if ( columnCount()==0 ) {
        if (openStream()) {
            initColumns();
            closeStream();
        }
    }
    return m_columns;
}

QString NCReportTextDataSource::columnName(int column) const
{
    /*
    if ( columnCount()==0 ) {
        if (openStream()) {
            initColumns();
            closeStream();
        }
    }
    */
    if (column < columnCount())
        return m_columns.at(column);
    else
        return QString();

}
