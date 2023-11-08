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
#include "ncreportdatasource.h"
#include "ncreportutils.h"
#include "ncreportdatafieldinfo.h"
//#include "ncreportxmlreader.h"
//#include "ncreportxmlwriter.h"
//#include <QDir>

NCReportDataSource::NCReportDataSource(QObject * parent) : QObject( parent ),
  m_dataSourceType(Custom),
  m_connectionType(Internal),
  m_locationType(Static),
  m_error(0),
  m_recno(0),
  m_flagEnd(false),
  m_flagBegin(false),
  m_allowEmpty(true),
  m_enableSampling(false),
  m_samplingField(""),
  m_samplingPeriodPara(""),

  m_specification(""),
  m_recordFilter(""),
  m_fileName(""),

  m_openRole(BeginReport),
  m_parentID(""),
  m_done(false),
  m_opened(false),
  m_available(true),
  m_parent(0),
  m_encoding("UTF-8"),
  m_pkColumn(0),
  m_fKey(QString()),
  m_useAppearanceRoles(false),
  m_forceUpdateOnStepBackwordInRelation(false),
  m_OpenModel(OPEN_MODEL_QUERY)
{
}

NCReportDataSource::~NCReportDataSource()
{
}

NCReportDataSource::DataSourceType NCReportDataSource::dataSourceType() const
{
    return m_dataSourceType;
}

NCReportDataSource::ConnectionType NCReportDataSource::connectionType() const
{
    return m_connectionType;
}

void NCReportDataSource::setErrorHandler(NCReportError *e )
{
    m_error = e;
}

void NCReportDataSource::setEncoding(const QString & encoding)
{
    m_encoding = encoding;
}

QString NCReportDataSource::encoding() const
{
    return m_encoding;
}

void NCReportDataSource::setDataSourceType(DataSourceType dsType)
{
    m_dataSourceType = dsType;
}

void NCReportDataSource::setConnectionType(ConnectionType cType)
{
    m_connectionType = cType;
}

void NCReportDataSource::setID(const QString & dsId )
{
    m_id = dsId;
}

void NCReportDataSource::setId(const QString & dsId)
{
    m_id = dsId;
}

void NCReportDataSource::setConnectionID(const QString & id)
{
    m_connectionID = id;
}

QString NCReportDataSource::connectionID() const
{
    return m_connectionID;
}

void NCReportDataSource::setBriefDescription( const QString& description)
{
   m_briefDescription = description;
}

QString NCReportDataSource::briefDescription() const
{
   return m_briefDescription;
}

void NCReportDataSource::setLongDescription( const QString& description)
{
   m_longDescription = description;
}

QString NCReportDataSource::longDescription() const
{
   return m_longDescription;
}

void NCReportDataSource::setRecordFilter(const QString & filter)
{
    m_recordFilter = filter;
}

void NCReportDataSource::setFileName(const QString & fileName)
{
    m_fileName = fileName;
}

QString NCReportDataSource::id() const
{
    return m_id;
}

NCReportDataSource::LocationType NCReportDataSource::locationType() const
{
    return m_locationType;
}

QString NCReportDataSource::fileName() const
{
    QString fn(m_fileName);
    NCReportUtils::evaluateFileName(fn);
    return fn;
}

QString NCReportDataSource::rawFileName() const
{
    return m_fileName;
}

QString NCReportDataSource::recordFilter() const
{
    return m_recordFilter;
}

QString NCReportDataSource::specText() const
{
    return m_specification;
}

void NCReportDataSource::setLocationType(LocationType ltype )
{
    m_locationType = ltype;
}

void NCReportDataSource::setSpecText(const QString & txt)
{
    m_specification = txt;
}

void NCReportDataSource::setSpecification(const QString & txt)
{
    m_specification = txt;
}

bool NCReportDataSource::read(NCReportXMLReader * reader)
{
    Q_UNUSED(reader)
    return true;
}

bool NCReportDataSource::write(NCReportXMLWriter *writer)
{
    Q_UNUSED(writer)
    return true;
}

void NCReportDataSource::setViewPath(const QString & viewPath)
{
    Q_UNUSED(viewPath)
}

/*!
 * \brief NCReportDataSource::rowCount
 * \return number of data rows (records)
 */
int NCReportDataSource::rowCount() const
{
    return -1;
}

/*!
 * \brief NCReportDataSource::columnCount
 * \return number of data columns
 */
int NCReportDataSource::columnCount() const
{
    return -1;
}

/*!
 * \brief NCReportDataSource::allowEmpty
 * \return if empty data is allowed or not
 */
bool NCReportDataSource::allowEmpty()
{
    return m_allowEmpty;
}

/*!
 * \brief NCReportDataSource::setAllowEmpty
 * \param set
 * sets if empty data is allowed or not
 */
void NCReportDataSource::setAllowEmpty(bool set)
{
    m_allowEmpty = set;
}

// enable sampling
void NCReportDataSource::setEnableSampling( bool set)
{
    m_enableSampling = set;
}
bool NCReportDataSource::EnableSampling() const
{
    return m_enableSampling;
}
// samplingField
void NCReportDataSource::setSamplingField( const QString& field )
{
    m_samplingField = field;
}
QString NCReportDataSource::samplingField() const
{
    return m_samplingField;
}
// samplingPeriodPara
void NCReportDataSource::setSamplingPeriodPara( const QString& para )
{
    m_samplingPeriodPara = para;
}
QString NCReportDataSource::samplingPeriodPara() const
{
    return m_samplingPeriodPara;
}

