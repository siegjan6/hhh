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
#ifndef NCREPORTRECTITEM_H
#define NCREPORTRECTITEM_H

#include "ncreportshapeitem.h"

/*!
Rectangle item's data class
 */

/*!

section 功能

矩形控件数据类
【派生自报表形状控件类NCReportShapeData，而NCReportShapeItem类又派生自报表控件基类NCReportItemData】
＃＃＃项目中暂时屏蔽了椭圆控件相关功能＃＃＃

*/
class NCReportRectData : public NCReportShapeData
{
public:
    NCReportRectData() : xRound(0), yRound(0), rect(QRectF())
    {}

    int xRound, yRound;
    QRectF rect;
};

/*!
Rectangle item class
 */

/*!

section 功能

矩形控件属性类
【派生自报表形状控件类NCReportShapeItem，而NCReportShapeItem类又派生自报表控件基类NCReportItem】
＃＃＃项目中暂时屏蔽了矩形控件相关功能＃＃＃

*/
class NCReportRectItem : public NCReportShapeItem
{
public:
    NCReportRectItem( NCReportDef* rdef, QGraphicsItem* parent =0);
    ~NCReportRectItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    inline int xRound() const
    { return ((NCReportRectData*)d)->xRound; }

    inline void setXRound( int r )
    { ((NCReportRectData*)d)->xRound = r; }

    inline int yRound() const
    { return ((NCReportRectData*)d)->yRound; }

    inline void setYRound( int r )
    { ((NCReportRectData*)d)->yRound = r; }

    void setRect(const QRectF &rect);
    bool read( NCReportXMLReader* );
    bool write( NCReportXMLWriter* );
    void setDefaultForEditor();
    void paint( NCReportOutput*, const QPointF& );
};

#endif
