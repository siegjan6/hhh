/*!
\author zyb 2015 .01.08
\section 设备或变量列表控件中树节点基类
*/
#ifndef IMDTREEMODEL
#define IMDTREEMODEL
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QVariant>
#include "IMDTreeItem.h"

class IMDTreeModel: public QAbstractItemModel
{
public:
    virtual ~IMDTreeModel(){}
public:
    virtual void setTreeItemIcon(QVariant icon,MDTreeItemType itemType) = 0;
    virtual QList<QModelIndex> getCheckedItems()= 0;
    virtual void setCheckedable(bool enableChecked)= 0;
    virtual void deleteTreeItem(const QModelIndex &itemIndex)= 0;

    virtual void changeLanguage() = 0;
    //!设置当前该刷新的节点
    virtual void setCurRootTreeItem(const QString& rootTreeItemName) = 0;
    //!重新bindModel后展开某节点
    virtual void setExpandItem(const QModelIndex &itemIndex) = 0;
};
#endif // IMDTREEMODEL

