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
#ifndef NCREPORTGRAPHITEM_H
#define NCREPORTGRAPHITEM_H

#include "ncreportitem.h"

class NCReportDef;
class NCReportEvaluator;
class NCReportDataSource;
class NCReportGraphRenderer;

/*!
Graph or custom item's data class
 */

/*!

section 功能

图形／自定义控件数据类
【公有继承自报表数据基类NCReportItemData】
＃＃＃暂时屏蔽图形／自定义控件的相关功能＃＃＃

*/
class NCReportGraphData : public NCReportItemData
{
public:
    NCReportGraphData()
    {}

    QString graphDefinition;
    QString classID;
    //NCReportDataSource* dataSource;
};

/*!
Graph or custom item class
 */

/*!

section 功能

图形／自定义控件管理类
【公有继承自报表数据基类NCReportItem】
＃＃＃暂时屏蔽图形／自定义控件的相关功能＃＃＃

*/
class NCREPORTSHARED_EXPORT NCReportGraphItem : public NCReportItem
{
public:
    NCReportGraphItem( NCReportDef* rdef, QGraphicsItem* parent =0);
    ~NCReportGraphItem();
    //QRectF boundingRect() const;

    void adjustSize();
    void adjustSize( NCReportOutput* output);
    bool read( NCReportXMLReader* reader );
    bool write( NCReportXMLWriter* writer );
    void setDefaultForEditor();
    void paint( NCReportOutput*, const QPointF& );
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintEditor(QPainter *painter, const QRectF &rect);

    inline QString graphDefinition() const
    { return ((NCReportGraphData*)d)->graphDefinition; }

    inline void setGraphDefinition( const QString& gd )
    { ((NCReportGraphData*)d)->graphDefinition = gd; }

    inline QString classID() const
    { return ((NCReportGraphData*)d)->classID; }

    inline void setClassID( const QString& id )
    { ((NCReportGraphData*)d)->classID = id; }

//    inline void setDataSource(NCReportDataSource *ds)
//    { ((NCReportGraphData*)d)->dataSource = ds; }

//    NCReportDataSource *dataSource() const
//    { return ((NCReportGraphData*)d)->dataSource; }

    void updateValue(NCReportOutput *output, NCReportEvaluator *evaluator);

    bool isDynamic() const;

    /*!
     * \brief isParameterMode Returns the parameter mode. If true the item works by the new parameter approach, if false it works in the old way (compatible mode)
     * \return true if the new mode is active.
     */
    bool isParameterMode() const;

    NCReportGraphRenderer *renderer();
};


#endif
