#include "MDDeviceRunTreeModel.h"
#include "MDMultiLanguage.h"
CMDDeviceRunTreeModel::CMDDeviceRunTreeModel()
{
    m_rootTreeItem = NULL;
    m_enableChecked = false;
}

CMDDeviceRunTreeModel::~CMDDeviceRunTreeModel()
{

}
//!设置图标
void CMDDeviceRunTreeModel::setTreeItemIcon(QVariant icon,MDTreeItemType itemType)
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
//!获取选中节点
QList<QModelIndex>  CMDDeviceRunTreeModel::getCheckedItems()
{
    return m_checkedList;
}
//!设置选中节点
void CMDDeviceRunTreeModel::setCheckedable(bool enableChecked)
{
    m_enableChecked = enableChecked;
}

//!多语言切换
void CMDDeviceRunTreeModel::changeLanguage()
{
    //通过触发headerDataChanged()信号实行多语言标题栏文本切换
    m_headerList.clear();
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("名称")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("描述")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("启用")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("品质")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("发送次数")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("接收次数")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("超时次数")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("重试次数")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("错误次数")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("最后读时间")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("最后写时间")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("最后错误时间")));
    emit headerDataChanged(Qt::Horizontal ,0,m_headerList.count()-1);
}
//!列表头
void CMDDeviceRunTreeModel::initial(IMDDriverManager *driverManager)
{
    //************************* Y2hw ************************
    /*
    m_headerList.append(tr("名称"));
    m_headerList.append(tr("描述"));
    m_headerList.append(tr("启用"));
    m_headerList.append(tr("品质"));
    m_headerList.append(tr("发送次数"));
    m_headerList.append(tr("接收次数"));
    m_headerList.append(tr("超时次数"));
    m_headerList.append(tr("重试次数"));
    m_headerList.append(tr("错误次数"));
    m_headerList.append(tr("最后读时间"));
    m_headerList.append(tr("最后写时间"));
    m_headerList.append(tr("最后错误时间"));
    */
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("名称")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("描述")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("启用")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("品质")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("发送次数")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("接收次数")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("超时次数")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("重试次数")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("错误次数")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("最后读时间")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("最后写时间")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("最后错误时间")));
    //-------------------------------------------------------
    m_rootTreeItem =driverManager;
    m_driverManager = driverManager;
}

void CMDDeviceRunTreeModel::setCurRootTreeItem(const QString& rootTreeItemName)
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

void CMDDeviceRunTreeModel::setExpandItem(const QModelIndex &itemIndex)
{

}

int CMDDeviceRunTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return m_headerList.count();
}
//!变量品质
QString qualityDescription(const MDVariableDataQuality &quality)
{
    QString ret = "";
    switch (quality)
    {
    case MDVariableDataQuality::AddressError:
        //ret ="地址错误";
        ret = CMDMultiLanguage::instance()->value(QObject::tr("地址错误"));
        break;
    case MDVariableDataQuality::ConnectDeviceFailed:
        //ret = "无法与设备建立通信";
        ret = CMDMultiLanguage::instance()->value(QObject::tr("无法与设备建立通信"));
        break;
    case MDVariableDataQuality::StopCommunication:
        //ret = "停止设备通信";
        ret = CMDMultiLanguage::instance()->value(QObject::tr("停止设备通信"));
        break;
    case MDVariableDataQuality::DataTypeError:
        //ret = "数据类型错误";
        ret = CMDMultiLanguage::instance()->value(QObject::tr("数据类型错误"));
        break;
    case MDVariableDataQuality::DataBlockConfigError:
        ret = CMDMultiLanguage::instance()->value(QObject::tr("数据块配置错误"));
        break;
    case MDVariableDataQuality::Good:
        //ret = "正确";
        ret = CMDMultiLanguage::instance()->value(QObject::tr("正确"));
        break;
    case MDVariableDataQuality::None:
        //ret = "没有初始化";
        ret = CMDMultiLanguage::instance()->value(QObject::tr("没有初始化"));
        break;
    case MDVariableDataQuality::ProtocolError:
        //ret = "通信协议错误";
        ret = CMDMultiLanguage::instance()->value(QObject::tr("通信协议错误"));
        break;
    case MDVariableDataQuality::TimeoutError:
        //ret = "命令超时";
        ret = CMDMultiLanguage::instance()->value(QObject::tr("命令超时"));
        break;
    case MDVariableDataQuality::DataConvertError:
        ret = CMDMultiLanguage::instance()->value(QObject::tr("工程转换失败"));
        break;
    default:
        break;
    }
    return ret;
}
//!各列的文本
QVariant CMDDeviceRunTreeModel::data(const QModelIndex &index, int role) const
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
                    vt= QVariant(device->description());
                    break;
                case 2:
                    vt= device->isEnabled()?QVariant("Enabled"):QVariant("disEnabled");
                    break;
                case 3:
                    //vt= QVariant(getMDVariableDataQualityDescription(device->);
                    break;                    
                case 4:
                    vt= QVariant(QString::number(device->transmitCount()));
                    break;
                case 5:
                    vt= QVariant(device->receiveCount());
                    break;
                case 6:
                    vt= QVariant(device->timeoutCount());
                    break;
                case 7:
                    vt= QVariant(device->retryCount());
                    break;
                case 8:
                    vt= QVariant(device->errorCount());
                    break;
                case 9:
                    vt= QVariant(device->lastReadTime().toString("yyyy-MM-dd hh:mm:ss zzz"));
                    break;
                case 10:
                    vt= QVariant(device->lastWriteTime().toString("yyyy-MM-dd hh:mm:ss zzz"));
                    break;
                case 11:
                    vt= QVariant(device->lastErrorTime().toString("yyyy-MM-dd hh:mm:ss zzz"));
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
                    vt= QVariant(db->description());
                    break;
                case 2:
                    vt= db->isEnabled()?QVariant("Enabled"):QVariant("disEnabled");
                    break;
                case 3:
                    vt= QVariant(qualityDescription(db->quality()));
                    break;
                case 4:
                    vt= QVariant(db->transmitCount());
                    break;
                case 5:
                    vt= QVariant(db->receiveCount());
                    break;
                case 6:
                    vt= QVariant(db->timeoutCount());
                    break;
                case 7:
                    vt= QVariant(db->retryCount());
                    break;
                case 8:
                    vt= QVariant(db->errorCount());
                    break;
                case 9:
                    vt= QVariant(db->lastReadTime().toString("yyyy-MM-dd hh:mm:ss zzz"));
                    break;
                case 10:
                    vt= QVariant(db->lastWriteTime().toString("yyyy-MM-dd hh:mm:ss zzz"));
                    break;
                case 11:
                    vt= QVariant(db->lastErrorTime().toString("yyyy-MM-dd hh:mm:ss zzz"));
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
                CMDRTDBVariable *variable = (CMDRTDBVariable *)item;
                int column = index.column();
                if(column<=0)
                {
                    column =0;
                }
                QVariant vt;
                switch (column)
                {
                case 0:
                    {
//                        vt= QVariant(variable->localName());
                        vt= QVariant(variable->name());
                    }

                    break;
                case 1:
                    vt= QVariant(variable->getDescription());
                    break;
                case 2:
                   vt= variable->isValid()?QVariant("Valid"):QVariant("inValid");
                    break;
                case 3:
                    vt= QVariant(qualityDescription(variable->currentQuality()));
                    break;
                case 4:
                    // vt= QVariant(db->transmitCount());
                    break;
                case 5:
                    vt= QVariant(variable->changeCount());
                    break;
                case 6:
                   // vt= QVariant(db->timeoutCount());
                    break;
                case 7:
                    //vt= QVariant(db->retryCount());
                    break;
                case 8:
                   // vt= QVariant(db->errorCount());
                    break;
                case 9:
                    vt= QVariant(variable->timeStamp().toString("yyyy-MM-dd hh:mm:ss zzz"));
                    break;
                case 10:
                    //vt= QVariant(db->lastWriteTime());
                    break;
                case 11:
                    //vt= QVariant(db->lastErrorTime());
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
//!节点状态
Qt::ItemFlags CMDDeviceRunTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags(index);
    //return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}
IMDTreeItem *CMDDeviceRunTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootTreeItem;
}

QVariant CMDDeviceRunTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(m_headerList[section]);

    return QVariant();
}

QModelIndex CMDDeviceRunTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    IMDTreeItem *parentItem= getItem(parent);
    if(parentItem == NULL)//Y2hw
        return QModelIndex();


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
bool CMDDeviceRunTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    CMDVariableGroup *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, columnCount());
    endInsertRows();

    return success;
}*/
QModelIndex CMDDeviceRunTreeModel::parent(const QModelIndex &index) const
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
bool CMDDeviceRunTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    CMDVariableGroup *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->r(position, rows);
    endRemoveRows();

    return success;
}
*/
int CMDDeviceRunTreeModel::rowCount(const QModelIndex &parent) const
{
    IMDTreeItem *parentItem = getItem(parent);
    if(parentItem == NULL)//Y2hw
        return 0;

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

bool CMDDeviceRunTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

void CMDDeviceRunTreeModel::deleteTreeItem(const QModelIndex &itemIndex)
{

}

