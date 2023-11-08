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
#include "ncreportfielditem.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportoutput.h"
#include "ncreportdef.h"
#include "ncreportevaluator.h"
#include "ncreportdatasource.h"

#include <QPainter>
#include <QDate>

NCReportFieldItem::NCReportFieldItem(NCReportDef* rdef, QGraphicsItem * parent) : NCReportLabelItem( NCReportItem::Field, rdef, parent )
{
    NCReportFieldData *fd = new NCReportFieldData;
    d = fd;
    setReportDef(rdef);
    d->objectType = Field;
    d->tagname = "field";
    d->setDataType( NCReportData::Text );
    setSourceType(DataSource);
    setBackColor(QColor(210,210,255));	// back in designer
}

NCReportFieldItem::~ NCReportFieldItem()
{
}

bool NCReportFieldItem::read(NCReportXMLReader *reader )
{
    return NCReportLabelItem::read( reader );
}

bool NCReportFieldItem::write(NCReportXMLWriter *writer )
{
    return NCReportLabelItem::write( writer );
}

void NCReportFieldItem::writeProps(NCReportXMLWriter *w )
{
    NCReportFieldData* fd = (NCReportFieldData*)d;
    w->writeAttribute("type", fd->typeName() );
    QString dst;
    // compatibility resons:
    switch ( sourceType() ) {
    case DataSource: dst="ds"; break;
    case Parameter: dst="par"; break;
    case Variable: dst="var"; break;
    case SystemVariable: dst="sys"; break;
    case ScriptExpression: dst="exp"; break;
    case Template: dst="tpl"; break;
    default: break;
    }
    if (!dst.isNull())
        w->writeAttribute("ftype", dst );

    if ( dataType() == NCReportData::Numeric ) {
        if ( fd->formatnum )
            w->writeAttribute("formatting", "true" );

        if ( fd->formatnum ) {
            w->writeAttribute("numwidth", QString::number(fd->fieldwidth) );
            w->writeAttribute("format", QString(fd->format) );
            w->writeAttribute("precision", QString::number(fd->precision) );
            //w->writeAttribute("fillchar", QString(fd->fillchar) );
            w->writeAttribute("localized", fd->localized ? "true" : "false" );
        }
        if ( fd->numBlankIfZero )
            w->writeAttribute("blankifzero", "true" );
    } else if ( fd->dataType() == NCReportData::Date || fd->dataType() == NCReportData::DateTime ) {
        w->writeAttribute("dateFormat", fd->dateFormat );
    }

    if ( !fd->fillchar.isNull() && !fd->fillchar.isSpace() )
        w->writeAttribute("fillchar", fd->fillchar );

    if ( !fd->arg.isEmpty() )
        w->writeAttribute("arg", fd->arg );
    if ( fd->numChars > 0 )
        w->writeAttribute("numChars", QString::number(fd->numChars) );

}

void NCReportFieldItem::readProps(NCReportXMLReader *r)
{
    NCReportFieldData* fd = (NCReportFieldData*)d;
    if ( r->isAttrib("type") )
        fd->setDataType(  r->attributes().value("type").toString() );

    if ( r->isAttrib("ftype") ) {
        QStringRef s = r->attributes().value("ftype");
        if ( s == "sql" || s=="ds" )
            setSourceType(DataSource);
        else if ( s == "par" )
            setSourceType(Parameter);
        else if ( s == "var" )
            setSourceType(Variable);
        else if ( s == "sys" )
            setSourceType(SystemVariable);
        else if ( s == "exp" )
            setSourceType(ScriptExpression);
        else if ( s == "tpl" )
            setSourceType(Template);
        else
            setSourceType(DataSource);
    }
    if ( r->isAttrib("formatting") )
        fd->formatnum = r->attribToBool("formatting");
    if ( r->isAttrib("numwidth") )
        fd->fieldwidth = r->attribToInt("numwidth");
    if ( r->isAttrib("format") )
        fd->format = r->attribToString("format")[0].toLatin1();
    if ( r->isAttrib("precision") )
        fd->precision = r->attribToInt("precision");
    if ( r->isAttrib("fillchar") )
        fd->fillchar = r->attribToString("fillchar")[0].toLatin1();

    if ( r->isAttrib("dateFormat") )
        fd->dateFormat = r->attribToString("dateFormat");
    if ( r->isAttrib("localized") )
        fd->localized = r->attribToBool("localized");
    if ( r->isAttrib("blankifzero") )
        fd->numBlankIfZero = r->attribToBool("blankifzero");
    if ( r->isAttrib("arg") )
        fd->arg = r->attribToString("arg");

    if ( r->isAttrib("numChars") )
        fd->numChars = r->attribToInt("numChars");
}

