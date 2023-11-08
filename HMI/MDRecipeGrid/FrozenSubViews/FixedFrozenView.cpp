#include "FixedFrozenView.h"

CFixedFrozenView::CFixedFrozenView(qint32 rowFrozen, qint32 columnFrozen, CRecipeGridView *parent)
    :CAbstractFrozenView(parent)
{
    m_frozenRow = m_model->rowCount() < rowFrozen ? m_model->rowCount() : rowFrozen;
    m_frozenColumn = m_model->columnCount() < columnFrozen ? m_model->columnCount() : columnFrozen;

    for (qint32 row = m_frozenRow; row < m_model->rowCount(); ++row)
        setRowHidden(row, true);
    for (qint32 col = m_frozenColumn; col < m_model->columnCount(); ++col)
        setColumnHidden(col, true);

    updateGeometry();
}

void CFixedFrozenView::drawFrame(QPainter &painter)
{
    drawBottomLine(painter);
    drawRightLine(painter);
}

void CFixedFrozenView::updateGeometry()
{
    qint32 i;
    qint32 x = m_parent->frameWidth();
    qint32 y = m_parent->frameWidth();
    qint32 width = 0;
    for( i = 0; i < m_frozenColumn; ++i)
       width +=  m_parent->columnWidth(i);
    width += m_parent->verticalHeader()->width();
    qint32 height = 0;
    for( i = 0; i < m_frozenRow; ++i)
       height +=  m_parent->rowHeight(i);
    height += m_parent->horizontalHeader()->height();

    setGeometry(x,y,width,height);
}

bool CFixedFrozenView::canSectionHidden(Qt::Orientation orientation, int index)
{
    if( Qt::Horizontal == orientation && m_frozenColumn < index + 1)
        return false;
    if( Qt::Vertical == orientation && m_frozenRow < index + 1)
        return false;

    return true;
}
