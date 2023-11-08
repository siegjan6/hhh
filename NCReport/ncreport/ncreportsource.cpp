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
#include "ncreportsource.h"
#include "ncreportdef.h"
#include "ncreportxmldefreader.h"
#include "ncreportxmldefwriter.h"
#include "ncreportutils.h"

#include <QXmlStreamReader>
#include <QTime>
#include <QFile>
#ifndef NCREPORT_NO_SQL
#include <QSqlQuery>
#include <QSqlError>
#endif
#include <QBuffer>

NCReportSource::NCReportSource() :  m_type(File), rdef(0), m_id(0)
{
}

NCReportSource::~ NCReportSource()
{
}

void NCReportSource::setReportDef(NCReportDef *r )
{
    rdef = r;
}

void NCReportSource::setSourceType( NCReportSource::ReportSourceType t )
{
    m_type = t;
}

NCReportSource::ReportSourceType NCReportSource::sourceType() const
{
    return m_type;
}

NCReportDef * NCReportSource::reportDef()
{
    return rdef;
}

void NCReportSource::setIntegerReportID(int i )
{
    m_id = i;
}

int NCReportSource::integerReportID() const
{
    return m_id;
}

void NCReportSource::setStringReportID(const QString & i)
{
    m_idS = i;
}

QString NCReportSource::stringReportID() const
{
    return m_idS;
}

void NCReportSource::setFileName(const QString & fn )
{
    m_fileName = fn;
    NCReportUtils::saveFilePath("REPORT_PATH", fn); // save the report path
}

QString NCReportSource::fileName() const
{
    return m_fileName;
}

/*
void NCReportSource::setLoadQuery(const QString & q)
{
    m_reportLoadQuery = q;
}

QString NCReportSource::loadQuery() const
{
    return m_reportLoadQuery;
}

void NCReportSource::setSaveQuery(const QString & q)
{
    m_reportSaveQuery = q;
}

QString NCReportSource::saveQuery() const
{
    return m_reportSaveQuery;
}
*/

bool NCReportSource::load( NCReportXMLDefReader& reader )
{
    if ( m_type == String ) {	// For report def it's added as parameter
        if (m_reportDefXML.isEmpty()) {
            rdef->error()->setError( QObject::tr("Report definition XML string is undefined.") );
            return false;
        }
    } else
        m_reportDefXML = QString();

    bool ok = true;

    switch ( m_type ) {
        case File: ok = loadFromFile( reader ); break;
        case Database: ok = loadFromDatabase( reader ); break;
        case Http: ok = loadFromHttp( reader ); break;
        case Ftp: ok = loadFromFtp( reader ); break;
        case Custom: ok = loadFromCustomSource( reader ); break;
        case String: ok = loadFromString( reader ); break;
    }

    if (ok) {
        evaluateParameters();
    }

    return ok;
}

/*
bool NCReportSource::loadXML(NCReportXMLDefReader &reader,QMap<QString,QStringList> &info)
{
    if ( m_type == String ) {	// For report def it's added as parameter
        if (m_reportDefXML.isEmpty()) {
            rdef->error()->setError( QObject::tr("Report definition XML string is undefined.") );
            return false;
        }
    } else
        m_reportDefXML = QString();

    bool ok = true;

    switch ( m_type ) {
        case File: ok = loadFromXmlFile( reader ,info); break;
        case Database: ok = loadFromDatabase( reader ); break;
        case Http: ok = loadFromHttp( reader ); break;
        case Ftp: ok = loadFromFtp( reader ); break;
        case Custom: ok = loadFromCustomSource( reader ); break;
        case String: ok = loadFromString( reader ); break;
    }

    if (ok) {
        evaluateParameters();
    }

    return ok;
}
*/



bool NCReportSource::save( NCReportXMLDefWriter & writer )
{
    bool ok = true;
    switch ( m_type ) {
        case File: ok = saveToFile( writer ); break;
        case Database: ok = saveToDatabase( writer ); break;
        case Http: ok = saveToHttp( writer ); break;
        case Ftp: ok = saveToFtp( writer ); break;
        case Custom: ok = saveToCustomSource( writer ); break;
        case String: break;
    }

    return ok;
}

void NCReportSource::addParameter(const QString &id, const QString &value)
{
    m_parameters[id]= value;
}

QString NCReportSource::getParameter(const QString &id)
{
    if(m_parameters.contains(id))
        return m_parameters[id];
    return QString::null;
}

bool NCReportSource::loadFromFile(NCReportXMLDefReader& reader )
{
    QFile file;
    file.setFileName( m_fileName );
    if ( !file.open( QIODevice::ReadOnly ) ) {
        rdef->error()->setError(  QObject::tr("Cannot open file %1").arg( m_fileName ) );
        return false;
    }
    bool ok = reader.read(&file, QString::null );
    if (!ok)
        setParseError( reader );
    return ok;
}

/*
/////////////////////////////////////////////////////////////////////////////////////
/// \brief NCReportSource::loadFromXmlFile
/// \param reader
/// \return
///
bool NCReportSource::loadFromXmlFile(NCReportXMLDefReader& reader ,QMap<QString,QStringList> &info)
{
    QFile file;
    file.setFileName( m_fileName );
    if ( !file.open( QIODevice::ReadOnly ) ) {
        rdef->error()->setError(  QObject::tr("Cannot open file %1").arg( m_fileName ) );
        return false;
    }
    bool ok = reader.readXml(&file, QString::null ,info);
    if (!ok)
        setParseError( reader );
    return ok;
}
/////////////////////////////////////////////////////////////////////////////////////
*/