void NCReportFieldItem::setDefaultForEditor()
{
    NCReportLabelItem::setDefaultForEditor();
}

void NCReportFieldItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    NCReportFieldData* fd = (NCReportFieldData*)d;
    QRectF rect( QPointF(0, 0), d->currentSize );

    painter->setFont( font() );

    int aflag = alignment();
    if ( isWordBreak() )
        aflag |= Qt::TextWordWrap; //Qt::TextWrapAnywhere
    // BACKGROUND IN DESIGNER

    QPen m_pen(Qt::red);
    m_pen.setStyle(Qt::DotLine);

    if(borderVisible()){
        m_pen = borderPen();
        NCReportScale sc;
        m_pen.setWidthF( sc.mmToRealPixel(lineWidth()) );
    }

    painter->setPen( m_pen );
    if ( backgroundMode() == FilledBackground || isBackgroundToDesign() )
        painter->setBrush( QBrush( backColor() ) );
    painter->save();
    painter->setPen(borderPen());
    painter->drawRect( rect );
    painter->restore();
    rect.setLeft( rect.left()+1 );
    painter->setPen( QPen(foreColor(),1) );
    QString textToPrint = NCReportLabelItem::text();

    if (rotation()!=0) {
        painter->save();
        painter->setClipRect(rect);
        rotateText( painter, QPointF(0,0), screenRect().size() );
    }

    painter->drawText( rect, aflag, ( fd->description.isEmpty() ? textToPrint : fd->description + " ("+textToPrint+")" ));

    if (rotation()!=0)
        painter->restore();

    paintSelection(painter);
}

void NCReportFieldItem::paint( NCReportOutput* o, const QPointF& mPos )
{
    NCReportLabelItem::paint(o,mPos);
}

void NCReportFieldItem::adjustSize()
{
    // Do nothing. we should not adjust Fields
}

bool NCReportFieldItem::isEmpty() const
{
    NCReportFieldData* fd = (NCReportFieldData*)d;
    bool empty = false;

    switch ( fd->dataType() ) {
        case NCReportData::Text:
        case NCReportData::Numeric:
        case NCReportData::Date:
        case NCReportData::DateTime:
            empty = fd->displayValue.isEmpty();
            break;
        case NCReportData::Boolean:
            empty = false;
            break;
        case NCReportData::Image:
        case NCReportData::Custom:
            empty = !fd->valueRef().isValid();
            break;
    }

    if (!empty && hideIfRepeated()) {
        // HIDE REPEATED TEXT
        empty = ( fd->displayValue == fd->lastValue );
        fd->lastValue = fd->displayValue;	//save last
    }

    if(!fd->value().isNull())
        fd->lastNoneNullValue = fd->displayValue;

    if(useLastNoneNull())
    {
        // use last none null value
        if(fd->value().isNull())
            fd->displayValue = fd->lastNoneNullValue;
    }

    return empty;
}

//void NCReportFieldItem::swapLastValue(SwapMode mode)
//{
//    NCReportFieldData* fd = (NCReportFieldData*)d;
//    QString tmp;

//    if ( mode == LastValue ) {
//        tmp = fd->lastDisplayValue;
//        fd->lastDisplayValue = fd->displayValue;
//        fd->displayValue = tmp;
//    } else if ( mode == OriginalValue ) {
//        tmp = fd->displayValue;
//        fd->displayValue = fd->lastDisplayValue;
//        fd->lastDisplayValue = tmp;
//    }

//}

void NCReportFieldItem::updateValue(NCReportOutput* output, NCReportEvaluator *evaluator)
{
    evaluator->evaluateField( this );
    NCReportLabelItem::updateValue(output, evaluator);
}

QList<int> NCReportFieldItem::availableSourceTypes() const
{
    QList<int> types;
    types << NCReportItem::DataSource << NCReportItem::Parameter
                      << NCReportItem::Variable << NCReportItem::SystemVariable
                      << NCReportItem::ScriptExpression << NCReportItem::Template;
    return types;
}