// samplingNullGap  seconds
void NCReportDataSource::setSamplingNullGap( const QString& gap )
{
    m_samplingNullGap = gap;
}
QString NCReportDataSource::samplingNullGap() const
{
    return m_samplingNullGap;
}


void NCReportDataSource::setDone(bool set)
{
    m_done = set;
}

bool NCReportDataSource::isDone() const
{
    return m_done;
}

bool NCReportDataSource::isOpened() const
{
    return m_opened;
}

void NCReportDataSource::setParentId(const QString& parentId )
{
    m_parentID = parentId;
}

QString NCReportDataSource::parentId() const
{
    return m_parentID;
}

void NCReportDataSource::setParentDataSource( NCReportDataSource* ds )
{
    m_parent = ds;
}

NCReportDataSource* NCReportDataSource::parentDataSource() const
{
    return m_parent;
}

void NCReportDataSource::setOpenRole( OpenRoles role )
{
    m_openRole = role;
}

NCReportDataSource::OpenRoles NCReportDataSource::openRole() const
{
    return m_openRole;
}

void NCReportDataSource::setAvailable(bool set)
{
    m_available = set;
}

bool NCReportDataSource::isAvailable() const
{
    return m_available;
}

int NCReportDataSource::currentRow() const
{
    return m_recno;
}

int NCReportDataSource::recno() const
{
    return m_recno;
}

const QList<int> &NCReportDataSource::appearanceRoles() const
{
    return m_appearanceRoles;
}

QString NCReportDataSource::specification() const
{
    return m_specification;
}

bool NCReportDataSource::update()
{
    emit updateRequestFKey( id(), foreignKeyValue() );
    return true;
}

void NCReportDataSource::evaluate(NCReportEvaluator * evaluator)
{
    Q_UNUSED(evaluator)
}

/*!
  The list of reference keywords of data columns. This is primary used by the designer.
  */
QStringList NCReportDataSource::keywordList()
{
    QStringList colnames = columnNames();
    QStringList keywords = columnNames();
    for ( int i=0; i<colnames.count(); ++i ) {
        keywords << id()+"."+colnames.at(i);
//        keywords << "$D{"+colnames.at(i)+"}";
//        keywords << "$D{"+id()+"."+colnames.at(i)+"}";
    }
    return keywords;
}

/*!
  The list of available data columns.
  */
QStringList NCReportDataSource::columnNames()
{
    QStringList cnames;
    for (int i=0; i<columnCount(); ++i) {
        QString column = columnName(i);
        if (!column.isEmpty())
            cnames << column;
    }

    return cnames;
}

QVector<NCReportDataFieldInfo> NCReportDataSource::fieldInfo() const
{
    return QVector<NCReportDataFieldInfo>();
}

QString NCReportDataSource::columnName(int column) const
{
    return QString("col%1").arg(column);
}

QString NCReportDataSource::columnTitle(int column) const
{
    return columnName(column);
}

QString NCReportDataSource::rowTitle(int /*row*/) const
{
    return QString();
}

bool NCReportDataSource::hasColumn(int column) const
{
    return (column>=0 && column<columnCount()-1);
}

bool NCReportDataSource::hasColumn(const QString &columnname) const
{
    for (int i=0; i<columnCount(); ++i) {
        if (columnName(i) == columnname)
            return true;
    }
    return false;
}

int NCReportDataSource::columnIndexByName(const QString &columnname) const
{
    for (int i=0; i<columnCount(); ++i) {
        if (columnName(i) == columnname)
            return i;
    }
    return -1;
}

bool NCReportDataSource::hasParameters() const
{
    return !m_parameters.isEmpty();
}

QHash<QString, QVariant> &NCReportDataSource::parameters()
{
    return m_parameters;
}

QVariant NCReportDataSource::getUserFunctionValue(const QVariant& value, const QString &arguments) const
{
    Q_UNUSED(value)
    Q_UNUSED(arguments)
    return QVariant();
}

QString NCReportDataSource::columnToolTip( int column ) const
{
    return columnName(column);
}

void NCReportDataSource::setOpened(bool set)
{
    m_opened = set;
}

NCReportError *NCReportDataSource::error()
{
    return m_error;
}

int &NCReportDataSource::recno()
{
    return m_recno;
}

bool &NCReportDataSource::flagEnd()
{
    return m_flagBegin;
}

bool &NCReportDataSource::flagBegin()
{
    return m_flagEnd;
}

QString &NCReportDataSource::specificationRef()
{
    return m_specification;
}


/*!
 * \brief NCReportDataSource::addAppearanceRole
 * \param role
 * Adds a new role that will be considered as an appeareance item role instead of the default template settings.
 */
void NCReportDataSource::addAppearanceRole(int role)
{
    m_appearanceRoles.append(role);
}

void NCReportDataSource::setUseAppearanceRoles(bool set)
{
    m_useAppearanceRoles = set;
}

bool NCReportDataSource::useAppearanceRoles() const
{
    return m_useAppearanceRoles;
}

void NCReportDataSource::setPrimaryKeyColumn(int column)
{
    m_pkColumn = column;
}

int NCReportDataSource::primaryKeyColumn() const
{
    return m_pkColumn;
}

void NCReportDataSource::setForeignKeyValue(const QString &key)
{
    m_fKey = key;
}

QString NCReportDataSource::foreignKeyValue() const
{
    return m_fKey;
}

void NCReportDataSource::setForceUpdateOnStepBackwordInRelation(bool set)
{
    m_forceUpdateOnStepBackwordInRelation = set;
}

bool NCReportDataSource::forceUpdateOnStepBackwordInRelation() const
{
    return m_forceUpdateOnStepBackwordInRelation;
}

