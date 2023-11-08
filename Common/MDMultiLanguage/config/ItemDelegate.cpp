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

