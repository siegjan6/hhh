#include "ColumnFrozenView.h"

CColumnFrozenView::CColumnFrozenView(qint32 columnFrozen, CRecipeGridView *parent)
    :CAbstractFrozenView(parent)
{
    m_frozenColumn = m_model->columnCount() < columnFrozen ? m_model->columnCount() : columnFrozen;

    qint32 i;
    for (i = m_frozenColumn; i < m_model->columnCount(); ++i)
        setColumnHidden(i, true);

    updateGeometry();

    connect(verticalScrollBar(), SIGNAL(valueChanged(int)),
            parent->verticalScrollBar(), SLOT(setValue(int)));
    connect(parent->verticalScrollBar(), SIGNAL(valueChanged(int)),
            verticalScrollBar(), SLOT(setValue(int)));
}

void CColumnFrozenView::drawFrame(QPainter &painter)
{
    //! rightLine
    drawRightLine(painter);
    //! topLine
    if( m_parent->frozenRowCount() > 0 )
    {
        qint32 i;
        qreal penWidth = painter.pen().width();
        qint32 width = 0;
        for (i = 0; i < m_parent->frozenColumnCount(); ++i)
            width += m_parent->columnWidth(i);
        qint32 height = 0;
        for (i = 0; i < m_parent->frozenRowCount(); ++i)
            height += m_parent->rowHeight(i);
        QPointF point3(0, height);
        QPointF point4(width - penWidth/2.f, point3.y());
        painter.drawLine(point3, point4);
    }
}

void CColumnFrozenView::updateGeometry()
{
    qint32 x = m_parent->frameWidth();
    qint32 y = m_parent->frameWidth();
    qint32 width = 0;
    for( qint32 i = 0; i < m_frozenColumn; ++i)
       width +=  m_parent->columnWidth(i);
    width += m_parent->verticalHeader()->width();
    qint32 height = m_parent->viewport()->height() + m_parent->horizontalHeader()->height();

    setGeometry(x,y,width,height);
}

bool CColumnFrozenView::canSectionHidden(Qt::Orientation orientation, int index)
{
    if( Qt::Horizontal == orientation && m_frozenColumn < index + 1)
        return false;

    return true;
}
