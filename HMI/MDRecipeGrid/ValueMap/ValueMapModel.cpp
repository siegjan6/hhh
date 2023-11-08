/*!
author Y2hvdWps 2016.10.24
section 数值映射表数据实体类
*/
#include "ValueMapModel.h"
#include "ValueMapDelegate.h"
#include "ShareTool.h"
#include "ValueMapView.h"
#include <QLinkedList>

CValueMapModel::CValueMapModel(int rows, int columns, CValueMapView *parent)
    :QStandardItemModel(rows,columns,parent)
{
    m_mainView = parent;
}

bool CValueMapModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( index.column() == CValueMapDelegate::itemtype::value && role == Qt::EditRole)
    {
        QString str = value.toString();
        QString oldStr = data(index).toString();
        if(!str.isEmpty() && str.compare(oldStr))
        {
            if( m_nameSet.contains(str) )
            {
                CShareTool::trWarningMessageBox(tr("已存在同名的值,请重新输入!"), m_mainView);
                return false;
            }
            else
                m_nameSet.insert(str);
        }
    }

    return QStandardItemModel::setData(index,value,role);
}

QVariant CValueMapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //! 使用默认行为
    if( Qt::Vertical == orientation )
        return QStandardItemModel::headerData(section, orientation, role);
    //! 表格的表头暂时只能显示,其他的按默认处理
    if (role != Qt::DisplayRole)
        return QStandardItemModel::headerData(section, orientation, role);

    //! 设置横表头为字母
    switch(section)
    {
        case 0: return tr("值");
        case 1: return tr("描述");
        case 2: return tr("插入图片");
        default: return QVariant();
    }
}

bool CValueMapModel::removeRows(int row, int count, const QModelIndex &parent)
{
    for(qint32 idx = row; idx < row+count; idx++)
    {
        QStandardItem* theitem = item(row,CValueMapDelegate::itemtype::value);
        if( theitem )
            m_nameSet.remove(theitem->text());
    }

    return QStandardItemModel::removeRows(row,count,parent);
}

bool CValueMapModel::dropMimeData(const QMimeData *, Qt::DropAction, int, int, const QModelIndex &parent)
{
    if( !parent.isValid() )
        return false;

    QList<qint32> rows;
    m_mainView->selectedRows(rows);
    QList<QList<QStandardItem*>> copyRows;
    foreach(qint32 idx, rows)
    {
        QList<QStandardItem*> items;
        items.clear();
        for( qint32 col = 0; col < columnCount(); col++ )
        {
            QStandardItem* newitem = item(idx, col);
            if(newitem)
                newitem = newitem->clone();
            items.append(newitem);
        }
        copyRows.append(items);
    }

    qint32 destRow = parent.row();
    for( qint32 ilist = 0; ilist < copyRows.count(); ++ilist )
    {
        insertRow(destRow, copyRows[ilist]);
        m_mainView->setImageButtonItem(destRow);
    }

    return true;
}

