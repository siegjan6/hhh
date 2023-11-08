#include "MDVariableRunTreeModel.h"
#include "MDVariableGroup.h"
#include "MDRTDBVariableUaBase.h"
#include "IMDTreeItem.h"
#include "MDMultiLanguage.h"

CMDVariableRunTreeModel::CMDVariableRunTreeModel()
{
    m_rootTreeItem = NULL;
    m_enableChecked = false;
}

CMDVariableRunTreeModel::~CMDVariableRunTreeModel()
{

}
//! 设置树节点属性
void CMDVariableRunTreeModel::setTreeItemIcon(QVariant icon,MDTreeItemType itemType)
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
//! 获取选中节点
QList<QModelIndex>  CMDVariableRunTreeModel::getCheckedItems()
{
    return m_checkedList;
}
//!选中使能
void CMDVariableRunTreeModel::setCheckedable(bool enableChecked)
{
    m_enableChecked = enableChecked;
}
//列表头
void CMDVariableRunTreeModel::initial(IMDVariableGroup *rootVariableGroup)
{
    //****************************** Y2hw *****************************
    /*
    m_headerList.append("名称");
    m_headerList.append("描述");
    m_headerList.append("工程值");
    m_headerList.append("工程数据类型");
    m_headerList.append("原始值");
    m_headerList.append("原始数据类型");
    m_headerList.append("设备");
    m_headerList.append("地址");
    m_headerList.append("品质");
    m_headerList.append("时间戳");
    m_headerList.append("更新次数");
    m_headerList.append("变量类型");
    m_headerList.append("有效性");
    */
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("名称")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("描述")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("工程值")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("工程数据类型")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("原始值")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("原始数据类型")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("设备")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("地址")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("品质")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("时间戳")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("更新次数")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("变量类型")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("有效性")));
    //----------------------------------------------------------------
    m_rootTreeItem = rootVariableGroup;
    m_variableGroup = rootVariableGroup;
   // connect((CMDVariableGroup*)rootVariableGroup, SIGNAL(itemChanged()), this, SLOT(onTimerOut()));
/*
    QString strError;
    rootVariableGroup->setLocalName("VariableGroups");
    CMDVariableGroup *pVariableGroup = new CMDVariableGroup();
    pVariableGroup->setLocalName("Group1");
    CMDVariableGroup*pVariableGroup2 = new CMDVariableGroup();
    pVariableGroup2->setLocalName("Group11");
    pVariableGroup->addChildVariableGroup(pVariableGroup2,strError);
    pVariableGroup2 = new CMDVariableGroup();
    pVariableGroup2->setLocalName("Group12");
    pVariableGroup->addChildVariableGroup(pVariableGroup2,strError);

    pVariableGroup2 = new CMDVariableGroup();
        pVariableGroup2->setLocalName("Group13");
        pVariableGroup->addChildVariableGroup(pVariableGroup2,strError);

        pVariableGroup2 = new CMDVariableGroup();
            pVariableGroup2->setLocalName("Group14");
            pVariableGroup->addChildVariableGroup(pVariableGroup2,strError);


    rootVariableGroup->addChildVariableGroup(pVariableGroup,strError);

    pVariableGroup = new CMDVariableGroup();
    pVariableGroup->setLocalName("Group2");
    pVariableGroup2 = new CMDVariableGroup();
    pVariableGroup2->setLocalName("Group21dddddddddddddddddd");
    pVariableGroup->addChildVariableGroup(pVariableGroup2,strError);
    pVariableGroup2 = new CMDVariableGroup();
        pVariableGroup2->setLocalName("Group22");
        pVariableGroup->addChildVariableGroup(pVariableGroup2,strError);

        pVariableGroup2 = new CMDVariableGroup();
            pVariableGroup2->setLocalName("Group23");
            pVariableGroup->addChildVariableGroup(pVariableGroup2,strError);

            QString error;
    CMDRTDBVariable *var1 = new CMDRTDBVariable();
        var1->setLocalName("fV4ddddddddddddddddddddd");
        pVariableGroup->addVariable(var1,error);

        var1 = new CMDRTDBVariable();
            var1->setLocalName("fV5");
            pVariableGroup->addVariable(var1,error);

            var1 = new CMDRTDBVariable();
                var1->setLocalName("fV6");
                pVariableGroup->addVariable(var1,error);


    rootVariableGroup->addChildVariableGroup(pVariableGroup,strError);*/
}
void CMDVariableRunTreeModel::setCurRootTreeItem(const QString& rootTreeItemName)
{
    if(rootTreeItemName.isEmpty())
        return;

    IMDTreeItem * pGroup = m_variableGroup->getChildVariableGroupWithName(rootTreeItemName);
    if(pGroup != NULL)
        m_rootTreeItem = pGroup;

}
void CMDVariableRunTreeModel::setExpandItem(const QModelIndex &itemIndex)
{
}
//!修改列表头文本语言
void CMDVariableRunTreeModel::changeLanguage()
{
    //通过触发headerDataChanged()信号实行多语言标题栏文本切换
    m_headerList.clear();
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("名称")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("描述")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("工程值")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("工程数据类型")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("原始值")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("原始数据类型")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("设备")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("地址")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("品质")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("时间戳")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("更新次数")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("变量类型")));
    m_headerList.append(CMDMultiLanguage::instance()->value(tr("有效性")));
    emit headerDataChanged(Qt::Horizontal ,0,m_headerList.count()-1);
}

int CMDVariableRunTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return m_headerList.count();
}
QVariant CMDVariableRunTreeModel::data(const QModelIndex &index, int role) const
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
                QVariant vt;                
                switch (column)
                {
                case 0:
                    vt= QVariant(variable->localName());
                    break;
                case 1:
                    vt=QVariant(variable->getDescription());
                    break;
                case 2:
                    if(variable->dataType()==MDDataType::Float)
                    {
                        vt= QVariant(QString::number(variable->currentValue().toFloat(),'f',variable->getPrecision()));
                    }
                    else if(variable->dataType()==MDDataType::Double)
                    {
                        vt= QVariant(QString::number(variable->currentValue().toDouble(),'f',variable->getPrecision()));
                    }
                    else
                    {
                        vt= QVariant(variable->currentValue());
                    }
                    break;
                case 3:
                    vt= QVariant(variable->dataTypeDescription());
                    break;
                case 4:
                    if(variable->originalDataType()==MDDataType::Float)
                    {
                        vt= QVariant(QString::number(variable->originalValue().toFloat(),'f',variable->getPrecision()+1));
                    }
                    else if(variable->dataType()==MDDataType::Double)
                    {
                        vt= QVariant(QString::number(variable->originalValue().toDouble(),'f',variable->getPrecision()+1));
                    }
                    else
                    {
                        vt= QVariant(variable->originalValue());
                    }
                    break;
                case 5:
                    vt= QVariant(variable->originalDataTypeDescription());
                    break;
                case 6:
                    vt= QVariant(variable->deviceName());
                    break;
                case 7:
                    vt= QVariant(variable->address());
                    break;
                case 8:
                    vt= QVariant(variable->currentQualityDescription());
                    break;
                case 9:
                    vt= QVariant(variable->timeStamp().toString("yyyy-MM-dd hh:mm:ss zzz"));
                    break;
                case 10:
                    vt=  QVariant(variable->changeCount());
                    break;
                case 11:
                    vt=  QVariant(CMDMultiLanguage::instance()->value(variable->variableTypeDescription()));
                    break;
                case 12:
                   // vt=  QVariant(variable->isValid());
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
//!树节点属性
Qt::ItemFlags CMDVariableRunTreeModel::flags(const QModelIndex &index) const
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
IMDTreeItem *CMDVariableRunTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        IMDTreeItem *item = static_cast<IMDTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootTreeItem;
}

QVariant CMDVariableRunTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant(m_headerList[section]);

    return QVariant();
}

QModelIndex CMDVariableRunTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    IMDTreeItem *parentItem = getItem(parent);
    if(parentItem == NULL)//Y2hw
        return QModelIndex();

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

/*
bool CMDVariableRunTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    CMDVariableGroup *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, columnCount());
    endInsertRows();

    return success;
}*/
QModelIndex CMDVariableRunTreeModel::parent(const QModelIndex &index) const
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
bool CMDVariableRunTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    CMDVariableGroup *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->r(position, rows);
    endRemoveRows();

    return success;
}
*/
int CMDVariableRunTreeModel::rowCount(const QModelIndex &parent) const
{
    IMDTreeItem *parentItem = getItem(parent);
    if(parentItem == NULL)//Y2hw
        return 0;
    if(parentItem->itemType() == MDTreeItemType::VariableGroup)
    {
        CMDVariableGroup *variableGroup = (CMDVariableGroup*)parentItem;
        return variableGroup->getChildVariableGroupCount()+variableGroup->getVariableCount();
    }
    return 0;
}

bool CMDVariableRunTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
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
//!删除树节点
void CMDVariableRunTreeModel::deleteTreeItem(const QModelIndex &itemIndex)
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
