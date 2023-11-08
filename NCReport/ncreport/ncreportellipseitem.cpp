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
#include "ncreportellipseitem.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportoutput.h"
#include "ncreportdef.h"

#include <QPainter>

NCReportEllipseItem::NCReportEllipseItem( NCReportDef* rdef, QGraphicsItem * parent ) : NCReportShapeItem( parent )
{
    m_d = new NCReportEllipseData;
    d = m_d;
    d->reportDef = rdef;
    d->tagname = "ellipse";
    d->objectType = Ellipse;
}

NCReportEllipseItem::~ NCReportEllipseItem()
{
}

void NCReportEllipseItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    NCReportScale sc;
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF r( 0, 0, d->currentSize.width(), d->currentSize.height() );

    QPen pen( m_d->pen );
    if (pen.style()==Qt::NoPen)	{
        pen.setColor(Qt::darkGray);
        pen.setStyle(Qt::DotLine);
    }

    pen.setWidthF( sc.mmToPixel(m_d->mLineWidth) );
    painter->setPen( pen );
    painter->setBrush( m_d->brush );
    painter->drawEllipse( r );

    paintSelection(painter);
}

bool NCReportEllipseItem::read(NCReportXMLReader *r )
{
    NCReportShapeItem::read( r );
    r->readElementText();
    return true;
}

bool NCReportEllipseItem::write(NCReportXMLWriter *w)
{
    w->writeStartElement( d->tagname );
    NCReportShapeItem::write(w);
    w->writeEndElement();
    return true;
}

void NCReportEllipseItem::paint( NCReportOutput* o, const QPointF& mPos )
{
    QRectF r = QRectF( o->scale().mmToPoint( mPos + metricPos() ), o->scale().mmToSize( metricSize() ) );

    switch ( o->output() ) {
        case NCReportOutput::Printer:
        case NCReportOutput::Pdf:
        case NCReportOutput::Svg:
        case NCReportOutput::Image:
        case NCReportOutput::Preview:
        case NCReportOutput::QtPreview:
        {
            QPen pen( m_d->pen );
            pen.setWidthF( o->scale().mmToPixel(m_d->mLineWidth) );
            o->painter()->setPen( pen );
            o->painter()->setBrush( m_d->brush );
            o->painter()->drawEllipse( r );
            break;
        }
        default:
            break;
    }
    o->renderItem( this, r );
}

void NCReportEllipseItem::setDefaultForEditor()
{
    m_d->pen = QPen( Qt::black );
    m_d->pen.setWidth(1);
    resize( QSizeF(SELECTION_BOXSIZE*2,SELECTION_BOXSIZE*2) );
}


QRectF NCReportEllipseItem::boundingRect() const
{
    return NCReportItem::boundingRect();
}

void NCReportEllipseItem::setRect(const QRectF & rect)
{
    if (m_d->rect == rect)
        return;
    prepareGeometryChange();
    m_d->rect = rect;
    m_d->boundingRect = QRectF();
    update();
}
