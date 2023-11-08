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
#include "ncreportrectitem.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportoutput.h"
#include "ncreportdef.h"

#include <QPainter>

NCReportRectItem::NCReportRectItem( NCReportDef* rdef, QGraphicsItem * parent ) : NCReportShapeItem( parent )
{
    d = new NCReportRectData;
    d->reportDef = rdef;
    d->tagname = "rectangle";
    d->objectType = Rectangle;
}

NCReportRectItem::~ NCReportRectItem()
{
}

void NCReportRectItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    NCReportScale sc;
    Q_UNUSED(option);
    Q_UNUSED(widget);

    NCReportRectData* rd = (NCReportRectData*)d;
    QRectF r( 0, 0, d->currentSize.width(), d->currentSize.height() );

    QPen pen( rd->pen );
    if (pen.style()==Qt::NoPen)	{
        pen.setColor(Qt::darkGray);
        pen.setStyle(Qt::DotLine);
    }
    pen.setWidthF( sc.mmToPixel(rd->mLineWidth) );
    painter->setPen( pen );
    painter->setBrush( rd->brush );
    if ( rd->xRound >0 || rd->yRound>0 )
        painter->drawRoundRect( r, rd->xRound, rd->yRound );
    else
        painter->drawRect( r );

    paintSelection(painter);
}

bool NCReportRectItem::read(NCReportXMLReader *r )
{
    NCReportRectData* rd = (NCReportRectData*)d;

    NCReportShapeItem::read( r );
    if ( r->isAttrib("roundX") )
            rd->xRound = r->attribToInt("roundX");
    if ( r->isAttrib("roundY") )
            rd->yRound = r->attribToInt("roundY");
    r->readElementText();

    return true;
}

bool NCReportRectItem::write(NCReportXMLWriter *w)
{
    NCReportRectData* rd = (NCReportRectData*)d;

    w->writeStartElement( d->tagname );
    NCReportShapeItem::write(w);

    if ( rd->xRound > 0 )
        w->writeAttribute("roundX", QString::number(rd->xRound) );
    if ( rd->yRound > 0 )
        w->writeAttribute("roundY", QString::number(rd->yRound) );

    w->writeEndElement();
    return true;
}

void NCReportRectItem::paint( NCReportOutput* o, const QPointF& mPos )
{
    NCReportRectData* rd = (NCReportRectData*)d;
    QRectF r;
    QPointF pos(o->scale().mmToPoint( mPos + metricPos()));

    if ( isAutoHeight() ) {
        int section_height = o->scale().mmToPixel(sectionMetricSize().height() - metricPos().y());
        int zone_height = o->scale().mmToPixel(zoneMetricSize().height() );
        qreal topX = pos.x();
        qreal topY = pos.y();
        qreal width = o->scale().mmToPixel( metricSize().width());
        qreal height = ( rd->fitRole == FitHeightToZone ? zone_height : section_height ); // expand to section/zone height

        if (boundingRectOfLastPart().isValid())	// rect around pabe break text
            r = QRectF( topX, boundingRectOfLastPart().y(), width, boundingRectOfLastPart().height() );
        else
            r = QRectF( topX, topY, width, height );
    } else {
        r = QRectF( pos, o->scale().mmToSize( metricSize() ) );
    }

    if (pinToRight())
        r.setRight( o->scale().mmToRealPixel(reportDef()->option().pageWidthMM() - reportDef()->option().rightMarginMM()) );


    switch ( o->output() ) {
        case NCReportOutput::Printer:
        case NCReportOutput::Pdf:
        case NCReportOutput::Svg:
        case NCReportOutput::Image:
        case NCReportOutput::Preview:
        case NCReportOutput::QtPreview:
        {
            QPen pen( rd->pen );
            qreal m_penWidth = o->scale().mmToPixel(rd->mLineWidth);
            if (m_penWidth<1.0)
#if QT_VERSION>=0x050000
                pen.setWidthF(0.0);
#else
                pen.setWidthF(1.0);
#endif
            else
                pen.setWidthF( m_penWidth );

            o->painter()->setPen( pen );
            o->painter()->setBrush( rd->brush );
            if ( rd->xRound >0 || rd->yRound>0 )
                o->painter()->drawRoundRect ( r, rd->xRound, rd->yRound );
            else
                o->painter()->drawRect( r );

            break;
        }
        default:
            break;
    }
    o->renderItem( this, r );
}

void NCReportRectItem::setDefaultForEditor()
{
    NCReportRectData* rd = (NCReportRectData*)d;
    //update( 0, 0, s.mmToPixel(30.0), s.mmToPixel(0.5) );
    rd->pen = QPen( Qt::black );
    rd->pen.setWidth(1);
    //if ( currentSize().width() < SELECTION_BOXSIZE*6) )
        resize( QSizeF(SELECTION_BOXSIZE*6,SELECTION_BOXSIZE*6) );
}


QRectF NCReportRectItem::boundingRect() const
{
    return NCReportItem::boundingRect();
    /*
    NCReportRectData* rd = (NCReportRectData*)d;
    qreal pw = rd->pen.widthF();
    QRectF r( 0, 0, d->currentSize.width()+pw/2, d->currentSize.height()+pw/2 );
    return r;
    */
    /*
    NCReportRectData* rd = (NCReportRectData*)d;
    rd->boundingRect = NCReportItem::boundingRect();
    rd->rect = NCReportItem::boundingRect();
    qreal pw = rd->pen.widthF();
    if (pw != 0.0)
        rd->boundingRect = shape().controlPointRect();
    return rd->boundingRect;
    */
}

void NCReportRectItem::setRect(const QRectF & rect)
{
    NCReportRectData* rd = (NCReportRectData*)d;
    if (rd->rect == rect)
        return;
    prepareGeometryChange();
    rd->rect = rect;
    rd->boundingRect = QRectF();
    update();
}
