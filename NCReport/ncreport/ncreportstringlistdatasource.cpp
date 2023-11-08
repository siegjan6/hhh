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
#include "ncreportstringlistdatasource.h"

#include "ncreportdef.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportevaluator.h"

#include <QFile>
#include <QTextCodec>
#include <QTextStream>

NCReportStringListDataSource::NCReportStringListDataSource(QObject * parent) : NCReportDataSource( parent ),
    m_delimiterChar('\t'),
    m_delimiter(Tab)
{
    setDataSourceType(StringList);
    setLocationType(Static);
    recno() =0;
}

NCReportStringListDataSource::~ NCReportStringListDataSource()
{
}

bool NCReportStringListDataSource::open(OPEN_MODEL model)
{
    m_OpenModel = model;
    if ( m_list.isEmpty() ) {
        error()->setError( tr("No StringList added to datasource") );
        return false;
    }
    recno() =0;
    setOpened(true);
    return true;
}

bool NCReportStringListDataSource::close()
{
    recno() =0;
    setOpened(false);
    flagEnd() = false;
    return true;
}

bool NCReportStringListDataSource::hasNext()
{
    return recno() < m_list.count()-1;
}

bool NCReportStringListDataSource::next(bool isSamplingPageBreak)
{
    Q_UNUSED(isSamplingPageBreak);

    recno()++;

    if ( recno() >= m_list.count() ) {
        recno()--;
        flagEnd() = true;
        return false;
    }
    flagBegin() = false;
    return true;
}

int NCReportStringListDataSource::size() const
{
    return m_list.count();
}

int NCReportStringListDataSource::rowCount() const
{
    return m_list.count();
}

bool NCReportStringListDataSource::previous()
{
    recno()--;
    if ( recno() < 0 ) {
        recno() = 0;
        return false;
    }
    return true;
}

bool NCReportStringListDataSource::first()
{
    recno()=0;
    return true;
}

bool NCReportStringListDataSource::last()
{
    recno() = m_list.count()-1;
    return true;
}

QVariant NCReportStringListDataSource::value(const QString & column, bool* ok, int /*nQTDataRole*/ ) const
{
    return value( columnIndexByName(column), ok );
}

QVariant NCReportStringListDataSource::value( int column, bool* ok, int /*nQTDataRole*/ ) const
{
    if (isValid()) {
        if (ok) *ok=true;
    } else 	{
        if (ok) *ok=false;
    }

    QString val = m_list.at(recno()).section( m_delimiterChar, column, column );
    return QVariant(val);
}

bool NCReportStringListDataSource::isValid() const
{
    return (m_list.count()>0);
}

int NCReportStringListDataSource::columnIndexByName(const QString &columnname) const
{
    if ( columnname.startsWith("col") )
        return columnname.mid(3).toInt();
    else
        return columnname.toInt();
}

bool NCReportStringListDataSource::read(NCReportXMLReader *r)
{
    if ( r->isAttrib("columnDelimiter") ) {
        QStringRef dl = r->attrib("columnDelimiter");
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
            if ( r->isAttrib("customDelimChar") )
                setCustomDelimiterCharacter( r->attribToString("customDelimChar")[0].toLatin1() );
        }
    }
    if ( r->isAttrib("listID") )
        setListID( r->attribToString("listID") );
    // sets the mapped stringlist
    if ( !r->reportDef()->stringLists().contains( m_listId ) )
        return false;

    setStringList( r->reportDef()->stringLists().value( m_listId ) );

    return true;
}

bool NCReportStringListDataSource::write(NCReportXMLWriter *w)
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
    w->writeAttribute("columnDelimiter", dl );
    if ( m_delimiter == Custom )
        w->writeAttribute("customDelimChar", QString(m_delimiterChar) );

    w->writeAttribute("listID", m_listId );

    return true;
}

void NCReportStringListDataSource::setColumnDelimiter( const Delimiter delim )
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

NCReportStringListDataSource::Delimiter NCReportStringListDataSource::columnDelimiter() const
{
    return m_delimiter;
}

void NCReportStringListDataSource::setCustomDelimiterCharacter(const char d )
{
    m_delimiterChar = d;
}

char NCReportStringListDataSource::delimiterCharacter() const
{
    return m_delimiterChar;
}

void NCReportStringListDataSource::setStringList(const QStringList & list)
{
    m_list = list;
}

void NCReportStringListDataSource::setListID(const QString & id )
{
    m_listId = id;
}

QString NCReportStringListDataSource::listID() const
{
    return m_listId;
}

bool NCReportStringListDataSource::seek(int index)
{
    bool ok=true;
    if (index<0)
        recno() = 0;
    else if ( index < m_list.count() )
        recno() = index;
    else {
        recno() = m_list.count()-1;
        ok=false;
    }

    return ok;
}

int NCReportStringListDataSource::columnCount() const
{
    int ccount=0;
    if (isValid()) {
        ccount = m_list.at(0).count(m_delimiterChar)+1;
    }
    return ccount;
}


