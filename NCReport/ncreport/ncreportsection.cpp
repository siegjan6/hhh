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
#include "ncreportsection.h"
#include "ncreportgroup.h"
#include "ncreportitem.h"
#include "ncreportdef.h"
#include "ncreportdatasourcerelation.h"

#include <QGraphicsSceneContextMenuEvent>

NCReportSection::NCReportSection(QObject * parent) : QGraphicsScene( parent ),
    m_gridVisible(false),
    m_snapX(false),
    m_snapY(false),
    m_deltaX(6),
    m_deltaY(6),
    m_zvalue(0),

    m_type(Custom),
    m_checkRole(SectionOnly),
    m_heightMM(10),
    m_orphanedTolaranceMM(0),

    m_show(true),
    m_finish(false),
    m_autoHeight(false),
    m_hideIfBlank(false),
    m_printLock(false),
    m_id(QString()),
    m_tagname(QString()),
    m_datasourceID(QString()),
    m_datasource2ID(QString()),
    m_layoutIndex(-1),
    m_pageBreakExp(QString()),
    m_startsOnNewPage(false),
    m_printWhenExp(QString()),
    m_group(0),
    m_anchorToFooter(false),
    m_notAlone(false),
    m_dataSource(0),
    m_altBackMode(Disabled),
    m_altBackColor(QColor(220,220,220)),
    m_useDefaultAltBackColor(true)
{

    //leftMargin=0, rightMargin=0, topMargin=0, bottomMargin=0;
}

NCReportSection::~NCReportSection()
{
    qDeleteAll(m_groups.begin(), m_groups.end());
    m_groups.clear();
    qDeleteAll(m_itemList.begin(), m_itemList.end());
    m_itemList.clear();
}

QString NCReportSection::caption() const
{
    switch( m_type ) {
        case PageHeader: return tr("Page header");
        case PageFooter: return tr("Page footer");
        case GroupHeader:
        case GroupFooter:
            {
                if (m_group) {
                    QString ttl = (m_type == GroupHeader ? tr("Group header [%1.%2]") : tr("Group footer [%1.%2]") );
                    return ttl.arg( m_group->detail()->id() ).arg( m_group->groupId() );
                } else {
                    QString ttl = (m_type == GroupHeader ? tr("Group header") : tr("Group footer") );
                    return ttl;
                }
            }
        case Detail: return tr("Detail [%1]").arg(id());
        case ReportHeader: return tr("Report header");
        case ReportFooter: return tr("Report footer");
        case Custom: return tr("Custom section");
    }

    return QString::null;
}

void NCReportSection::setUseDefaultAlternateRowColor(bool set)
{
    m_useDefaultAltBackColor = set;
}

bool NCReportSection::useDefaultAlternateRowColor() const
{
    return m_useDefaultAltBackColor;
}

// void NCReportSection::setVirtualMargin(qreal left, qreal right, qreal top, qreal bottom)
// {
// 	leftMargin = left;
// 	rightMargin = right;
// 	topMargin = top;
// 	bottomMargin = bottom;
// }

// QPointF NCReportSection::mapToMargin(const QPointF & point) const
// {
// 	return QPointF( point.x()+leftMargin, point.y()+topMargin );
// }

int NCReportSection::snapValue(int value, int grid) const
{
    const int rest = value % grid;
    const int absRest = (rest < 0) ? -rest : rest;
    int offset = 0;
    if (2 * absRest > grid)
        offset = 1;
    if (rest < 0)
        offset *= -1;
    return (value / grid + offset) * grid;
}

QPointF NCReportSection::snapToPoint(const QPointF &p) const
{
    const int sx = m_snapX ? snapValue( (int)p.x(), m_deltaX) : (int)p.x();
    const int sy = m_snapY ? snapValue( (int)p.y(), m_deltaY) : (int)p.y();
    return QPointF(sx, sy);
}

qreal NCReportSection::snapToX(const qreal value ) const
{
    const int sx = m_snapX ? snapValue((int)value, m_deltaX) : (int)value;
    return (qreal)sx;
}

qreal NCReportSection::snapToY(const qreal value ) const
{
    const int sy = m_snapY ? snapValue( (int)value, m_deltaY) : (int)value;
    return (qreal)sy;
}

void NCReportSection::emitContentChanged()
{
}

void NCReportSection::emitItemChanged(NCReportItem *)
{
}

int NCReportSection::nextZValue()
{
    return m_zvalue++;
}

void NCReportSection::resetZValue()
{
    m_zvalue = 0;
}

void NCReportSection::addItemZ(NCReportItem *)
{
    // nothing to do for report engine
}

void NCReportSection::addItemForEngine(NCReportItem *item)
{
    item->setSection(this);
    m_itemList.append(item);
}
