/****************************************************************************
*
*  Copyright (C) 2002-2012 Helta Kft. / NociSoft Software Solutions
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
#include "ncreportconditionalformat.h"
#include "ncreportevaluator.h"

#include <QTextStream>

#define CF_TAG_COLOR			"color"
#define CF_TAG_BGCOLOR			"background-color"
#define CF_TAG_FAMILY			"font-family"
#define CF_TAG_BOLD				"font-bold"
#define CF_TAG_ITALIC			"font-italic"
#define CF_TAG_WEIGHT			"font-weight"
#define CF_TAG_UNDERLINE		"font-underline"
#define CF_TAG_SIZE				"font-size"
#define CF_TAG_STRIKEOUT		"font-strikeout"
#define CF_TAG_LETTERSPACING	"letter-spacing"
#define CF_TAG_CAPITALIZATION	"capitalization"

NCReportConditionalFormat::NCReportConditionalFormat() : m_fontChanged(false), m_colorChanged(false), m_backColorChanged(false)
{
}

void NCReportConditionalFormat::setFont(const QFont &font)
{
    m_font = font;
}

QFont NCReportConditionalFormat::font() const
{
    return m_font;
}

QFont &NCReportConditionalFormat::fontRef()
{
    return m_font;
}

bool NCReportConditionalFormat::evaluate( const QString &formatCode, NCReportEvaluator *evaluator)
{
    QString fc(formatCode);
    QTextStream in(&fc);
    int lineno=0;
    bool error=false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        lineno++;
        if (line.isEmpty())
            continue;
        if (line.startsWith("$V{")) // variable manipulation expression
            continue;
        QString tag(line.section(':',0,0).trimmed());
        if (tag.isEmpty()) {
            setError(QObject::tr("Invalid tag or missing colon at line %1").arg(lineno));
            error=true;
        }
        QString condition(line.section(':',1,1).trimmed());
        evaluator->evaluate( condition, NCReportItem::DataSource );
        evaluator->evaluate( condition, NCReportItem::Parameter );
        evaluator->evaluate( condition, NCReportItem::Variable );
        if (condition.isEmpty()) // leave default, no formatting
            continue;

        QVariant result;

        if (condition.startsWith('{')) {
            // SCRIPT starts with '{'
            if (!evaluator->evaluateScript(condition, result)) {
                setError(QObject::tr("Error in script expression at line %1: %2").arg(lineno).arg(result.toString()));
                error=true;
            }
        } else {
            // condition is not a script but a constant value or data/parameter/variable
            result = condition;
        }

        if (!evaluateTag(tag, result )) {
            setError(QObject::tr("Invalid tag: %1").arg(tag));
            error=true;
        }
    }

    return !error;
}

QString NCReportConditionalFormat::errorMsg() const
{
    return m_error;
}

bool NCReportConditionalFormat::fontChanged() const
{
    return m_fontChanged;
}

bool NCReportConditionalFormat::colorChanged() const
{
    return m_colorChanged;
}

bool NCReportConditionalFormat::backColorChanged() const
{
    return m_backColorChanged;
}

QColor NCReportConditionalFormat::color() const
{
    return m_color;
}

QColor NCReportConditionalFormat::backColor() const
{
    return m_backColor;
}

bool NCReportConditionalFormat::evaluateTag(const QString &tag, const QVariant &value)
{
    const Qt::CaseSensitivity cs = Qt::CaseInsensitive;
    /*
    const char* m_tags[] = { CF_TAG_COLOR, CF_TAG_BGCOLOR, CF_TAG_FAMILY, CF_TAG_BOLD, CF_TAG_ITALIC, CF_TAG_WEIGHT, CF_TAG_UNDERLINE,
            CF_TAG_SIZE, CF_TAG_STRIKEOUT, CF_TAG_LETTERSPACING, CF_TAG_CAPITALIZATION,0x0 };
    int i=0;
    while (m_tags[i]) {
        QString s(m_tags[i++]);
        if(tag.compare(tag,s,cs)==0) {

        }

    }
    return false;
    */

    bool ok=true;
    if(tag.compare(tag,CF_TAG_COLOR,cs)==0) {
        m_color = QColor(value.toString());
        if (m_color.isValid())
            m_colorChanged = true;
    }
    else if(tag.compare(tag,CF_TAG_BGCOLOR,cs)==0) {
        m_backColor = QColor(value.toString());
        if (m_backColor.isValid())
            m_backColorChanged = true;
    }
    else if(tag.compare(tag,CF_TAG_FAMILY,cs)==0) {
        m_font.setFamily(value.toString());
        m_fontChanged=true;
    }
    else if(tag.compare(tag,CF_TAG_BOLD,cs)==0) {
        m_font.setBold(value.toBool());
        m_fontChanged=true;
    }
    else if(tag.compare(tag,CF_TAG_ITALIC,cs)==0) {
        m_font.setItalic(value.toBool());
        m_fontChanged=true;
    }
    else if(tag.compare(tag,CF_TAG_UNDERLINE,cs)==0) {
        m_font.setUnderline(value.toBool());
        m_fontChanged=true;
    }
    else if(tag.compare(tag,CF_TAG_SIZE,cs)==0) {
        int size=value.toInt();
        if (size>0) {
            m_font.setPointSize(size);
            m_fontChanged=true;
        }
    }
    else if(tag.compare(tag,CF_TAG_WEIGHT,cs)==0) {
        int weight=value.toInt();
        if (weight>0) {
            m_font.setWeight(weight);
            m_fontChanged=true;
        }
    }
    else if(tag.compare(tag,CF_TAG_STRIKEOUT,cs)==0) {
        m_font.setStrikeOut(value.toBool());
        m_fontChanged=true;
    }
    else if(tag.compare(tag,CF_TAG_LETTERSPACING,cs)==0) {
        m_font.setLetterSpacing(QFont::PercentageSpacing,value.toInt());
        m_fontChanged=true;
    }
    else if(tag.compare(tag,CF_TAG_CAPITALIZATION,cs)==0) {
        m_font.setCapitalization((QFont::Capitalization)value.toInt());
        m_fontChanged=true;
    }
    else {
        ok = false;
    }

    return ok;
}

void NCReportConditionalFormat::setError(const QString &errorMsg)
{
    m_error = errorMsg;
}

