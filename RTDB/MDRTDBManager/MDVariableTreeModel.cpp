#include "MDVariableTreeModel.h"
#include "MDVariableGroup.h"
#include "MDRTDBVariableUaBase.h"
#include "IMDTreeItem.h"
#include "MDMultiLanguage.h"
CMDVariableTreeModel::CMDVariableTreeModel()
{
    m_rootTreeItem = NULL;
    m_enableChecked = false;
    m_variableGroup = NULL;
}
CMDVariableTreeModel::~CMDVariableTreeModel()
{

}

void CMDVariableTreeModel::setTreeItemIcon(QVariant icon,MDTreeItemType itemType)
{
    switch (itemType) {
    case MDTreeItemType::VariableGroup:
        m_variableGroupIcon = icon;
        break;
    case MDTreeItemType::Variable:
        m_variableIcon = icon;
        break;
    default:
        break;
    }
}
QList<QModelIndex>  CMDVariableTreeModel::getCheckedItems()
{
    return m_checkedList;
}
void CMDVariableTreeModel::setCheckedable(bool enableChecked)
{
    m_enableChecked = enableChecked;
}
void CMDVariableTreeModel::setCurRootTreeItem(const QString& rootTreeItemName)
{
    if(rootTreeItemName.isEmpty())
        return;

    IMDTreeItem * pGroup = m_variableGroup->getChildVariableGroupWithName(rootTreeItemName);
    if(pGroup != NULL)
        m_rootTreeItem = pGroup;
}
void CMDVariableTreeModel::setExpandItem(const QModelIndex &itemIndex)
{
    setData(itemIndex, true, Qt::DecorationRole);
}

void CMDVariableTreeModel::initial(IMDVariableGroup *rootVariableGroup)
{
    if(m_enableChecked)
    {
        m_headerList.append("名称");
        m_headerList.append("数据类型");
        m_headerList.append("描述");
    }
    else
    {
        m_headerList.append("名称");
        m_headerList.append("数据类型");
        m_headerList.append("设备");
        m_headerList.append("地址");
        m_headerList.append("变量类型");
        m_headerList.append("描述");
		m_headerList.append("单位");
    }

    m_rootTreeItem = rootVariableGroup;
    m_variableGroup = rootVariableGroup;
}

