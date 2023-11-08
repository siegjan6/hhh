/****************************************************************************
* 
*  Copyright (C) 2007-2010 Helta Kft. / NociSoft Software Solutions
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
#include "ncreportbarcoderenderer.h"

#include <QPainter>

//NCReportBarcodeRenderer::NCReportBarcodeRenderer()
//{
//	xml.setAutoFormatting(true);
//}

NCReportBarcodeRenderer::NCReportBarcodeRenderer( QPainter* painter, const QByteArray & data, const QRectF & rect )
	: QXmlStreamReader( data ), m_painter(painter), m_paintRect(rect), m_rate(0), m_showText(true), m_error(false)
{
	m_error = data.isEmpty() || data.startsWith("error");
	if (m_error)
		m_errorMsg = data.isEmpty() ? QObject::tr("Barcode error") : data;
	m_baseTag="svg";

}

NCReportBarcodeRenderer::~NCReportBarcodeRenderer()
{
}


bool NCReportBarcodeRenderer::render( bool showText )
{
	if (m_error) {
		paintError();
		return false;
	}
	m_painter->save();
	m_showText = showText;

	while (!atEnd()) {
		readNext();

		if (isStartElement()) {
			if (name() == m_baseTag )
				renderContents();
			else
				raiseError( QObject::tr("Invalid SVG stream") );
		}
	}

	m_painter->restore();
	return !error();
}

void NCReportBarcodeRenderer::renderContents()
{
	Q_ASSERT(isStartElement() && name() == m_baseTag);

	m_svgSize.setWidth( attributes().value("width").toString().toInt() );
	m_svgSize.setHeight( attributes().value("height").toString().toInt() );

	m_rate = m_paintRect.width()/m_svgSize.width();

	if (m_svgSize.height()*m_rate > m_paintRect.height()) // barcode is clipped -> fix the width to height
		m_rate/=(m_svgSize.height()*m_rate/m_paintRect.height());

	m_painter->setPen( Qt::NoPen );
	m_painter->setClipRect( m_paintRect.adjusted(0,0,1,1) );
	QColor fill( Qt::black );

	while (!atEnd()) {
		readNext();

		if (isEndElement())
			break;

		if (isStartElement()) {
			if (name() == "g") {

				if ( !attributes().value("fill").isEmpty() )
					fill = QColor( attributes().value("fill").toString() );

			} else if (name() == "desc") {
				readElementText();
			} else if (name() == "rect") {

				QRectF r(rc("x"), rc("y"), rc("width"), rc("height") );
				QColor bg(fill);
				if ( !attributes().value("fill").isEmpty() )
					bg = QColor( attributes().value("fill").toString() );

				paintElement( r, bg );
				readElementText();

			} else if (name() == "text" && m_showText )  {

				QPointF p(rc("x"), rc("y") );
				QString family( attributes().value("font-family").toString() );
				qreal fontSize = attributes().value("font-size").toString().toDouble();
				QColor color(Qt::black);
				if ( !attributes().value("fill").isEmpty() )
					color = QColor( attributes().value("fill").toString() );

				paintText( readElementText(), p, family, fontSize, color );


			}
		}
	}
}

qreal NCReportBarcodeRenderer::rc( const char* name ) const
{
	return attributes().value(name).toString().toDouble();
}

void NCReportBarcodeRenderer::paintElement( QRectF &elementRect, const QColor& fillColor )
{
	m_painter->setBrush( fillColor );
	elementRect.setRect( elementRect.x()*m_rate, elementRect.y()*m_rate, elementRect.width()*m_rate, elementRect.height()*m_rate );
	elementRect.translate( m_paintRect.topLeft() );
	m_painter->drawRect(elementRect);
}

void NCReportBarcodeRenderer::paintText( const QString& text, QPointF &elementPoint, const QString &fontFamily, qreal fontSize, const QColor& color )
{
	QString m_text(text.trimmed());
	QFont font( fontFamily );
	font.setPixelSize( fontSize*m_rate );
	QFontMetrics fm(font, m_painter->device() );

	m_painter->setFont( font );
	m_painter->setPen( QPen(color,1) );
	m_painter->setBrush( Qt::NoBrush );
	elementPoint.rx()*=m_rate;
	elementPoint.ry()*=m_rate;
	elementPoint.rx()-=fm.width(m_text)/2;		// fix middle x position
	//elementPoint.ry()-=fm.height()/2;		// fix middle y position
	elementPoint += m_paintRect.topLeft();

	m_painter->drawText( elementPoint, m_text );
}

void NCReportBarcodeRenderer::paintError()
{
	m_painter->setFont( QFont("Arial",8) );
	m_painter->setPen( QPen(Qt::black,1) );
	m_painter->drawText( m_paintRect, Qt::AlignCenter | Qt::TextWordWrap, m_errorMsg );
}


