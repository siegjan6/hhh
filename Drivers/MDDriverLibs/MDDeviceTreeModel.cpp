#include "MDDeviceTreeModel.h"
#include "IDevice.h"
#include "IDataBlock.h"
#include "IMDRTDBVariable.h"
#include "IMDDriverManager.h"

CMDDeviceTreeModel::CMDDeviceTreeModel()
{
    m_rootTreeItem = NULL;
}
CMDDeviceTreeModel::~CMDDeviceTreeModel()
{

}
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
void CMDDeviceTreeModel::initial(IMDDriverManager *driverManager)
{
    m_headerList.append("名称");
    m_headerList.append("数据类型");
    m_headerList.append("设备");
    m_headerList.append("地址");
    m_headerList.append("变量类型");
    m_headerList.append("描述");

    m_rootTreeItem =driverManager;
    /*
    CMDDevice *device = new CMDDevice();
    device->setName("device1");

    CMDDataBlock *db = new CMDDataBlock();
    db->setName("db11");
    device->addDataBlock(db);
    db = new CMDDataBlock();
    db->setName("db12");
    device->addDataBlock(db);
    db = new CMDDataBlock();
    db->setName("db12");
    device->addDataBlock(db);

    deviceManager->addDevice(device);

    device = new CMDDevice();
    device->setName("device2");
    db = new CMDDataBlock();
    db->setName("db21");
    device->addDataBlock(db);
    db = new CMDDataBlock();
    db->setName("db22");
    device->addDataBlock(db);
    deviceManager->addDevice(device);

    device = new CMDDevice();
    device->setName("device3");
    db = new CMDDataBlock();
    db->setName("db31");
    device->addDataBlock(db);
    db = new CMDDataBlock();
    db->setName("db32");
    device->addDataBlock(db);
    db = new CMDDataBlock();
    db->setName("db33");
    device->addDataBlock(db);
    db = new CMDDataBlock();
    db->setName("db34");

    QString error;
    CMDRTDBVariable *var1 = new CMDRTDBVariable();
    var1->setLocalName("fV4");
    db->addVariable(var1,error);

    var1 = new CMDRTDBVariable();
    var1->setLocalName("fV5");
    db->addVariable(var1,error);

    var1 = new CMDRTDBVariable();
    var1->setLocalName("fV6");
    db->addVariable(var1,error);

    device->addDataBlock(db);

    deviceManager->addDevice(device);

    device = new CMDDevice();
    device->setName("device4");

    db = new CMDDataBlock();
    db->setName("db41");

    var1 = new CMDRTDBVariable();

    var1->setLocalName("fV1");
    db->addVariable(var1,error);

    var1 = new CMDRTDBVariable();
    var1->setLocalName("fV2");
    db->addVariable(var1,error);

    device->addDataBlock(db);

    deviceManager->addDevice(device);
 */

}

int CMDDeviceTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return m_headerList.count();
}
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
                if(index.column()<=0)
                {
                    return QVariant(device->name());
                }
                else
                {
                    return QVariant();
                }
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
                if(index.column()<=0)
                {
                    return QVariant(db->name());
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
                IMDRTDBVariable *variable = (IMDRTDBVariable *)item;
                if(index.column()<=0)
                {
                    return QVariant(variable->getLocalName());
                }
                else
                {
                    return QVariant();
                }
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags CMDDeviceTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
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
        IMDDriverManager *parentItem  = (IMDDriverManager*)childItem->parent(MDTreeItemType::DriverManager);
        if (parentItem == m_rootTreeItem||parentItem==NULL)
            return QModelIndex();
        return createIndex(parentItem->getDeviceCount(), 0, parentItem);
    }
    else if(childItem->itemType() == MDTreeItemType::DataBlock)
    {
        IDevice *parentItem = (IDevice*)childItem->parent(MDTreeItemType::Device);
        if (parentItem == m_rootTreeItem||parentItem==NULL)
            return QModelIndex();
        return createIndex(parentItem->getDataBlockCount(), 0, parentItem);
    }
    else if(childItem->itemType() == MDTreeItemType::Variable)
    {
        IDataBlock *parentItem = (IDataBlock*)childItem->parent(MDTreeItemType::DataBlock);
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
