/****************************************************************************
*
*  Copyright (C) 2002-2011 Helta Kft. / NociSoft Software Solutions
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
#include "ncreportcrosstabitemcell.h"
#include "ncreportoutput.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportdataformat.h"

#include <QFontMetricsF>
#include <QPainter>
#include <math.h>

static const double Pi = 3.141592653;

NCReportCrossTabItemCell::NCReportCrossTabItemCell() : 	m_sizeMM(QSizeF()), m_align(Qt::AlignLeft | Qt::AlignVCenter),
	m_font(QFont()), m_foreColor(Qt::black), m_brush(QBrush()),
	m_formatString(QString()), m_wordbreak(false),
	m_value(0), m_fieldWidth(0), m_format('f'), m_precision(-1), m_blankIfZero(false), m_localized(false),
	m_fillChar(QLatin1Char(' ')),
	m_cellType(Data),m_dataType(NCReportData::Text),
	m_backgroundMode(Qt::TransparentMode), m_size(QSizeF()), m_rotation(0)
{

}

NCReportCrossTabItemCell::~NCReportCrossTabItemCell()
{
}

/*
void NCReportCrossTabItemCell::paint(NCReportOutput *o, const QPointF &mPos)
{
	QRect rect( o->scale().mmToPixel( mPos.x() + d->mPos.x() ),
			 o->scale().mmToPixel( mPos.y() + d->mPos.y() ),
			 o->scale().mmToPixel( d->mSize.width() ),
			   o->scale().mmToPixel( d->mSize.height() ) );

}
*/

void NCReportCrossTabItemCell::paint(NCReportOutput *o, const QRectF& rect, const qreal padding)
{
	switch ( o->output() ) {
		case NCReportOutput::Printer:
		case NCReportOutput::Pdf:
		case NCReportOutput::Svg:
		case NCReportOutput::Image:
		case NCReportOutput::Preview:
		case NCReportOutput::QtPreview:
			break;
		default:
			return;
	}

	//if ( m_dataType == NCReportData::Numeric && m_blankIfZero && m_value.toDouble()== 0 )
	//	return;

	QFont font(m_font);
	//adjustFont( font, o );
	o->painter()->setFont( font );

	int aflag = m_align;
	if ( m_wordbreak )
		aflag |= Qt::TextWordWrap;

	if ( m_backgroundMode == Qt::OpaqueMode ) {
		o->painter()->setPen( Qt::NoPen );
		o->painter()->setBrush( m_brush );
		o->painter()->drawRect( rect );
	}

	QString cellText;
	switch ( m_cellType ) {
	case CornerHeader:
	case SideSummaryHeader:
	case BottomSummaryHeader:
		cellText = m_text;
		break;
	default:
		{
			cellText = m_value.toString();

            NCReportDataFormat fmt(m_dataType,m_value);
            fmt.setNumberIsFormatted(m_formatNum);
            fmt.setLocalized(m_localized);
            fmt.setFieldWidth(m_fieldWidth);
            fmt.setNumericFormat(m_format);
            fmt.setPrecision(m_precision);
            fmt.setFillChar(m_fillChar);
            fmt.setBlankIfZero(m_blankIfZero);
            fmt.setDateFormat(m_formatString);
            fmt.setArg(m_formatString);
            fmt.setAlignment(m_align);
            fmt.formatData(cellText);

            break;
		}
	}

	QRectF textRect(rect);

	if (padding>0)
		textRect.adjust(padding,0,-padding,0);

    if ( m_dataType == NCReportData::Numeric && m_text=="NB" && m_value.toDouble() < 0 )
    {
        cellText = "N.B.";
    }

	if ( m_dataType == NCReportData::Boolean && m_text=="." ) {
		if (m_value.toBool()) {
			qreal r = o->scale().mmToPixel(0.75);
			o->painter()->setPen(Qt::NoPen);
			o->painter()->setBrush( QBrush(m_foreColor) );
			o->painter()->drawEllipse( rect.topLeft()+QPointF(rect.width()/2,rect.height()/2), r,r );
		}

	} else {

		o->painter()->setPen( QPen(m_foreColor) );

		if ( m_rotation != 0) {
			o->painter()->save();
			rotateText( o->painter(), rect );
		}
		if (!cellText.isEmpty())
			o->painter()->drawText( textRect, aflag, cellText );
		if ( m_rotation != 0)
			o->painter()->restore();
	}

	// borders
	for ( int border=0; border<4; ++border)
		paintBorder((BorderPos)border, o, rect );

}

void NCReportCrossTabItemCell::paintBorder( BorderPos pos,  NCReportOutput* o, const QRectF& rect )
{
	if ( m_borders[pos].enabled ) {

		QLineF line;
		switch (pos) {
		case TopBorder: line = QLineF(rect.topLeft(), rect.topRight()); break;
		case RightBorder: line = QLineF(rect.topRight(), rect.bottomRight()); break;
		case BottomBorder: line = QLineF(rect.bottomRight(), rect.bottomLeft()); break;
		case LeftBorder: line = QLineF(rect.bottomLeft(), rect.topLeft()); break;
		}

        m_borders[pos].pen.setWidthF( qMax(o->scale().mmToRealPixel(m_borders[pos].lineWidth),(qreal)1.0) );
		o->painter()->setPen(m_borders[pos].pen);
		o->painter()->drawLine( line );
	}
}


