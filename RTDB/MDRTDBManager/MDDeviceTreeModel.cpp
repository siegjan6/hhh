#include "MDDeviceTreeModel.h"
#include "IDevice.h"
#include "IDataBlock.h"
#include "IMDRTDBVariable.h"
#include "IMDDriverManager.h"
#include "MDRTDBVariableUaBase.h"
#include "MDMultiLanguage.h"

CMDDeviceTreeModel::CMDDeviceTreeModel()
{
    m_rootTreeItem = NULL;
    m_enableChecked = false;    
}
CMDDeviceTreeModel::~CMDDeviceTreeModel()
{

}
//!设置图标
void CMDDeviceTreeModel::setTreeItemIcon(QVariant icon,MDTreeItemType itemType)
{
    switch (itemType) {
    case MDTreeItemType::Device:
        m_deviceIcon = icon;
        break;
    case MDTreeItemType::DataBlock:
        m_dataBlockIcon = icon;
        break;
    case MDTreeItemType::Variable:
        m_variableIcon = icon;
        break;
    default:
        break;
    }

}
//!获取所有选中节点
QList<QModelIndex>  CMDDeviceTreeModel::getCheckedItems()
{
    return m_checkedList;
}
void CMDDeviceTreeModel::setCheckedable(bool enableChecked)
{
    m_enableChecked = enableChecked;
}
//!列表头
void CMDDeviceTreeModel::initial(IMDDriverManager *driverManager)
{
    //****************************** Y2hw *****************************
    /*
    m_headerList.append(tr("名称"));
    m_headerList.append(tr("启用"));
    m_headerList.append(tr("描述"));
    m_headerList.append(tr("数据类型"));
    m_headerList.append(tr("地址"));
    */
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("名称")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("启用")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("描述")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("数据类型")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("地址")));
    //----------------------------------------------------------------
    m_rootTreeItem =driverManager;
    m_driverManager = driverManager;

}

void CMDDeviceTreeModel::setCurRootTreeItem(const QString& rootTreeItemName)
{
    if(!rootTreeItemName.isEmpty())
    {
        QString strDevName = rootTreeItemName;
        QString strDBName = "";
        int nIndex = rootTreeItemName.indexOf('.');
        if(nIndex > 0)//db
        {
            strDevName = rootTreeItemName.left(nIndex);
            strDBName = rootTreeItemName.mid(nIndex + 1);
            IDevice* pdev = m_driverManager->getDevice(strDevName);
            if(pdev != NULL)
            {
                IDataBlock* pdb = pdev->getDataBlock(strDBName);
                if(pdb != NULL)
                    m_rootTreeItem = pdb;
            }
        }
        else
        {
            IDevice* pdev = m_driverManager->getDevice(rootTreeItemName);
            if(pdev != NULL)
                m_rootTreeItem = pdev;
        }
    }
}

void CMDDeviceTreeModel::setExpandItem(const QModelIndex &itemIndex)
{
    setData(itemIndex, true, Qt::DecorationRole);
}

int CMDDeviceTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return m_headerList.count();
}
//!各列文本文本
QVariant CMDDeviceTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    if (role != Qt::DisplayRole && role != Qt::EditRole && role!=Qt::DecorationRole)
        return QVariant();

    if (index.isValid())
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(index.internalPointer());

        if(item->itemType() == MDTreeItemType::Device)
        {
            if(role==Qt::DecorationRole)
            {
                if(index.column()==0)
                {
                    return m_deviceIcon;
                }
                else
                {
                    return QVariant();
                }
            }
            else
            {
                IDevice *device  = (IDevice *)item;
                int column = index.column();
                if(column<=0)
                {
                    column =0;
                }
                QVariant vt;
                switch (column)
                {
                case 0:
                    vt= QVariant(device->name());
                    break;
                case 1:
                    vt= device->isEnabled()?QVariant("Enabled"):QVariant("disEnabled");
                    break;
                case 2:
                    vt= QVariant(device->description());
                    break;
                default:
                    break;
                }
                return vt;
            }
        }
        else if(item->itemType() == MDTreeItemType::DataBlock)
        {
            if(role==Qt::DecorationRole)
            {
                if(index.column()==0)
                {
                    return m_dataBlockIcon;
                }
                else
                {
                    return QVariant();
                }
            }
            else
            {
                IDataBlock *db = (IDataBlock *)item;
                int column = index.column();
                if(column<=0)
                {
                    column =0;
                }
                QVariant vt;
                switch (column)
                {
                case 0:
                    vt= QVariant(db->name());
                    break;
                case 1:
                    vt= db->isEnabled()?QVariant("Enabled"):QVariant("disEnabled");
                    break;
                case 2:
                    vt= QVariant(db->description());
                    break;
                default:
                    break;
                }
                return vt;
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
                CMDRTDBVariableT *variable = (CMDRTDBVariableT *)item;
                int column = index.column();
                if(column<=0)
                {
                    column =0;
                }
                QVariant vt;
                switch (column)
                {
                case 0:
                    vt= QVariant(variable->name());
                    break;
                case 1:
                    vt= variable->isValid()?QVariant("Valid"):QVariant("inValid");
                    break;
                case 2:
                    vt= QVariant(variable->getDescription());
                    break;
                case 3:
                    vt= QVariant(variable->dataTypeDescription());
                    break;
                case 4:
                    vt= QVariant(variable->address());
                    break;
                default:
                    break;
                }
                return vt;
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags CMDDeviceTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}
IMDTreeItem *CMDDeviceTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootTreeItem;
}
//!列表头
QVariant CMDDeviceTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(m_headerList[section]);

    return QVariant();
}

