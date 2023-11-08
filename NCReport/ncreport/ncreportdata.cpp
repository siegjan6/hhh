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
#include "ncreportdata.h"
#include "ncreportutils.h"

NCReportData::NCReportData()
{
    m_type = Custom;
    m_id="0";
}


NCReportData::NCReportData( DataType t ) : m_type( t )
{
    m_id="0";
}

NCReportData::~NCReportData()
{
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("Delete reportdata: %s", qPrintable(m_id) );
#endif
}

void NCReportData::setValue( const QVariant& v, DataType t)
{
    m_value = v;
    m_type = t;
}

void NCReportData::setValue( const QVariant& v )
{
    m_value = v;
}

QVariant NCReportData::value() const
{
    return m_value;
}

NCReportData::DataType NCReportData::dataType() const
{
    return m_type;
}

void NCReportData::setID( const QString& id)
{
    if (id=="%") {
        m_id = NCReportUtils::randomString(5);
    } else {
        m_id=id;
    }
}

QString NCReportData::id() const
{
    return m_id;
}

void NCReportData::setDataType(DataType t)
{
    m_type = t;
}

QVariant & NCReportData::valueRef()
{
    return m_value;
}


QString NCReportData::typeName() const
{
    switch ( m_type ) {
        case Numeric: return "num";
        case Text: return "txt";
        case Date: return "date";
        case DateTime: return "datetime";
        case Boolean: return "bool";
        case Image: return "img";
        case Custom: return "cust";
    }
    return "cust";
}

void NCReportData::setDataType(const QString & tname)
{
    if ( tname.toLower()=="num" )
        m_type = Numeric;
    if ( tname.toLower()=="txt" )
        m_type = Text;
    if ( tname.toLower()=="date" )
        m_type = Date;
    if ( tname.toLower()=="datetime" )
        m_type = DateTime;
    if ( tname.toLower()=="bool" )
        m_type = Boolean;
    if ( tname.toLower()=="img" )
        m_type = Image;
    if ( tname.toLower()=="cust" )
        m_type = Custom;
}