QSizeF NCReportCrossTabItemCell::sizeHint( const QRectF& constrainedRect, QPaintDevice* device) const
{
	QFontMetricsF fm( m_font, device );

	int aflag = m_align;
	if ( m_wordbreak )
		aflag |= Qt::TextWordWrap;
	QRectF rect(fm.boundingRect( constrainedRect, aflag, m_value.toString() ));
	//rect.setHeight( rect.height() + m_o->scale().mmToPixel(LABEL_HEIGHT_TRIM_MARGIN_MM));		//TRIM
	return rect.size();
}

bool NCReportCrossTabItemCell::read(NCReportXMLReader *r)
{
	bool ok = true;

	qDebug("NCReportCrossTabItemCell::read %p",this);

	if ( r->isAttrib("dt") )
		m_dataType = (NCReportData::DataType)r->attribToInt("dt");
	m_align = r->readAlignment();
	r->readFont(m_font);
	r->readColor("foreColor",m_foreColor);
	r->readBrush(m_brush);

	if (r->isAttrib("formatString"))
		m_formatString = r->attribToString("formatString");
	if (r->isAttrib("wordbreak"))
		m_wordbreak = r->attribToBool("wordbreak");

	if (r->isAttrib("formatting"))
		m_formatNum = r->attribToBool("formatting");
	if (r->isAttrib("formatNum"))
		m_formatNum = r->attribToBool("formatNum");
	if (r->isAttrib("fieldWidth"))
		m_fieldWidth = r->attribToInt("fieldWidth");
	if (r->isAttrib("format"))
		m_format = r->attribToString("format")[0].toLatin1();
	if (r->isAttrib("precision"))
		m_precision = r->attribToInt("precision");
	if (r->isAttrib("fillChar"))
		m_fillChar = r->attribToString("fillChar")[0].toLatin1();
	if (r->isAttrib("localized"))
		m_localized = r->attribToBool("localized");
	if (r->isAttrib("blankIfZero"))
		m_blankIfZero = r->attribToBool("blankIfZero");

	if ( r->isAttrib("bgMode") )
		m_backgroundMode = (Qt::BGMode)r->attribToInt("bgMode");
	if ( r->isAttrib("rotation") )
		m_rotation = r->attribToReal("rotation");

	while (!r->atEnd()) {
		r->readNext();
		if (r->isEndElement())
			break;

		if (r->isStartElement()) {
			if (r->name() == "border")
				ok = readBorder(r);
			else if (r->name() == "text")
				m_text = r->readElementText();
			else
				r->readUnknownElement();
		}
	}

	return ok;
}

bool NCReportCrossTabItemCell::readBorder(NCReportXMLReader *r)
{
	//qDebug("NCReportCrossTabItemCell::readBorder");
	int borderID = r->attribToInt("pos");
	m_borders[borderID].enabled = r->attribToBool("enabled");
	r->readPen(m_borders[borderID].pen);
	m_borders[borderID].lineWidth = r->readLineWidth();
	r->readElementText();
	return true;
}

bool NCReportCrossTabItemCell::writeBorder(BorderPos pos, NCReportXMLWriter *w)
{
	//if (m_borders[pos].enabled) {
		w->writeStartElement("border");
		w->writeInt("pos", (int)pos );
		w->writeBool("enabled", m_borders[pos].enabled );
		w->writePen(m_borders[pos].pen);
		w->writeLineWidth( m_borders[pos].lineWidth );
		w->writeEndElement();
	//}
	return true;
}

bool NCReportCrossTabItemCell::write(NCReportXMLWriter *w)
{
	w->writeStartElement("cell");

	w->writeInt("ct", m_cellType );
	w->writeInt("dt", m_dataType );
	w->writeAlignment(m_align);
	w->writeFont(m_font);
	w->writeBrush(m_brush);
	if (!m_formatString.isEmpty())
		w->writeAttribute("formatString", m_formatString );
	if (m_wordbreak)
		w->writeBool("wordbreak",true);
	w->writeAttribute("foreColor",m_foreColor.name());


	if (m_dataType == NCReportData::Numeric && m_formatNum) {
		w->writeAttribute("formatting", "1");
		if (m_fieldWidth>0)
			w->writeInt("fieldWidth", m_fieldWidth );
		w->writeAttribute("format", QString(m_format) );
		if ( m_precision>=0)
			w->writeInt("precision", m_precision );
		if (m_fillChar != QLatin1Char(' '))
			w->writeAttribute("fillChar", QString(m_fillChar) );
		if (m_blankIfZero)
			w->writeAttribute("blankIfZero", "1" );
		if (m_localized)
			w->writeAttribute("localized", "1" );
	}

	// Background, style
	if (m_backgroundMode == Qt::OpaqueMode)
		w->writeInt("bgMode", m_backgroundMode );
	if (m_rotation!=0)
		w->writeReal("rotation", m_rotation );

	if ( !m_text.isEmpty() )
		w->writeTextElement("text", m_text);

	for ( int i=0; i < 4; ++i )
		writeBorder((BorderPos)i,w);

	//w->writeCharacters( i.value() );

	w->writeEndElement();

	return true;
}

NCReportCrossTabItemCell::CellBorder & NCReportCrossTabItemCell::border(const BorderPos i)
{
	return m_borders[(int)i];
}

void NCReportCrossTabItemCell::rotateText( QPainter* p, const QRectF& rect )
{
	QPointF offsetPoint( rect.bottomRight() );

	offsetPoint.rx()-=rect.height()*sin(m_rotation*Pi/180)/2;
	offsetPoint.ry()-=rect.height()*sin(m_rotation*Pi/180)/2;
	p->translate( offsetPoint );
	p->rotate(m_rotation);
	p->translate( -offsetPoint );
}
