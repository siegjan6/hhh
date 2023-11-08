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
#include "ncreportsqldatasource.h"
#include "ncreportparameter.h"
#include "ncreportdef.h"
#include "ncreportevaluator.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include <QFileInfo>
#include <QtMath>
#include <QDebug>

//#define SQL_FORWARD_ONLY

NCReportSQLDataSource::NCReportSQLDataSource(QObject * parent) : NCReportDataSource( parent ),
    sql(0), m_recordStatus(BeforeFirst), m_forwardOnly(false), m_samplingSize(0), m_currentSampleRow(0), m_samplePeriod(0), m_nullRecordFlag(false), m_sampleNullGap(0)
{
    setDataSourceType(Sql);
    setLocationType(Static);
}

NCReportSQLDataSource::~ NCReportSQLDataSource()
{
    if ( sql )
        delete sql;
}

bool NCReportSQLDataSource::open(OPEN_MODEL model)
{
    m_OpenModel = model;
    if ( sql || specification().isEmpty() )	// opened already or no query
        return false;

    QSqlDatabase database;

    if ( connectionType() == External ) { //外部数据源,即指定了服务器的IP Port等信息
        // External connection with report built-in parameters
        if ( !connectDatabase( database ) )
            return false;
    } else {
        //NCReport做为dll嵌入到宿主程序中后 通过连接的名称 获得宿主程序已经建立好的连接 故称为内连接
        // internal connection - default or by connectionName
        if ( connectionID().isEmpty() )
            database = QSqlDatabase::database();	// default DB connection
        else
            database = QSqlDatabase::database( connectionID() );
    }
    if ( !database.isValid() ) {
        error()->setError( tr("Database is not connected or invalid in \"%1\" data source.").arg(id()) );
        return false;
    }
    bool ok = true;
    if ( !connOpt.isEmpty() )
        database.setConnectOptions( connOpt );

    sql = new QSqlQuery(database);
    sql->setForwardOnly(m_forwardOnly);

    if (query.isEmpty())
        query = specification();

    if ( !queryIsValid( query ) )	{ // invalid, if not a select statement
        ok = false;
        error()->setError( tr("Not a valid SQL statement: %1").arg(query), sql->lastError().number() );
    }

    if (ok)
        ok = executeQuery();

    if (ok) {
        m_recordStatus = BeforeFirst;	// next() is required to go to the 1st rec.
        setOpened(true);
    } else {

        delete sql;
        sql = 0;

        if ( connectionType() == External )
            disconnectDatabase();
    }

    return ok;
}

bool NCReportSQLDataSource::close()
{
    //if (!sql)
    //	return true;

    if (sql) {
        delete sql;
        sql =0;
    }

    if ( connectionType() == External ) {
        disconnectDatabase();
    }
    setOpened(false);
    return true;
}

bool NCReportSQLDataSource::first()
{
    if (!sql)
        return false;

    if (m_forwardOnly) {
        if ( m_recordStatus == BeforeFirst )
            return next();
        else
            return false;
    } else {
        return sql->first();
    }
}

bool NCReportSQLDataSource::last()
{
    if ( !sql )
        return false;

    bool isLast=false;

    if (m_forwardOnly) {
        if ( !sql->isValid() )
            previous();
        isLast = true;
    } else {
        isLast = sql->last();
    }

    return isLast;
}

