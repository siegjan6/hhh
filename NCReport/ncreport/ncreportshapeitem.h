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
#ifndef NCREPORTSHAPEITEM_H
#define NCREPORTSHAPEITEM_H

#include "ncreportitem.h"

#include <QPen>
#include <QBrush>
#include <QPointF>
#include <QRectF>

/*!
Shape item's data class. The class stores the properties of the item
*/

/*!

section 功能

形状控件数据类
【派生自报表控件基类NCReportItemData】

*/
class NCReportShapeData : public NCReportItemData
{
public:
	NCReportShapeData() : mLineWidth(0.2) {}

	qreal mLineWidth;
	QPen pen;
	QBrush brush;
	QRectF boundingRect;

};

/*!
Abstract Shape item class.
 */

/*!

section 功能

虚形状控件属性类
【派生自报表控件基类NCReportItem】

*/
class NCReportShapeItem : public NCReportItem
{
public:
 	NCReportShapeItem( QGraphicsItem* parent =0);
 	~NCReportShapeItem();
	//QRectF boundingRect() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	
	virtual bool read( NCReportXMLReader* r);
	virtual bool write( NCReportXMLWriter* w);
	virtual void paint( NCReportOutput*, const QPointF& ) =0;
	virtual bool isObscuredBy(const QGraphicsItem *item) const;
	virtual QPainterPath opaqueArea() const;
	virtual bool isLine() const;
	static QPainterPath shapeFromPath(const QPainterPath &path, const QPen &pen);

	
	inline QPen pen() const
	{ return ((NCReportShapeData*)d)->pen; }

	inline QBrush brush() const
	{ return ((NCReportShapeData*)d)->brush; }

	virtual void setBrush( const QBrush & brush );
	virtual void setPen( const QPen & pen );

	inline qreal lineWidth() const
	{ return ((NCReportShapeData*)d)->mLineWidth; }

	inline void setLineWidth( qreal width )
	{ ((NCReportShapeData*)d)->mLineWidth = width; }

};



#endif
