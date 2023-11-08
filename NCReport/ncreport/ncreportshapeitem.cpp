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
#include "ncreportshapeitem.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportoutput.h"

NCReportShapeItem::NCReportShapeItem(QGraphicsItem * parent) : NCReportItem( parent )
{
}

NCReportShapeItem::~ NCReportShapeItem()
{
}

QPainterPath NCReportShapeItem::shapeFromPath(const QPainterPath & path, const QPen & pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath())
        return path;
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}

void NCReportShapeItem::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}

bool NCReportShapeItem::read(NCReportXMLReader *r )
{
	NCReportShapeData* sd = (NCReportShapeData*)d;

	NCReportItem::read(r);
	r->readPen(sd->pen);
	if (!isLine())
		r->readBrush( sd->brush );
	sd->mLineWidth = r->readLineWidth();

	if ( r->isAttrib("fitTo") ) {
		 if ( r->attrib("fitTo") == "section" )
			 setFitRole(FitHeightToSection);
		 else if ( r->attrib("fitTo") == "zone" )
			 setFitRole(FitHeightToZone);
	}

	return true;
}

bool NCReportShapeItem::write(NCReportXMLWriter *w)
{
	NCReportItem::write(w);
	w->writePen( pen() );
	w->writeLineWidth(lineWidth());
	if (!isLine())
		w->writeBrush( brush() );

	if ( isAutoHeight() ) {
		QString fit;
		switch ( fitRole() ) {
			case FitHeightToSection: fit="section"; break;
			case FitHeightToZone: fit="zone"; break;
			case NoFit: fit="none"; break;
		}
		w->writeAttribute("fitTo", fit );
	}

	return true;
}

void NCReportShapeItem::paint(NCReportOutput *, const QPointF &)
{
}



void NCReportShapeItem::setPen(const QPen &pen)
{
	NCReportShapeData *sd = (NCReportShapeData*)d;
    prepareGeometryChange();
    sd->pen = pen;
    sd->boundingRect = QRectF();
    update();
}


void NCReportShapeItem::setBrush(const QBrush &brush)
{
	NCReportShapeData *sd = (NCReportShapeData*)d;
    sd->brush = brush;
    update();
}

bool NCReportShapeItem::isObscuredBy(const QGraphicsItem *item) const
{
    return QGraphicsItem::isObscuredBy(item);
}

QPainterPath NCReportShapeItem::opaqueArea() const
{
	NCReportShapeData *sd = (NCReportShapeData*)d;
    if (sd->brush.isOpaque())
        return shape();
	return QGraphicsItem::opaqueArea();
}

bool NCReportShapeItem::isLine() const
{
	return false;
}
