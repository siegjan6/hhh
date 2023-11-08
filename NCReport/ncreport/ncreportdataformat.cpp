/****************************************************************************
*
*  Copyright (C) 2002-2015 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2015. 01. 07. (nszabo)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncreportdataformat.h"
#include "ncreportfielditem.h"

#include <QDate>
#include <QDateTime>

NCReportDataFormat::NCReportDataFormat(NCReportData::DataType dt, const QVariant &value):
    m_type(dt),
    m_value(value),

    m_formatNum(false),
    m_localized(false),
    m_fieldwidth(0),
    m_format('f'),
    m_precision(-1),
    m_fillchar(QChar(' ')),
    m_blankIfZero(false),
    m_numChars(0),

    m_dateFormat( QString() ),
    m_arg( QString() ),
    m_alignment( Qt::AlignLeft )
{
}

NCReportDataFormat::~NCReportDataFormat()
{
}

QString NCReportDataFormat::formattedData()
{
    QString result;
    formatData(result);
    return result;
}

void NCReportDataFormat::formatData(QString &result)
{
    switch ( m_type ) {
    case NCReportData::Text:
        formatString(result);
        break;
    case NCReportData::Numeric:
        formatNumber(result);
        break;
    case NCReportData::Date:
        formatDate(result);
        break;
    case NCReportData::DateTime:
        formatDateTime(result);
        break;
    case NCReportData::Boolean:
       formatBool(result);
       break;
    case NCReportData::Image:
    case NCReportData::Custom:
        break;
    }

    if ( !m_arg.isEmpty() && m_arg.contains('%') )
        result = m_arg.arg(result);

    if (m_numChars>0) {
        QFlags<Qt::AlignmentFlag> flags(m_alignment);
        if (flags.testFlag(Qt::AlignLeft))
            result = result.leftJustified(m_numChars,m_fillchar,false);
        else if (flags.testFlag(Qt::AlignRight))
            result = result.rightJustified(m_numChars,m_fillchar,false);
    }
}

void NCReportDataFormat::formatField(NCReportFieldItem *field)
{
    NCReportDataFormat df(field->dataType(), field->valueRef());

    df.setNumberIsFormatted(field->isNumFormat());
    df.setLocalized(field->isLocalized());
    df.setFieldWidth(field->fieldWidth());
    df.setNumericFormat(field->format());
    df.setPrecision(field->precision());
    df.setFillChar(field->fillChar());
    df.setBlankIfZero(field->isNumBlankIfZero());
    df.setDateFormat(field->dateFormat());
    df.setArg(field->arg());
    df.setNumChars(field->numCharacters());
    df.setAlignment(field->alignment());

    field->setDisplayValue(df.formattedData());
}

void NCReportDataFormat::formatString(QString &result)
{
    result = m_value.toString();
}

void NCReportDataFormat::formatNumber(QString &result)
{
    if ( m_blankIfZero && m_value.toDouble() == 0 ) {
        result = "";
        return;
    }
    // formatting
    if ( m_formatNum )	{
        QString tmp;
        if ( m_localized )
            tmp = "%L1";
        else
            tmp = "%1";
        result = tmp.arg( m_value.toDouble(), m_fieldwidth, m_format, m_precision, m_fillchar );
    } else {
        result = m_value.toString();
    }
}

void NCReportDataFormat::formatDate(QString &result)
{
    QDate d = m_value.toDate();
    if ( m_dateFormat.isEmpty() )
        result = d.toString( Qt::DefaultLocaleShortDate );
    else
        result = d.toString( m_dateFormat );
}

void NCReportDataFormat::formatDateTime(QString &result)
{
    switch (m_value.type()) {
    case QVariant::Double:
    case QVariant::Int:
    case QVariant::LongLong:
    case QVariant::UInt:
    case QVariant::ULongLong:
    {
        QTime t(0,0);
        t = t.addSecs(m_value.toInt());
        result = t.toString(m_dateFormat);
        break;
    }
    default:
    {
        QDateTime dt;
        dt = m_value.toDateTime();
        if ( m_dateFormat.isEmpty() )
            result = dt.toString( Qt::DefaultLocaleShortDate );
        else
            result = dt.toString( m_dateFormat );
        break;
    }
    }
}

void NCReportDataFormat::formatBool(QString &result)
{
    result = ( m_value.toBool() ? QObject::tr("true") : QObject::tr("false") );
}

