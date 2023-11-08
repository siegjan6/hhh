#include "RowFrozenView.h"

CRowFrozenView::CRowFrozenView(qint32 rowFrozen, CRecipeGridView *parent)
    :CAbstractFrozenView(parent)
{
    m_frozenRow = m_model->rowCount() < rowFrozen ? m_model->rowCount() : rowFrozen;

    for (qint32 row = m_frozenRow; row < m_model->rowCount(); ++row)
        setRowHidden(row, true);

    updateGeometry();

    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)),
            parent->horizontalScrollBar(), SLOT(setValue(int)));
    connect(parent->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            horizontalScrollBar(), SLOT(setValue(int)));
}

void CRowFrozenView::drawFrame(QPainter &painter)
{
    //! bottomLine
    drawBottomLine(painter);
    //! LeftLine
    if( m_parent->frozenColumnCount() > 0 )
    {
        qreal penWidth = painter.pen().width();
        qint32 width = 0;
        for (qint32 i = 0; i < m_parent->frozenColumnCount(); ++i)
            width += m_parent->columnWidth(i);

        QPointF point3(width, 0);
        QPointF point4(point3.x(), verticalHeader()->length() - penWidth/2.f );
        painter.drawLine(point3, point4);
    }
}

void CRowFrozenView::updateGeometry()
{
    qint32 x = m_parent->frameWidth();
    qint32 y = m_parent->frameWidth();
    qint32 width =m_parent->viewport()->width() + m_parent->verticalHeader()->width();
    qint32 height = 0;
    for( qint32 i = 0; i < m_frozenRow; ++i)
       height +=  m_parent->rowHeight(i);
    height += m_parent->horizontalHeader()->height();

    setGeometry(x,y,width,height);

}

bool CRowFrozenView::canSectionHidden(Qt::Orientation orientation, int index)
{
    if( Qt::Vertical == orientation && m_frozenRow < index + 1)
        return false;

    return true;
}
