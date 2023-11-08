/****************************************************************************
*
*  Copyright (C) 2002-2012 Helta Kft. / NociSoft Software Solutions
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
#include "ncreportgraphrenderer.h"
#include "ncreportgraphitem.h"
#include <QPainter>

NCReportGraphRenderer::NCReportGraphRenderer() : m_dataSource(0), m_item(0)
{
}

NCReportGraphRenderer::~NCReportGraphRenderer()
{
}

void NCReportGraphRenderer::setID(const QString & id)
{
    classid = id;
}

void NCReportGraphRenderer::setId(const QString &id)
{
    classid = id;
}

void NCReportGraphRenderer::setId(int id)
{
    classid = QString::number(id);
}

const QString & NCReportGraphRenderer::id() const
{
    return classid;
}

int NCReportGraphRenderer::intId() const
{
    return classid.toInt();
}

void NCReportGraphRenderer::paintEditorItem(QPainter *painter, const QRectF &rect, const QString& text)
{
    Q_UNUSED(text)

    if (item()) {
        item()->paintEditor(painter,rect);
    }
}

qreal NCReportGraphRenderer::calculateHeightMM( NCReportOutput* ) const
{
    return 0.0;
}

QSizeF NCReportGraphRenderer::calculateSizeMM( NCReportOutput*, const QSizeF & ) const
{
    return QSizeF();
}

QSizeF NCReportGraphRenderer::calculateSizeMM( NCReportOutput*, const QSizeF &, const QString & ) const
{
    return QSizeF();
}

void NCReportGraphRenderer::setDataSource(NCReportDataSource *ds)
{
    m_dataSource = ds;
}

void NCReportGraphRenderer::setItem(NCReportGraphItem *item)
{
    m_item = item;
}

NCReportGraphItem *NCReportGraphRenderer::item() const
{
    return m_item;
}

QString NCReportGraphRenderer::dataSourceID() const
{
    return QString("n/a");
}

NCReportDataSource *NCReportGraphRenderer::dataSource() const
{
    return m_dataSource;
}

