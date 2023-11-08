#include "MDObjectTreeModel.h"
#include <QIcon>
#include "MDObjectTreeWindow.h"
#include "MDMultiLanguage.h"

CMDObjectTreeModel::CMDObjectTreeModel()
{
    m_treeWindow = NULL;
    m_enableChecked = false;
}

CMDObjectTreeModel::~CMDObjectTreeModel()
{
    m_rootTreeItem.releaseChild();
}

/*!
\section 初始化
*/
void CMDObjectTreeModel::initial(CMDObjectTreeWindow *treeWindow, const QStringList& sourceList)
{
    m_treeWindow = treeWindow;
    m_sourceList = sourceList;

    initHeader();
    rebuildObjectTree();
    initCheckItems(sourceList);
}

// actionData 要根据实际的列取数据
void CMDObjectTreeModel::initHeader()
{
    if(m_treeWindow)
        m_treeWindow->initHeader(m_enableChecked, m_headerList);
}

void CMDObjectTreeModel::initCheckItems(const QStringList& sourceList)
{
    if(!m_treeWindow)
        return;

    int childCount = rowCount();
    for (int i=0;i<childCount;i++)
    {
       QModelIndex childIndex = index(i, 0);
       CMDObjectTreeItem *item = static_cast<CMDObjectTreeItem*>(childIndex.internalPointer());
       if(sourceList.contains(m_treeWindow->objectName(item ? item->ptr() : NULL), Qt::CaseInsensitive))
          setData(childIndex, Qt::Checked, Qt::CheckStateRole);
    }
}

/*!
\section 构造后台树结构
*/
void CMDObjectTreeModel::resetModelData()
{
    beginResetModel();

    rebuildObjectTree();

    endResetModel();
}

void CMDObjectTreeModel::rebuildObjectTree()
{
    if(!m_treeWindow)
        return;

    m_checkedList.clear();

    m_rootTreeItem.setType(-1);
    //m_rootTreeItem.setPtr(m_actionConfiger);
    m_rootTreeItem.releaseChild();

    // 外部构造对象树
    // -----------------------------------------------------
    m_treeWindow->buildObjectTree(&m_rootTreeItem);
}

CMDObjectTreeItem *CMDObjectTreeModel::indexToItem(const QModelIndex &index) const
{
    CMDObjectTreeItem *item = NULL;
    if (index.isValid())
        item = static_cast<CMDObjectTreeItem*>(index.internalPointer());

    return (item ? item : (CMDObjectTreeItem*)&m_rootTreeItem);
}

/*!
\section 重载虚函数，填充模型信息
*/
int CMDObjectTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return m_headerList.count();
}

// 确定父项 parent 包含多少子项，界面则呈现为“行数”
int CMDObjectTreeModel::rowCount(const QModelIndex &parent) const
{
    CMDObjectTreeItem *parentItem = indexToItem(parent);
    return (parentItem ? parentItem->childCount() : 0);
}

QVariant CMDObjectTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(STRANS(m_headerList[section]));

    return QVariant();
}

QVariant CMDObjectTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(m_enableChecked)
    {
        if (role==Qt::CheckStateRole && index.column()==0)
        {
            if (m_checkedList.contains(index))
                return Qt::Checked;
            else
            {
                return Qt::Unchecked;
            }
        }
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole && role!=Qt::DecorationRole)
         return QVariant();

    CMDObjectTreeItem *item = static_cast<CMDObjectTreeItem*>(index.internalPointer());
    if (!item)
        return QVariant();

    return (m_treeWindow ?
                m_treeWindow->objectData(m_enableChecked, item, index.column(), role) :
                QVariant());
}

// 根据父项索引 parent 和 子项的位置【row, column】，创建子项的索引
QModelIndex CMDObjectTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    CMDObjectTreeItem *parentItem = indexToItem(parent);
    if(!parentItem)
        return QModelIndex();

    CMDObjectTreeItem *childItem = parentItem->child(row);
    if(childItem)
    {
        QModelIndex childIndex = createIndex(row, column, childItem);
        return childIndex;
    }
    else return QModelIndex();
}

