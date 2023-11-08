#include <QLineEdit>
#include <QRegExp>
#include "ItemDelegate.h"
////////////////////////////////////////////////////////////////////////////////
CReadOnlyDelegate::CReadOnlyDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}
QWidget* CReadOnlyDelegate::createEditor(QWidget*parent, const QStyleOptionViewItem &option,
    const  QModelIndex &index) const
{
    return  NULL;
}
////////////////////////////////////////////////////////////////////////////////////
CDoubleDelegate::CDoubleDelegate(int decimal, QObject *parent/* = 0*/)
    : QItemDelegate(parent)
{
    if (decimal < 0)
        decimal = 0;
    QString dec = QString::number(decimal);
    m_reg = "-?\\d+";
    if (decimal > 0)
        m_reg += "\\.?\\d{0," + dec + "}";
}
QWidget *CDoubleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
    const  QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    QRegExp regExp(m_reg);
    editor->setValidator(new QRegExpValidator(regExp, parent));
    return editor;
}
void CDoubleDelegate::setEditorData(QWidget *editor, const QModelIndex &index)  const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *lineEdit = static_cast <QLineEdit*>(editor);
    lineEdit->setText(text);
}
void CDoubleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
    const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString text = lineEdit->text();
    model->setData(index, text, Qt::EditRole);
}
void CDoubleDelegate::updateEditorGeometry(QWidget *editor,
    const  QStyleOptionViewItem &option,  const  QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

