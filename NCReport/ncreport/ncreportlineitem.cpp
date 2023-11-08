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
#include "ncreportlineitem.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportoutput.h"
#include "ncreportsection.h"
#include "ncreportdef.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

NCReportLineData::NCReportLineData()
{
    mPos2 = QPointF();
    line = QLineF();
}

//-----------------------------------------------------------------------------------------------------
NCReportLineItem::NCReportLineItem( NCReportDef* rdef, QGraphicsItem * parent ) : NCReportShapeItem( parent )
{
    d = new NCReportLineData;
    d->reportDef = rdef;
    d->tagname = "line";
    d->objectType = Line;
    movePoint = QPointF(); // for handling user moves
}

NCReportLineItem::~ NCReportLineItem()
{
}

void NCReportLineItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    NCReportScale sc;
    Q_UNUSED(option);
    Q_UNUSED(widget);
    NCReportLineData* ld = (NCReportLineData*)d;

    QPen pen( ld->pen );
    qreal m_penWidth = sc.mmToPixel(ld->mLineWidth);
    if (m_penWidth<1)
        pen.setWidth(1);
    else
        pen.setWidthF( m_penWidth );

    painter->setPen( pen );
    painter->drawLine(ld->line);
    paintSelection( painter );
}

bool NCReportLineItem::read(NCReportXMLReader *r )
{
    NCReportShapeItem::read( r );
    Q_ASSERT(r->isStartElement() && r->name() == d->tagname );
    NCReportLineData* ld = (NCReportLineData*)d;
    NCReportScale s;

    NCReportShapeItem::read(r);
    ld->mPos = r->readPosFrom();
    ld->mPos2 = r->readPosTo();
    r->readElementText();

    return true;
}

bool NCReportLineItem::write(NCReportXMLWriter *w)
{
    NCReportLineData* ld = (NCReportLineData*)d;

    w->writeStartElement( d->tagname );
    NCReportShapeItem::write(w);
    w->writePosFrom( ld->mPos );
    w->writePosTo( ld->mPos2 );
    w->writeEndElement();

    return true;
}

void NCReportLineItem::paint( NCReportOutput* o, const QPointF& mPos )
{
    NCReportLineData* ld = (NCReportLineData*)d;
    QPointF p1( o->scale().mmToPoint(mPos + ld->mPos) );
    QPointF p2( o->scale().mmToPoint(mPos + ld->mPos2) );

    if ( isAutoHeight() ) {
        //int section_bottom = o->scale().mmToPixel(sectionMetricSize().height()+mPos.y());
        //int zone_bottom = o->scale().mmToPixel(zoneMetricSize().height()+mPos.y()+metricOffset().y());

        int section_height = o->scale().mmToPixel(sectionMetricSize().height() - metricPos().y());
        int zone_height = o->scale().mmToPixel(zoneMetricSize().height() );

        QPointF *pFrom, *pTo =0;
        if (p1.y()>p2.y()) {
            pFrom = &p2;
            pTo = &p1;
        } else {
            pFrom = &p1;
            pTo = &p2;
        }

        if (boundingRectOfLastPart().isValid())	{// rect around pabe break text
            pFrom->setY(boundingRectOfLastPart().top());
            pTo->setY(boundingRectOfLastPart().bottom());
        } else {
            pFrom->setY(pFrom->y());
            pTo->setY(ld->fitRole == FitHeightToZone ? pFrom->y()+zone_height : pFrom->y()+section_height);
        }
    }

    if (pinToRight()) {
        //QPointF *pFrom =0;
        QPointF *pTo =0;
        if (p1.x()>p2.x()) {
            //pFrom = &p2;
            pTo = &p1;
        } else {
            //pFrom = &p1;
            pTo = &p2;
        }
        pTo->setX(o->scale().mmToRealPixel(reportDef()->option().pageWidthMM() - reportDef()->option().rightMarginMM()));
    }

    switch ( o->output() ) {
        case NCReportOutput::Printer:
        case NCReportOutput::Pdf:
        case NCReportOutput::Svg:
        case NCReportOutput::Image:
        case NCReportOutput::Preview:
        case NCReportOutput::QtPreview:
        {
            QPen pen( ld->pen );
            qreal m_penWidth = o->scale().mmToPixel(ld->mLineWidth);
            if (m_penWidth<1.0)
#if QT_VERSION>=0x050000
                pen.setWidthF(0.0);
#else
                pen.setWidthF(1.0);
#endif
            else
                pen.setWidthF( m_penWidth );
            o->painter()->setPen( pen );
            o->painter()->drawLine( p1, p2 );
            break;
        }
        default:
            break;
    }
    o->renderItem( this, QRectF(p1,p2) );
}

QRectF NCReportLineItem::boundingRect() const
{
    return shape().boundingRect();
    /*
    NCReportLineData* ld = (NCReportLineData*)d;
    if (ld->pen.widthF() == 0.0) {
        const qreal x1 = ld->line.p1().x();
        const qreal x2 = ld->line.p2().x();
        const qreal y1 = ld->line.p1().y();
        const qreal y2 = ld->line.p2().y();
        qreal lx = qMin(x1, x2);
        qreal rx = qMax(x1, x2);
        qreal ty = qMin(y1, y2);
        qreal by = qMax(y1, y2);
        return QRectF(lx, ty, rx - lx, by - ty);
    }
    return shape().controlPointRect();
    */
}