// 返回 index 的父索引。
// 【关键点：顶层索引的父必须是 invalid】
// 动作视图是 1 层结构，“动作”节点索引都是顶层索引，故此处必须返回 QModelIndex。
// （2 层以上结构的代码，参考 NSEventUI 中的 CMDEventTreeModel::parent）
QModelIndex CMDObjectTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CMDObjectTreeItem *childItem = indexToItem(index);
    if(!childItem)
        return QModelIndex();

    if(!m_treeWindow)
        return QModelIndex();

    CMDObjectTreeItem *parentItem = NULL;
    if(!m_treeWindow->isTopObject(childItem->type()))
    {
        parentItem = childItem->parent();
    }
    return (parentItem ? createIndex(parentItem->childCount(), 0, parentItem) : QModelIndex());
}

Qt::ItemFlags CMDObjectTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if(m_enableChecked)
    {
        if(index.column()==0)
        {
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
        }
        else
        {
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        }
    }
    else
    {
        return QAbstractItemModel::flags(index);
        //return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }
}

bool CMDObjectTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role==Qt::CheckStateRole && index.column()==0)
    {
        if (value==Qt::Unchecked)
        {
            m_checkedList.removeOne(index);
            emit(dataChanged(index, index));
        }
        else if(value==Qt::Checked)
        {
            m_checkedList.append(index);
            emit(dataChanged(index, index));
        }
        int childCount = rowCount(index);
        if (childCount>0)
        {
            for (int i=0;i<childCount;i++)
            {
               QModelIndex child = this->index(i, 0, index);
               setData(child, value, Qt::CheckStateRole);
            }
        }
        return true;
    }
    else
    {
         if (role != Qt::EditRole)
            return false;
        CMDObjectTreeItem *item = indexToItem(index);
        bool result =(m_treeWindow ? m_treeWindow->resultOnSetData(item->type()) : true);
        if (result)
            emit dataChanged(index, index);

        return result;
    }
}

/*!
\section 管理选中的项
*/
QList<QModelIndex>  CMDObjectTreeModel::getCheckedItems()
{
    return m_checkedList;
}

void CMDObjectTreeModel::setCheckedable(bool enableChecked)
{
    m_enableChecked = enableChecked;
}

void CMDObjectTreeModel::deleteTreeItem(const QModelIndex &itemIndex)
{
    beginRemoveRows(itemIndex.parent(), itemIndex.row(),itemIndex.row());

//    IMDTreeItem *item = indexToItem(itemIndex);
//    if(item->type() == MDTreeItemType::VariableGroup)
//    {
//        IMDVariableGroup* varGroup =(IMDVariableGroup *)item;
//        IMDVariableGroup* parentVarGroup = (IMDVariableGroup*)varGroup->parent(MDTreeItemType::VariableGroup);
//        parentVarGroup->removeChildVariableGroup(varGroup);
//        delete varGroup;
//    }
//    else if(item->type() == MDTreeItemType::Variable)
//    {
//        IMDRTDBVariable* rtdbVariable =(IMDRTDBVariable *)item;
//        IMDVariableGroup* parentVarGroup = (IMDVariableGroup*)(rtdbVariable->parent(MDTreeItemType::VariableGroup));
//        parentVarGroup->removeVariable(rtdbVariable);
//        delete rtdbVariable;
//    }

    endRemoveRows();
}


/*
bool CMDObjectTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    CMDVariableGroup *parentItem = indexToItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, columnCount());
    endInsertRows();

    return success;
}*/
/*
bool CMDObjectTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    CMDVariableGroup *parentItem = indexToItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->r(position, rows);
    endRemoveRows();

    return success;
}
*/
