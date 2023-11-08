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
    virtual ~CReadOnlyDelegate(){}
    QWidget *createEditor(QWidget*parent,  const QStyleOptionViewItem &option,
        const  QModelIndex &index)  const;
};

#endif // ITEMDELEGATE_H
