#include "ValueMapDelegate.h"
#include <QLineEdit>
#include <QPainter>
#include "ShareTool.h"

CValueMapDelegate::CValueMapDelegate(QTableView *parent, QString placeholderText, QString validatorText) :
    QStyledItemDelegate(parent)
{
    m_parent = parent;
    m_validatorText = validatorText;
    m_placeholderText = placeholderText;
}

void CValueMapDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    if( index.column() == itemtype::image )
        return;

    QString text = index.data().toString();
    if( !text.isEmpty() )
        return;

    bool isValid = isRowValid(index);
    //! 绘制提示
    painter->save();

    QPen pen = painter->pen();
    pen.setColor( isValid ? Qt::lightGray : Qt::red);
    painter->setPen(pen);

    QFont font = painter->font();
    font.setItalic(true);
    painter->setFont(font);

    QString showText = isValid ? m_placeholderText : m_validatorText;
    painter->drawText(option.rect, showText, QTextOption(Qt::AlignCenter));

    painter->restore();

}

bool CValueMapDelegate::isRowValid(const QModelIndex &index) const
{
    const QAbstractItemModel* theModel = index.model();
    qint32 row = index.row();

    QString valuestr = theModel->index(row, itemtype::value).data().toString();
    QString descstr = theModel->index(row, itemtype::desc).data().toString();

    if( valuestr.isEmpty() && descstr.isEmpty())
        return true;

    if( !valuestr.isEmpty() && !descstr.isEmpty())
        return true;

    return false;
}