bool NCReportSQLDataSource::next(bool isSamplingPageBreak)
{
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("Current record: %i", recno());
#endif

    if ( !sql )
        return false;

    if (m_forwardOnly) {
        // previous record is stored in buffer
        bool ok=true;
        if ( m_recordStatus == Buffer ) {
            // buffer mode
            m_recordStatus = Normal;	// go to normal mode
        } else {
            // normal mode
            saveBuffer();
            ok = sql->next();

            if ( sql->lastError().type() != QSqlError::NoError ) {
                error()->setError( sql->lastError().text(), sql->lastError().number() );
                return false;
            }

            if ( ok )
                m_recordStatus = Normal;	// go to normal mode
            else
                previous();
        }
        return ok;
    } else {
        if(EnableSampling())
        {
            if(isSamplingPageBreak){
                qDebug("page break RecordTime=%s,ms=%d", sql->value(0).toDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz").toUtf8().data(), sql->value(1).toInt());
                bool m_next = m_currentSampleRow < m_samplingSize-1;
                return m_next;
            }
            bool firstRow = false;
            qDebug("RecordTime=%s,ms=%d", sql->value(0).toDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz").toUtf8().data(), sql->value(1).toInt());
            if(m_currentSampleRow == 0) {
                m_currentSampleDt = m_minSampleDt;
                firstRow = true;
            }

            if (m_currentSampleRow >= 0) {
                m_currentSampleDt = m_currentSampleDt.addSecs(m_samplePeriod);
            }

            //m_nullRecordFlag = false;
            QDateTime cdt;
            do{
                QVariant v = sql->value(m_sampleTimeFieldCol);
                if(v.isValid())
                {
                    cdt = v.toDateTime();
                    qDebug("cdt=%s,last_cdt=%s,m_currentSampleDt=%s,m_currentSampleRow=%d", cdt.toString("yyyy-MM-dd HH:mm:ss.zzz").toUtf8().data(), last_cdt.toString("yyyy-MM-dd HH:mm:ss.zzz").toUtf8().data(), m_currentSampleDt.toString("yyyy-MM-dd HH:mm:ss.zzz").toUtf8().data(),m_currentSampleRow);
                    if(cdt == m_currentSampleDt)
                    {
                        m_currentSampleRow++;
                        break;
                    }
                    else if(cdt > m_currentSampleDt)
                    {
                        m_currentSampleRow++;
                        if(!m_nullRecordFlag)
                            sql->previous();
                        break;
                    }
                    // 数据库当前记录时间戳 《 下一个sample时间戳
                    if(!firstRow && m_sampleNullGap >0)
                    {

                        qint64 gapsecs = cdt.secsTo(last_cdt);
//                        if(gapsecs < 0)
//                            m_nullRecordFlag = false;
//                        else
                            if(gapsecs > m_sampleNullGap && m_currentSampleRow >0)
                                m_nullRecordFlag = true;
                            else if(gapsecs < m_sampleNullGap && cdt > last_cdt)
                                m_nullRecordFlag = false;


                        qDebug("m_nullRecordFlag=%d", m_nullRecordFlag);
                    }
                    last_cdt = cdt;
                }
            }while(sql->next());


            if(m_currentSampleRow == 1)
                m_nullRecordFlag = false;

            bool m_next = m_currentSampleRow < m_samplingSize-1;
            if ( !m_next )
                m_currentSampleRow = 0;
            else if(cdt.isValid())
                last_cdt = cdt;
            return m_next;
        }
        else
        {
            bool m_next = sql->next();

            if ( sql->lastError().type() != QSqlError::NoError ) {
                error()->setError( sql->lastError().text(), sql->lastError().number() );
                return false;
            }

            //--------------------------------
            // Go to last record if next() false. - This because other data sources do this.
            //--------------------------------
            // 2011-12-13 : Enable again
            if ( !m_next )
                sql->previous();

            return m_next;
        }
    }
}

bool NCReportSQLDataSource::previous()
{
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("Current record: %i", recno());
#endif

    if ( !sql )
        return false;

    if (m_forwardOnly) {
        // previous record is stored in buffer
        if ( m_recordStatus == Buffer || m_recordStatus == BeforeFirst || m_buffer.isEmpty() )
            return false;

        if (m_recordStatus == Normal)
            m_recordStatus = Buffer;

        return true;
    } else {
        bool m_prev = sql->previous();
        if ( !m_prev )
            sql->next();

        return m_prev;
    }
}

bool NCReportSQLDataSource::seek( int index )
{
    if (index<-1)
        return true;
    if ( !sql )
        return false;

    return sql->seek(index);
}

int NCReportSQLDataSource::size() const
{
    if ( sql )
    {
        if(EnableSampling())
        {
            NCReportSQLDataSource* tmp = const_cast<NCReportSQLDataSource*>(this);
            if(tmp->m_samplingSize > 0)
                return tmp->m_samplingSize;
            QString sf = samplingField();
            QDateTime firstDt;
            QDateTime lastDt;

            if(tmp->last())
            {
                bool ok = false;
                QVariant dt = value(sf, &ok, -999);
                if(dt.isValid())
                {
                    lastDt = dt.toDateTime();
                }
            }

            if(tmp->first())
            {
                bool ok = false;
                QVariant dt = value(sf, &ok, -999);
                if(dt.isValid())
                {
                    firstDt = dt.toDateTime();
                    tmp->m_sampleTimeFieldCol = sql->record().indexOf( sf );
                }
            }

            QDateTime minDt = firstDt;
            QDateTime maxDt = lastDt;
            if(firstDt > lastDt)
            {
                minDt = lastDt;
                maxDt = firstDt;
            }
            tmp->m_minSampleDt = minDt;
            tmp->m_maxSampleDt = maxDt;
            qint64 distance = minDt.secsTo(maxDt);

            QString periodpara = samplingPeriodPara();
            int period = periodpara.toInt();
            QString nullGap = samplingNullGap();
            int nulGapSeconds = nullGap.toInt();
            int size = qCeil(distance*1.0/period);
            tmp->m_samplePeriod = period;
            tmp->m_sampleNullGap = nulGapSeconds;
            tmp->m_samplingSize = size;
            return size;
        }
        else
            return sql->size();
    }
    else
        return 0;
}

bool NCReportSQLDataSource::hasNext()
{
    if ( !sql )
        return false;

    if(EnableSampling())
    {
        return m_currentSampleRow +1 < m_samplingSize;
    }

    bool isNext = sql->next();
    sql->previous();
    if ( !isNext && parentDataSource() ) {
        // End of child query but parent may not
        return parentDataSource()->hasNext();
        //return true;
    }
    return isNext;
}

QVariant NCReportSQLDataSource::value(const QString & column, bool* ok , int nQTDataRole ) const
{
    if ( column == "0" || column == "1" ) {
        // constant values
        if (ok) *ok=true;
        return QVariant("0");
    }
    int pos = sql->record().indexOf( column );
    if ( pos<0 ) {
        qWarning("Column '%s' not found in %s", qPrintable(column),qPrintable(id()) );
        if (ok) *ok=false;
        return QVariant();
    } else {
#ifdef NCREPORT_DEBUG_OUTPUT
    QSqlField field = sql->record().field(pos);
    qDebug("SQL Column: %s | Value type: %i | Value: %s", qPrintable(column), field.type(), qPrintable(value(pos,ok).toString()) );
#endif
        return value(pos,ok,nQTDataRole);
    }
}

QVariant NCReportSQLDataSource::value(int col, bool* ok, int nQTDataRole ) const
{
    if (!sql)
        return QVariant();

    if(EnableSampling() && nQTDataRole != -999)
    {
        if (m_forwardOnly) {

            if ( m_recordStatus == Normal ) {
                if ( sql->isValid() ) {
                    if (ok) *ok=true;
                    return sql->value(col);
                    //return sql->record().field(col).value();
                } else {
                    if (ok) *ok=false;
                    return QVariant();
                }
            } else if (m_recordStatus == Buffer) {
                if ( !m_buffer.isEmpty() ) {
                    if (ok) *ok=true;
                    return m_buffer.value(col);
                } else {
                    if (ok) *ok=false;
                    return QVariant();
                }
            }
        } else {
            if(m_currentSampleRow >= 0)
            {
                if(col == m_sampleTimeFieldCol)
                {
                    // sampling field col,usually RecordTime
                    return m_minSampleDt.addSecs(m_currentSampleRow*m_samplePeriod);
                }
            }

            if ( sql->isValid() ) {
                if (ok) *ok=true;
                if(m_nullRecordFlag && m_currentSampleRow>0)
                {
                    //qDebug()<< m_currentSampleRow << ":" << "888";
                    return QVariant();
                }
                return ( sql->value(col) );
                //return sql->record().field(col).value();
            } else {
                if (ok) *ok=true;  // not false, because empty sql result also acceptable (2010.11.19)
                //return QVariant("NULL");
                return QVariant();
            }
        }

        return QVariant();
    }
    else
    {
        if (nQTDataRole==def_nMarkHeaderDisplayRole)
            return columnName(col);

        if (m_forwardOnly) {

            if ( m_recordStatus == Normal ) {
                if ( sql->isValid() ) {
                    if (ok) *ok=true;
                    return sql->value(col);
                    //return sql->record().field(col).value();
                } else {
                    if (ok) *ok=false;
                    return QVariant();
                }
            } else if (m_recordStatus == Buffer) {
                if ( !m_buffer.isEmpty() ) {
                    if (ok) *ok=true;
                    return m_buffer.value(col);
                } else {
                    if (ok) *ok=false;
                    return QVariant();
                }
            }
        } else {
            if ( sql->isValid() ) {
                if (ok) *ok=true;
                return ( sql->value(col) );
                //return sql->record().field(col).value();
            } else {
                if (ok) *ok=true;  // not false, because empty sql result also acceptable (2010.11.19)
                //return QVariant("NULL");
                return QVariant();
            }
        }

        return QVariant();
    }
}


QVariant NCReportSQLDataSource::value( const QString& columnname, const QString& filter) const
{
    if (!sql)
        return QVariant();

    int idx = columnIndexByName(columnname);
    QStringList filterList = filter.split("=");
    int len = filterList.length();
    if(len == 2)
    {
        QString k = filterList[0];
        QString v = filterList[1];
        //qDebug()<< "filter";
        //qDebug() << k << "--" << v;

        if(const_cast<NCReportSQLDataSource*>(this)->first())
        {
            do
            {
                //qDebug() << value(k);
                if(v == value(k).toString())
                {
                    return value(idx);
                }
            }
            while(const_cast<NCReportSQLDataSource*>(this)->next());
        }
    }

//    int pos = sql->record().indexOf( columnname );
//    if ( pos<0 ) {
//        qWarning("Column '%s' not found in %s", qPrintable(columnname),qPrintable(id()) );
//        return QVariant();
//    } else {
//#ifdef NCREPORT_DEBUG_OUTPUT
//    QSqlField field = sql->record().field(pos);
//    qDebug("SQL Column: %s | Value type: %i | Value: %s", qPrintable(columnname), field.type(), qPrintable(value(pos,ok).toString()) );
//#endif
//        return value(pos,ok,nQTDataRole);
//    }

    return QVariant();
}

bool NCReportSQLDataSource::isValid() const
{
    if (sql)
        return sql->isValid();

    return false;
}

//从XML流中获得数据库连接
bool NCReportSQLDataSource::read( NCReportXMLReader *reader )
{
    if ( reader->isAttrib("host") )
        host = reader->attribToString("host");
    if ( reader->isAttrib("database") )
        dbname = reader->attribToString("database");
    if ( reader->isAttrib("user") )
        user = reader->attribToString("user");
    if ( reader->isAttrib("password") )
        password = reader->attribToString("password");
    if ( reader->isAttrib("port") )
        port = reader->attribToString("port");
    if ( reader->isAttrib("driver") )
        driver = reader->attribToString("driver");

    if ( reader->isAttrib("recordFilter") )
        setRecordFilter(reader->attribToString("recordFilter"));

    if ( reader->isAttrib("connection") ) {
        if ( reader->attrib("connection") == "internal" )
            setConnectionType( NCReportDataSource::Internal );
        else if ( reader->attrib("connection") == "external" )
            setConnectionType( NCReportDataSource::External );
    }
    if ( reader->isAttrib("connID") )
        setConnectionID(reader->attribToString("connID"));

    if ( reader->isAttrib("parentID") )
        setParentId( reader->attribToString("parentID") );
    if ( reader->isAttrib("fwOnly") )
        m_forwardOnly = reader->attribToBool("fwOnly");
    if (reader->isAttrib("connOpt"))
        connOpt = reader->attribToString("connOpt");
    if ( reader->isAttrib("forceUpdateOnStepBackRel") )
        setForceUpdateOnStepBackwordInRelation( reader->attribToBool("forceUpdateOnStepBackRel") );

    return true;
}

bool NCReportSQLDataSource::write(NCReportXMLWriter *writer )
{
    writer->writeAttribute("connection", connectionType() == Internal ? "internal" : "external");
    writer->writeAttribute("parentID", parentId() );
    writer->writeAttribute("connID", connectionID() );
    if ( connectionType() == External ) {
        writer->writeAttribute("host", dbHost() );
        writer->writeAttribute("database", dbDatabase() );
        writer->writeAttribute("user", dbUser() );
        writer->writeAttribute("password", dbPassword() );
        writer->writeAttribute("port", dbPort() );
        writer->writeAttribute("driver", dbDriver() );
    }
    if ( m_forwardOnly )
        writer->writeAttribute("fwOnly", "true" );
    if ( !connOpt.isEmpty() )
        writer->writeAttribute("connOpt", connectOptions() );
    if (forceUpdateOnStepBackwordInRelation())
        writer->writeAttribute("forceUpdateOnStepBackRel", "true" );

    return true;
}


void NCReportSQLDataSource::evaluate(NCReportEvaluator *evaluator)
{
    query = specification();
    evaluator->evaluate( query, NCReportItem::Parameter );	// evaluate parameters in sql query
    evaluator->evaluate( query, NCReportItem::DataSource );	// evaluate datasource values in sql query

    QString cID = connectionID();
    evaluator->evaluate( cID, NCReportItem::Parameter );	// evaluate parameters in connection ID
    setConnectionID(cID);

    if(EnableSampling())
    {
        QString sp = samplingPeriodPara();
        evaluator->evaluate( sp, NCReportItem::Parameter );	// evaluate parameters in sql query
        evaluator->evaluate( sp, NCReportItem::DataSource );	// evaluate datasource values in sql query
        setSamplingPeriodPara(sp);
    }
}

// void NCReportSQLDataSource::setDatabase( const QSqlDatabase & db )
// {
// 	database = db;
// }

QString NCReportSQLDataSource::dbHost() const
{
    return host;
}

QString NCReportSQLDataSource::dbDatabase() const
{
    return dbname;
}

QString NCReportSQLDataSource::dbUser() const
{
    return user;
}

QString NCReportSQLDataSource::dbPassword() const
{
    return password;
}

QString NCReportSQLDataSource::dbPort() const
{
    return port;
}

void NCReportSQLDataSource::setSqlQuery(const QString & qry)
{
    query = qry;
}

QString NCReportSQLDataSource::sqlQuery() const
{
    return query;
}

void NCReportSQLDataSource::setHost(const QString & h)
{
    host = h;
}

void NCReportSQLDataSource::setDB(const QString & db)
{
    dbname = db;
}

void NCReportSQLDataSource::setPort(const QString & p)
{
    port = p;
}

void NCReportSQLDataSource::setPassword(const QString & p)
{
    password = p;
}

void NCReportSQLDataSource::setUser(const QString & u)
{
    user = u;
}

void NCReportSQLDataSource::setDriver(const QString & dr)
{
    driver = dr;
}

QString NCReportSQLDataSource::dbDriver() const
{
    return driver;
}


bool NCReportSQLDataSource::connectDatabase( QSqlDatabase& database )
{
    // SQL/DATABASE CONNECTION IF EXTERNAL
    //QString m_connectID = connID.isEmpty() ? id() : connID;

    if (QSqlDatabase::contains(privateConnectionID())) {
        // This connect ID is already used!
        error()->setError( QObject::tr("Database connection ID %1 is already used.").arg(privateConnectionID()) );
        return false;
    }
    bool isSqlServer = false;
    if(driver == "SQLServer")
    {
        driver == "QODBC";
        isSqlServer = true;
    }
    database = QSqlDatabase::addDatabase(driver, privateConnectionID() );

    if ( !database.isValid() ) {
            //fprintf( stderr, "Error: Could not load database driver. \n" );
        error()->setError( QObject::tr("Could not load database driver: %1").arg(driver) );
        return false;
    }
    database.setHostName( host );
    QString db(dbname);

    if ( db.contains("{FILE}")) {
        db.replace("{FILE}","");
        QFileInfo info(db);
        db = info.absoluteFilePath();
    }

    if(isSqlServer)
    {
        QString dsn = QString("DRIVER={SQL SERVER};SERVER=%1;port=%2;DATABASE=%3;uid=%4;pwd=%5;")
            .arg(host)
            .arg(dbPort().isEmpty() ? 1433 : dbPort().toInt())
            .arg(privateConnectionID())
            .arg(user)
            .arg(password);
        database.setDatabaseName(dsn);
        // access
        //        dsn = "DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=myaccessfile.mdb"
    }
    else
        database.setDatabaseName( db );
    database.setUserName( user );
    database.setPassword( password );
    if (!dbPort().isEmpty())
        database.setPort(dbPort().toInt());

    if ( !database.open() ) {
        error()->setError( database.lastError().databaseText() );
        disconnectDatabase();
        return false;
    }

    return true;
}

bool NCReportSQLDataSource::disconnectDatabase()
{
    QString m_connectID( privateConnectionID() );
    if (QSqlDatabase::contains(m_connectID))
        QSqlDatabase::removeDatabase(m_connectID);

    return true;
}

int NCReportSQLDataSource::rowCount() const
{
    return size();
}

int NCReportSQLDataSource::columnCount() const
{
    if (sql )
        return sql->record().count();
    else
        return NCReportDataSource::columnCount();
}

bool NCReportSQLDataSource::queryIsValid( const QString& qry )
{
    QString q( qry.trimmed().toUpper() );
    if (qry.isEmpty())
        return false;
    const char* denied_keywords[] = {"INSERT","UPDATE","DELETE","TRUNCATE",0};
    int i=0;
    bool invalid=false;
    while (denied_keywords[i]) {
        if (q.startsWith(denied_keywords[i++])) {
            invalid=true;
            break;
        }
    }
    return !invalid;
}

bool NCReportSQLDataSource::update()
{
    last_cdt = QDateTime::fromString("1970-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss");
    if (!sql && !isOpened())
        return open(OPEN_MODEL_QUERY);

    return executeQuery();
}

QStringList NCReportSQLDataSource::keywordList()
{
    return NCReportDataSource::keywordList();
}

QStringList NCReportSQLDataSource::columnNames()
{
    QStringList words =  specText().split(QRegExp("\\W+"), QString::SkipEmptyParts); //txt.split(QRegExp("\\s+"));
    QString word;
    QStringList keywords, badWords;
    badWords << "select" << "as" << "show";

    for ( int i=0; i<words.count(); ++i ) {
            word =  words.at(i);
            if ( word.compare("from", Qt::CaseInsensitive )==0 )
                    break;
            if ( badWords.contains(word,Qt::CaseInsensitive) )
                    continue;	//"select"
            if ( keywords.contains(word,Qt::CaseInsensitive) )
                    continue;
            if ( word.size() > 1 && word[0].isDigit() )
                    continue;

            keywords << word;
    }

    return keywords;
}

bool NCReportSQLDataSource::isForwardOnly() const
{
    return m_forwardOnly;
}

void NCReportSQLDataSource::setForwardOnly(bool set)
{
    m_forwardOnly = set;
}

void NCReportSQLDataSource::saveBuffer()
{
    m_buffer = sql->record();
}

QString NCReportSQLDataSource::privateConnectionID() const
{
    return connectionID() + "_" + id();
}

bool NCReportSQLDataSource::executeQuery()
{
    QString p_query(query);
    p_query.replace("{ID}", foreignKeyValue());
    p_query.replace("{PK}", foreignKeyValue());
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("Execute SQL: %s", qPrintable(p_query));
#endif
    bool ok = sql->exec( p_query );
    if (!ok)
        error()->setError( sql->lastError().text() + "\n" + p_query, sql->lastError().number() );

    return ok;
}

QSqlQuery* NCReportSQLDataSource::queryObject()
{
    return sql;
}

bool NCReportSQLDataSource::hasColumn(int column) const
{
    if (sql )
        return (column>=0 && column < sql->record().count());

    return false;
}

bool NCReportSQLDataSource::hasColumn(const QString &columnname) const
{
    if (sql ) {
        int pos = sql->record().indexOf( columnname );
        return (pos>=0);
    }
    return false;
}

int NCReportSQLDataSource::columnIndexByName(const QString &columnname) const
{
    if (sql)
        return sql->record().indexOf( columnname );
    return -1;
}

void NCReportSQLDataSource::setConnectOptions(const QString & opt)
{
    connOpt = opt;
}

QString NCReportSQLDataSource::connectOptions() const
{
    return connOpt;
}

QString NCReportSQLDataSource::columnName(int column) const
{
    if (sql )
        return sql->record().fieldName(column);

    return QString();
}