QModelIndex CMDDeviceTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    IMDTreeItem *parentItem= getItem(parent);

    if(parentItem->itemType() == MDTreeItemType::DriverManager)
    {
        IMDDriverManager *driverManager  = (IMDDriverManager*)parentItem;
        IMDTreeItem *childItem =  driverManager->getDeviceAt(row);
        if (childItem)
        {
            return createIndex(row, column, childItem) ;
        }
        else
        {
            return QModelIndex();
        }
    }
    else if(parentItem->itemType() == MDTreeItemType::Device)
    {
        IDevice *device  = (IDevice*)parentItem;
        IMDTreeItem *childItem =  (IMDTreeItem *)device->getDataBlockAt(row);
        if (childItem)
        {
            return createIndex(row, column, childItem) ;
        }
        else
        {
            return QModelIndex();
        }
    }
    else if(parentItem->itemType() == MDTreeItemType::DataBlock)
    {
        IDataBlock *db  = (IDataBlock*)parentItem;
        IMDTreeItem *childItem =  (IMDTreeItem *)db->getVariableAt(row);
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

/*
bool CMDDeviceTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    CMDVariableGroup *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, columnCount());
    endInsertRows();

    return success;
}*/
QModelIndex CMDDeviceTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();


    IMDTreeItem *childItem= getItem(index);
    if(childItem->itemType() == MDTreeItemType::Device)
    {
        IMDDriverManager *parentItem  = (IMDDriverManager*)childItem->parentItem(MDTreeItemType::DriverManager);
        if (parentItem == m_rootTreeItem||parentItem==NULL)
            return QModelIndex();
        return createIndex(parentItem->getDeviceCount(), 0, parentItem);
    }
    else if(childItem->itemType() == MDTreeItemType::DataBlock)
    {
        IDevice *parentItem = (IDevice*)childItem->parentItem(MDTreeItemType::Device);
        if (parentItem == m_rootTreeItem||parentItem==NULL)
            return QModelIndex();
        return createIndex(parentItem->getDataBlockCount(), 0, parentItem);
    }
    else if(childItem->itemType() == MDTreeItemType::Variable)
    {
        IDataBlock *parentItem = (IDataBlock*)childItem->parentItem(MDTreeItemType::DataBlock);
        if (parentItem == m_rootTreeItem||parentItem==NULL)
            return QModelIndex();
        return createIndex(parentItem->getVariableCount(), 0, parentItem);
    }
    return QModelIndex();
}
/*
bool CMDDeviceTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    CMDVariableGroup *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->r(position, rows);
    endRemoveRows();

    return success;
}
*/
int CMDDeviceTreeModel::rowCount(const QModelIndex &parent) const
{
    IMDTreeItem *parentItem = getItem(parent);
    if(parentItem->itemType() == MDTreeItemType::DriverManager)
    {
        IMDDriverManager *driverManager =  (IMDDriverManager*)parentItem;
        return driverManager->getDeviceCount();
    }
    else if(parentItem->itemType() == MDTreeItemType::Device)
    {
        IDevice *device =  (IDevice*)parentItem;
        return device->getDataBlockCount();
    }
    else if(parentItem->itemType() == MDTreeItemType::DataBlock)
    {
        IDataBlock *db =  (IDataBlock*)parentItem;
        return db->getVariableCount();
    }
    return 0;
}

bool CMDDeviceTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;


    bool result =false;
    /*/

     IMDTreeItem *item = getItem(index);
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
    /*/
    return result;
}
//!多语言切换
void CMDDeviceTreeModel::changeLanguage()
{
    //通过触发headerDataChanged()信号实行多语言标题栏文本切换
    m_headerList.clear();
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("名称")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("启用")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("描述")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("数据类型")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("地址")));
    emit headerDataChanged(Qt::Horizontal ,0,m_headerList.count()-1);
}
//!删除某节点
void CMDDeviceTreeModel::deleteTreeItem(const QModelIndex &itemIndex)
{
    beginRemoveRows(itemIndex.parent(), itemIndex.row(),itemIndex.row());

    IMDTreeItem *item = getItem(itemIndex);
    if(item->itemType() == MDTreeItemType::Device)
    {
        IDevice* device =(IDevice *)item;
        IMDDriverManager* driverManager = (IMDDriverManager*)device->parentItem(MDTreeItemType::DriverManager);
        driverManager->removeDevice(device->name());
    }
    else if(item->itemType() == MDTreeItemType::DataBlock)
    {
        IDataBlock* db =(IDataBlock *)item;
        IDevice* device = (IDevice*)(db->parentItem(MDTreeItemType::Device));
        device->removeDataBlock(db->name());
    }/*
    else if(item->itemType() == MDTreeItemType::Variable)
    {
        IMDRTDBVariable* rtdbVariable =(IMDRTDBVariable *)item;
        IMDVariableGroup* parentVarGroup = (IMDVariableGroup*)(rtdbVariable->parentItem(MDTreeItemType::VariableGroup));
        parentVarGroup->removeVariable(rtdbVariable);
        delete rtdbVariable;
    }*/

    endRemoveRows();
}
