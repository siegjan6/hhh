/*!
\author aGRw 2015.12.19
\section 输入委托
*/
#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QItemDelegate>

//!readOnly
class CReadOnlyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    CReadOnlyDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget*parent,  const QStyleOptionViewItem &option,
        const  QModelIndex &index)  const;
};
//!double
class CDoubleDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    CDoubleDelegate(int decimal, QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index)  const;
    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option,  const  QModelIndex &index) const;
private:
    QString m_reg;
};

#endif // ITEMDELEGATE_H
