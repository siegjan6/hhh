/*!

\author dGFuZ3p3

\section 功能

对象树的模型类。

*/

#ifndef MDOBJECTTREEMODEL_H
#define MDOBJECTTREEMODEL_H

#include <QModelIndex>
#include <QVariant>
#include "MDObjectTreeItem.h"

class CMDObjectTreeWindow;
class CMDObjectTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    CMDObjectTreeModel();
    ~CMDObjectTreeModel();
/*!
\section 初始化
*/
public:
    void initial(CMDObjectTreeWindow *treeWindow, const QStringList& sourceList);
    void initHeader();
    void initCheckItems(const QStringList& sourceList);
private:
    CMDObjectTreeWindow *m_treeWindow;
    QStringList m_sourceList;
private:
    QStringList m_headerList;

/*!
\section 构造后台树结构
*/
public:
    void resetModelData();
private:
    void rebuildObjectTree();
    CMDObjectTreeItem * indexToItem(const QModelIndex &index) const;
    CMDObjectTreeItem m_rootTreeItem;

/*!
\section 重载虚函数，填充模型信息
*/
public:
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) Q_DECL_OVERRIDE;
    /*
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    */

/*!
\section 管理选中的项
*/
public:
    QList<QModelIndex> getCheckedItems();
    void setCheckedable(bool enableChecked);
    void deleteTreeItem(const QModelIndex &itemIndex);
private:
   bool m_enableChecked;
   QList<QModelIndex> m_checkedList;
};

#endif // MDOBJECTTREEMODEL_H