//!多语言切换
void CMDVariableTreeModel::changeLanguage()
{
    //通过触发headerDataChanged()信号实行多语言标题栏文本切换
    m_headerList.clear();
    if(m_enableChecked)
    {
        m_headerList.append(CMDMultiLanguage::instance()->value(tr("名称")));
        m_headerList.append(CMDMultiLanguage::instance()->value(tr("数据类型")));
        m_headerList.append(CMDMultiLanguage::instance()->value(tr("描述")));
    }
    else
    {
        m_headerList.append(CMDMultiLanguage::instance()->value(tr("名称")));
        m_headerList.append(CMDMultiLanguage::instance()->value(tr("数据类型")));
        m_headerList.append(CMDMultiLanguage::instance()->value(tr("设备")));
        m_headerList.append(CMDMultiLanguage::instance()->value(tr("地址")));
        m_headerList.append(CMDMultiLanguage::instance()->value(tr("变量类型")));
        m_headerList.append(CMDMultiLanguage::instance()->value(tr("描述")));
		m_headerList.append(CMDMultiLanguage::instance()->value(tr("单位")));
    }

    emit headerDataChanged(Qt::Horizontal ,0,m_headerList.count()-1);
}
int CMDVariableTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return m_headerList.count();
}
QVariant CMDVariableTreeModel::data(const QModelIndex &index, int role) const
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
                return Qt::Unchecked;
        }
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole && role!=Qt::DecorationRole)
         return QVariant();

    if (index.isValid())
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(index.internalPointer());

        if(item->itemType() == MDTreeItemType::VariableGroup)
        {
            if(role==Qt::DecorationRole)
            {
                if(index.column()==0)
                {
                    return m_variableGroupIcon;
                }
                else
                {
                    return QVariant();
                }
            }
            else
            {
                CMDVariableGroup *variableGroup  = (CMDVariableGroup *)item;
                if(index.column()<=0)
                {
                    return QVariant(variableGroup->localName());
                }
                else if(index.column() == 5)
                {
                    return QVariant(variableGroup->description());
                }
                else
                {
                    return QVariant();
                }
            }

        }
        else if(item->itemType() == MDTreeItemType::Variable)
        {
            if(role==Qt::DecorationRole)
            {
                if(index.column()==0)
                {
                    return m_variableIcon;
                }
                else
                {
                    return QVariant();
                }
            }
            else
            {               
                CMDRTDBVariable *variable = (CMDRTDBVariable *)item;
                int column = index.column();
                if(column<=0)
                {
                    column =0;
                }

                if(m_enableChecked)
                {
                    QVariant vt;
                    switch (column)
                    {
                    case 0:
                        vt=  QVariant(variable->localName());
                        break;
                    case 1:
                        vt= QVariant(variable->dataTypeDescription());
                        break;
                    case 2:
                        if((CMDRTDBVariableT*)variable)
                            vt= QVariant(((CMDRTDBVariableT*)variable)->getDescription());
                        else
                            vt= QVariant(variable->getDescription());
                        break;
                    default:
                        break;
                    }
                    return vt;
                }
                else
                {
                    QVariant vt;
                    switch (column)
                    {
                    case 0:
                        vt=  QVariant(variable->localName());
                        break;
                    case 1:
                        vt= QVariant(variable->dataTypeDescription());
                        break;
                    case 2:
                        if(variable->variableType() == MDVariableType::IOVariable)
                            vt =  QVariant(variable->deviceName());
                        else
                            vt = QVariant("");
                        break;
                    case 3:
                        if(variable->variableType() == MDVariableType::IOVariable ||
                                variable->variableType() == MDVariableType::MiddleVariable)
                            vt =  QVariant(variable->address());
                        else
                            vt = QVariant("");
                        break;
                    case 4:
                        vt=  QVariant(CMDMultiLanguage::instance()->value(variable->variableTypeDescription()));
                        break;
                    case 5:
                        if((CMDRTDBVariableT*)variable)
                            vt=  QVariant(((CMDRTDBVariableT*)variable)->getDescription());
                        else
                            vt=  QVariant(variable->getDescription());
                        break;
                    case 6:
                        if((CMDRTDBVariableT*)variable)
                            vt = QVariant(((CMDRTDBVariableT*)variable)->getUnit());
                        else
                            vt = QVariant(variable->getUnit());
                        break;
                    default:
                        break;
                    }
                    return vt;
                }
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags CMDVariableTreeModel::flags(const QModelIndex &index) const
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
        //!运行状态，变量，且colum=2(0,1...)可写工程值
        /*
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
        CMDVariableGroup* pGroup = (CMDVariableGroup*)m_variableGroup;
        if(pGroup != NULL && pGroup->getRTDBManager() != NULL)
        {
            if(pGroup->getRTDBManager()->isRunning())
            {
                IMDTreeItem *item = static_cast<IMDTreeItem*>(index.internalPointer());
                if (item && item->itemType() == MDTreeItemType::Variable)
                {
                    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
                    if(index.column() == 2)//工程值
                    {
                        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
                        //return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
                    }
                }
            }
        }
        */
        return QAbstractItemModel::flags(index);

        //return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
        //return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }
}
IMDTreeItem *CMDVariableTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootTreeItem;
}

QVariant CMDVariableTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(m_headerList[section]);

    return QVariant();
}