QPainterPath NCReportLineItem::shape() const
{
    QPainterPath path;
    NCReportLineData* ld = (NCReportLineData*)d;
    if (ld->line.isNull())
        return path;

    path.moveTo(ld->line.p1());
    path.lineTo(ld->line.p2());

    path.addRect( selectionRect(ld->line.p1()) );
    path.addRect( selectionRect(ld->line.p2()) );
    return path;
}

QLineF NCReportLineItem::line() const
{
    NCReportLineData* ld = (NCReportLineData*)d;
    return ld->line;
}

void NCReportLineItem::setLine(const QLineF & line)
{
    NCReportLineData* ld = (NCReportLineData*)d;
    if (ld->line == line)
        return;
    prepareGeometryChange();
    ld->line = line;
    update();
}


void NCReportLineItem::paintSelection(QPainter * painter)
{
    if ( !isSelected() )
        return;

    NCReportLineData* ld = (NCReportLineData*)d;

    preparePaintSelection(painter);

    painter->drawRect( selectionRect(ld->line.p1()) );
    painter->drawRect( selectionRect(ld->line.p2()) );

}

QRectF NCReportLineItem::selectionRect( const QPointF& point ) const
{
    return QRectF(QPointF(point.x()-SELECTION_BOXSIZE/2,point.y()-SELECTION_BOXSIZE/2), QSizeF(SELECTION_BOXSIZE,SELECTION_BOXSIZE));
}

QRectF NCReportLineItem::selectionRect(const SelectionDirection direction, const QSizeF& ) const
{
    QRectF r;
    NCReportLineData* ld = (NCReportLineData*)d;
    switch ( direction ) {
        case LinePoint1: r = selectionRect(ld->line.p1()); break;
        case LinePoint2: r = selectionRect(ld->line.p2()); break;
        default:
            break;
    }
    return r;
}


void NCReportLineItem::setDefaultForEditor()
{
    //setLine( QLineF(0.0, 0.0, 100.0, 10.0) );
}

void NCReportLineItem::setPen(const QPen & pen)
{
    NCReportLineData* ld = (NCReportLineData*)d;
    prepareGeometryChange();
    ld->pen = pen;
    update();
}

void NCReportLineItem::setBrush(const QBrush &)
{
}

void NCReportLineItem::updateForSave()
{
    NCReportLineData* ld = (NCReportLineData*)d;
    NCReportScale s;

    QLineF line( ld->line );
    if ( !movePoint.isNull() ) {
        line.translate( movePoint );
    }
    ld->mPos = s.pointToMM( line.p1() );
    ld->mPos2 = s.pointToMM( line.p2() );
    //ld->mLineWidth = s.mmToRealPixel( ld->pen.widthF() );
}

void NCReportLineItem::updateForEditor()
{
    NCReportLineData* ld = (NCReportLineData*)d;
    NCReportScale s;
    QLineF line( s.mmToPoint(ld->mPos), s.mmToPoint(ld->mPos2) );
    setLine( line );
}

void NCReportLineItem::itemPositionChange(const QPointF & pos )
{
    movePoint = pos;
    /*
    NCReportLineData* ld = (NCReportLineData*)d;
    ld->line.translate( pos - ld->line.p1() );
    */
}

void NCReportLineItem::updateSize()
{
}

void NCReportLineItem::updatePosition()
{
    updateForEditor();
}

void NCReportLineItem::enableOffsetPosition()
{
    NCReportLineData* ld = (NCReportLineData*)d;
    ld->mPos -= metricOffset();
    ld->mPos2 -= metricOffset();
}

void NCReportLineItem::disableOffsetPosition()
{
    NCReportLineData* ld = (NCReportLineData*)d;
    ld->mPos += metricOffset();
    ld->mPos2 += metricOffset();
}

bool NCReportLineItem::isLine() const
{
    return true;
}

bool NCReportLineItem::hasSizeAndPosition() const
{
    return false;
}

void NCReportLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if ( !isSelected() || m_selectionHoverState == None || isLocked() || event->modifiers() & Qt::ControlModifier )  {
        QGraphicsItem::mouseMoveEvent(event);
        return;
    }

    // RESIZE ITEM
    NCReportSection *sect = sectionScene();
    NCReportLineData* ld = (NCReportLineData*)d;
    QLineF newLine;
    bool changed = true;

    switch ( m_selectionHoverState ) {
    case LinePoint1: {
            newLine = QLineF(sect->snapToPoint( event->pos() ), ld->line.p2());
            break;
        }
        case LinePoint2:
            newLine = QLineF(ld->line.p1(), sect->snapToPoint(event->pos()) );
            break;
        case None:
        default:
            changed = false;
            break;
    }

    if (changed) {
        setLine( newLine );
        sect->emitContentChanged();
        sect->emitItemChanged( this );
    }
}

void NCReportLineItem::saveSelectState()
{
    NCReportItem::saveSelectState();

    NCReportLineData* ld = (NCReportLineData*)d;
    ld->selectLine = ld->line;
}

QLineF NCReportLineItem::selectLine() const
{
    return ((NCReportLineData*)d)->selectLine;
}