bool NCReportSource::loadFromDatabase(NCReportXMLDefReader& reader)
{
#ifndef NCREPORT_NO_SQL
    if ( loadQuery().isEmpty() ) {
        rdef->error()->setError( QObject::tr("Report load sql query is undefined.") );
        return false;
    }

    QSqlDatabase database;
    if ( m_connectionID.isEmpty() )
        database = QSqlDatabase::database();	// defaul DB
    else
        database = QSqlDatabase::database( m_connectionID );

    QSqlQuery sql( database );

    //QSqlQuery sql;

    if ( !sql.exec( loadQuery() ) ) {
        rdef->error()->setError( sql.lastError().text() );
        return false;
    }
    if ( sql.size() <= 0 ) {
        rdef->error()->setError( QObject::tr("No report definition found!") );
        return false;
    }
    sql.next();
    m_reportDefXML = sql.value(0).toString();
    m_reportDefXML.replace( '\012', '\n');  // fix postgres encoding problem

    if ( m_reportDefXML.isEmpty() ) {
        rdef->error()->setError( QObject::tr("Report definition is empty!") );
        return false;
    }
    bool ok = loadFromString( reader );

    return ok;
#else
    Q_UNUSED(reader);
    rdef->error()->setError( QObject::tr("NCReports not built with SQL support") );
    return false;
#endif

}

bool NCReportSource::loadFromString(NCReportXMLDefReader& reader)
{
    bool ok = reader.read( 0, m_reportDefXML );
    if (!ok)
        setParseError( reader );
    return ok;
}

bool NCReportSource::loadFromHttp(NCReportXMLDefReader& )
{
    return true;
}

bool NCReportSource::loadFromFtp(NCReportXMLDefReader& )
{
    return true;
}

bool NCReportSource::loadFromCustomSource(NCReportXMLDefReader& )
{
    return true;
}

void NCReportSource::setParseError(NCReportXMLDefReader& reader)
{
    rdef->error()->setError( QObject::tr("Parse error in file %1 at line %2, column %3:\n%4")
                             .arg(m_fileName)
                             .arg(reader.lineNumber())
                             .arg(reader.columnNumber())
                             .arg(reader.errorString()));
}

QString NCReportSource::loadQuery() const
{
    QString qry;
    if (m_columnName.isEmpty() || m_tableName.isEmpty())
        return qry;

    qry += "SELECT "+ m_columnName + " FROM "+m_tableName+" WHERE "+m_keyColumn+"='" + m_keyValue+"'";
    return qry;
}

QString NCReportSource::saveQuery() const
{
    QString qry;
    if (m_columnName.isEmpty() || m_tableName.isEmpty())
        return qry;
    qry += "UPDATE "+ m_tableName + " SET "+m_columnName+"=? WHERE "+m_keyColumn+"='" + m_keyValue+"'";
    return qry;
}

void NCReportSource::evaluateParameters()
{
    QHash<QString, QString>::const_iterator i = m_parameters.constBegin();
    while (i != m_parameters.constEnd()) {
        m_reportDefXML.replace( QString("{{%1}}").arg(i.key()), i.value());
        m_reportDefXML.replace( QString("$P{%1}").arg(i.key()), i.value());
        ++i;
    }

}

void NCReportSource::setReportDefXML(const QString & xmlstring )
{
    m_reportDefXML = xmlstring;
}

QString NCReportSource::reportDefXML() const
{
    return m_reportDefXML;
}

bool NCReportSource::saveToFile(NCReportXMLDefWriter& writer )
{
    QFile file(m_fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        rdef->error()->setError( QObject::tr("Cannot write file %1:\n%2.").arg(m_fileName).arg(file.errorString()) );
        return false;
    }
    writer.setCodec(reportDef()->option().encoding().toLocal8Bit());
    bool ok = writer.write( &file );
    return ok;
}

bool NCReportSource::saveToDatabase(NCReportXMLDefWriter& writer)
{
#ifndef NCREPORT_NO_SQL
    if ( m_reportDefXML.isEmpty() ) {
        rdef->error()->setError( QObject::tr("Report definition is empty!") );
        return false;
    }

    if ( saveQuery().isEmpty() ) {
        rdef->error()->setError( QObject::tr("Report save sql query is undefined.") );
        return false;
    }
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    writer.setCodec(reportDef()->option().encoding().toLocal8Bit());
    bool ok = writer.write( &buffer );

    QSqlDatabase database;
    if ( m_connectionID.isEmpty() )
        database = QSqlDatabase::database();	// defaul DB
    else
        database = QSqlDatabase::database( m_connectionID );

    QSqlQuery sql( database );

    if ( sql.prepare( saveQuery() ) ) {
        QString query = byteArray;  // workaround of PostgreSQL encoding issue
        sql.addBindValue( query );
    }
    if ( !sql.exec() ) {
        rdef->error()->setError( sql.lastError().text() );
        return false;
    }
    return ok;
#else
    Q_UNUSED(writer);
    rdef->error()->setError( QObject::tr("NCReport not built with SQL support") );
    return false;
#endif
}

bool NCReportSource::saveToHttp(NCReportXMLDefWriter&)
{
    return true;
}

bool NCReportSource::saveToFtp(NCReportXMLDefWriter&)
{
    return true;
}

bool NCReportSource::saveToCustomSource(NCReportXMLDefWriter&)
{
    return true;
}

bool NCReportSource::saveToString(NCReportXMLDefReader &)
{
    return true;
}

void NCReportSource::setConnectionID(const QString & id)
{
    m_connectionID = id;
}

QString NCReportSource::connectionID() const
{
    return m_connectionID;
}