QModelIndex CMDVariableTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    IMDTreeItem *parentItem = getItem(parent);

    if(parentItem->itemType() == MDTreeItemType::VariableGroup)
    {
        CMDVariableGroup *variableGroup  = (CMDVariableGroup *)parentItem;
        IMDTreeItem *childItem = NULL;
        if(row<variableGroup->getChildVariableGroupCount() && variableGroup->getChildVariableGroupCount()>0)
        {
            childItem = variableGroup->getChildVariableGroupAt(row);
        }
        else
        {
            if(variableGroup->getChildVariableGroupCount()>0)
            {
                childItem = (IMDTreeItem *)(CMDRTDBVariable*)variableGroup->getVariableAt(row- variableGroup->getChildVariableGroupCount());
            }
            else
            {
                childItem = (IMDTreeItem *)(CMDRTDBVariable*)variableGroup->getVariableAt(row);
            }

        }
        if (childItem)
        {
            return createIndex(row, column, childItem) ;
        }
        else
        {
            return QModelIndex();
        }

    }
    return QModelIndex();
}
//1234
//bool CMDVariableTreeModel::insertRows(int row, int count, const QModelIndex &parent)
//{
//    CMDVariableGroup *parentItem = getItem(parent);
//    bool success;
//    beginInsertRows(parent, row, row+count);

//    success = parentItem->insertChildren(position, rows, columnCount());
//    endInsertRows();

//    return success;
//    return false;
//}

/*
bool CMDVariableTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    CMDVariableGroup *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, columnCount());
    endInsertRows();

    return success;
}*/
//1234
QModelIndex CMDVariableTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    IMDTreeItem *childItem = getItem(index);
    if(childItem->itemType() == MDTreeItemType::VariableGroup)
    {
        CMDVariableGroup *parentItem = (CMDVariableGroup*)childItem->parentItem(MDTreeItemType::VariableGroup);
        if (parentItem == m_rootTreeItem||parentItem==NULL)
            return QModelIndex();
        return createIndex(parentItem->getChildVariableGroupCount(), 0, parentItem);
    }
    else if(childItem->itemType() == MDTreeItemType::Variable)
    {
        CMDVariableGroup *parentItem = (CMDVariableGroup*)childItem->parentItem(MDTreeItemType::VariableGroup);
        if (parentItem == m_rootTreeItem||parentItem==NULL)
            return QModelIndex();
        return createIndex(parentItem->getVariableCount(), 0, parentItem);
    }
    return QModelIndex();
}
/*
bool CMDVariableTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    CMDVariableGroup *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->r(position, rows);
    endRemoveRows();

    return success;
}
*/
int CMDVariableTreeModel::rowCount(const QModelIndex &parent) const
{
    IMDTreeItem *parentItem = getItem(parent);
    if(parentItem->itemType() == MDTreeItemType::VariableGroup)
    {
        CMDVariableGroup *variableGroup = (CMDVariableGroup*)parentItem;
        return variableGroup->getChildVariableGroupCount()+variableGroup->getVariableCount();
    }
    return 0;
}

bool CMDVariableTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
        IMDTreeItem *item = getItem(index);
        bool result =false;//
        if(item->itemType() == MDTreeItemType::VariableGroup)
        {
            CMDVariableGroup *variableGroup = (CMDVariableGroup*)item;
            //variableGroup->setData(index.column(), value);
            result = true;
        }
        else if(item->itemType() == MDTreeItemType::Variable)
        {
            CMDRTDBVariable *variable = (CMDRTDBVariable*)item;
            //variable->setData(index.column(), value);
            result = true;
        }
        if (result)
            emit dataChanged(index, index);

        return result;
    }
}
void CMDVariableTreeModel::deleteTreeItem(const QModelIndex &itemIndex)
{
    beginRemoveRows(itemIndex.parent(), itemIndex.row(),itemIndex.row());

    IMDTreeItem *item = getItem(itemIndex);
    if(item->itemType() == MDTreeItemType::VariableGroup)
    {
        IMDVariableGroup* varGroup =(IMDVariableGroup *)item;
        IMDVariableGroup* parentVarGroup = (IMDVariableGroup*)varGroup->parentItem(MDTreeItemType::VariableGroup);
        parentVarGroup->removeChildVariableGroup(varGroup);
        delete varGroup;
    }
    else if(item->itemType() == MDTreeItemType::Variable)
    {
        IMDRTDBVariable* rtdbVariable =(IMDRTDBVariable *)item;
        IMDVariableGroup* parentVarGroup = (IMDVariableGroup*)(rtdbVariable->parentItem(MDTreeItemType::VariableGroup));
        parentVarGroup->removeVariable(rtdbVariable);
        delete rtdbVariable;
    }

    endRemoveRows();
}
