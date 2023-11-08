#include "MDPowerGroupTreeWindowForActionSPUI.h"
#include <QTreeWidget>
#include "MDPowerGroupConfiger.h"
#include "MDPowerGroup.h"

#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "MDObjectTreeItem.h"
#include "MDStringResource.h"

CMDPowerGroupTreeWindowForActionSPUI::CMDPowerGroupTreeWindowForActionSPUI(QWidget *parent)
{
    m_configer = NULL;
}

CMDPowerGroupTreeWindowForActionSPUI::~CMDPowerGroupTreeWindowForActionSPUI()
{
}

/*!
\section 初始化
*/
void CMDPowerGroupTreeWindowForActionSPUI::init(CMDPowerGroupConfiger *configer)
{
    m_configer = configer;
    CMDObjectTreeWindow::init(NULL, configer);

    m_powerGroupIcon =QIcon(":/images/device.png");
    m_accountIcon = QIcon(":/images/datablock.png");

    setWindowTitle(STRANS(STRING_POWER_GROUP));
}

/*!
\section 重载的接口
*/
void CMDPowerGroupTreeWindowForActionSPUI::buildObjectTree(CMDObjectTreeItem* rootItem)
{
    if(!m_configer)
        return;

    QList<CMDPowerGroup*> objectList;
    m_configer->configedList(objectList);

    // 原始的动作 ---》通用的 treeitem，rootItem 管理全部 treeitem
    // --------------------------------------------------------------------------------
    QList<CMDPowerGroup*>::iterator iObject;
    for (iObject = objectList.begin(); iObject != objectList.end(); ++iObject)
    {
        CMDPowerGroup* configProperty = *iObject;
        CMDObjectTreeItem* objectItem = new CMDObjectTreeItem((qint16)MDObjectType::powerGroup, configProperty, rootItem);

        const QStringList& accountList = configProperty->accountList();
        QList<QString>::const_iterator iAcount;
        for (iAcount = accountList.constBegin(); iAcount != accountList.constEnd(); ++iAcount)
        {
            CMDObjectTreeItem* acountItem = new CMDObjectTreeItem((qint16)MDObjectType::account, (void*)&(*iAcount), objectItem);
            objectItem->appendChild(acountItem);
        }

        rootItem->appendChild(objectItem);
    }
}

QString CMDPowerGroupTreeWindowForActionSPUI::objectName(void* objectPtr)
{
    CMDPowerGroup* cfg = (CMDPowerGroup*)objectPtr;
    return (cfg ? cfg->name() : "");
}

bool CMDPowerGroupTreeWindowForActionSPUI::resultOnSetData(qint16 objectType)
{
    bool result = false;
    if(objectType == (qint16)MDObjectType::powerGroup)
    {
        result = true;
    }
    return result;
}

QVariant CMDPowerGroupTreeWindowForActionSPUI::objectData(bool enableChecked, CMDObjectTreeItem *item, int column, int role) const
{
    if(item->type() == (qint16)MDObjectType::powerGroup)
    {
        if(role==Qt::DecorationRole)
            return (column==0 ? m_powerGroupIcon : QVariant());
        else
        {
            CMDPowerGroup* prop  = static_cast<CMDPowerGroup*>(item->ptr());
            return powerGroupData(enableChecked, prop, column);
        }
    }
    else if(item->type() == (qint16)MDObjectType::account)
    {
        if(role==Qt::DecorationRole)
            return (column == 0 ? m_accountIcon : QVariant());
        else
        {
            QString* accountName  = static_cast<QString*>(item->ptr());
            return accountData(*accountName, column);
        }
    }
    else return QVariant();
}

// 实际的列，参考 initHeader()
QVariant CMDPowerGroupTreeWindowForActionSPUI::powerGroupData(bool enableChecked, CMDPowerGroup* prop, int propIndex) const
{
    if(enableChecked) // 用于选择的界面，可减少列数
    {
        if(propIndex<=0)
            return QVariant(prop->name());
        else if(propIndex==1)
            return QVariant(PTRANS(prop->comment()));
        else
            return QVariant();
    }
    else
    {
        if(propIndex<=0)
            return QVariant(prop->name());
        else if(propIndex==1)
            return QVariant(PTRANS(prop->comment()));
        else if(propIndex==2)
            return QVariant(prop->isEnable());
        else
            return QVariant();
    }
}

// 权限组绑定的帐户
QVariant CMDPowerGroupTreeWindowForActionSPUI::accountData(const QString& accountName, int propIndex) const
{
    if(propIndex<=0)
        return QVariant(accountName);
    else
        return QVariant();
}

bool CMDPowerGroupTreeWindowForActionSPUI::isTopObject(qint16 objectType)
{
    return (objectType == (qint16)MDObjectType::powerGroup);
}

void CMDPowerGroupTreeWindowForActionSPUI::initTreeColumnWidth(bool enableChecked, QTreeView *treeView)
{
    if(enableChecked)
    {
        treeView->setColumnWidth(0, 200);
        treeView->setColumnWidth(1, 100);
    }
    else
    {
        treeView->setColumnWidth(0, 200);
        treeView->setColumnWidth(1, 140);
        treeView->setColumnWidth(3, 100);
    }
}

void CMDPowerGroupTreeWindowForActionSPUI::initHeader(bool enableChecked, QStringList& headerList)
{
    if(enableChecked) // 用于选择的界面，可减少列数
    {
        headerList.append(STRING_NAME);
        headerList.append(STRING_COMMENT);
    }
    else
    {
        headerList.append(STRING_NAME);
        headerList.append(STRING_COMMENT);
        headerList.append(STRING_ENABLE);
    }
}
