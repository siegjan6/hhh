#include "MDPowerGroupTreeWindow.h"

#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonArray>

#include "MDPowerGroupConfiger.h"
#include "MDPowerGroup.h"

#include "MDPowerGroupMainDialog.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "MDObjectTreeItem.h"
#include "MDPowerGroupUIImp.h"
#include "MDAccountUI.h"
#include "MDStringResource.h"
#include "PropertyBrowser.h"

CMDPowerGroupTreeWindow::CMDPowerGroupTreeWindow(QWidget *parent)
{
    m_configer = NULL;
    m_UIImp = NULL;
    m_batchNewProperty = NULL;
}

CMDPowerGroupTreeWindow::~CMDPowerGroupTreeWindow()
{
    if(m_batchNewProperty)
        delete m_batchNewProperty;
}

/*!
\section 初始化
*/
void CMDPowerGroupTreeWindow::init(CMDPowerGroupUIImp* UI, CMDPowerGroupConfiger *configer)
{
    m_UIImp = UI;
    m_configer = configer;
    CMDObjectTreeWindow::init(UI, configer);

    m_powerGroupIcon =QIcon(":/images/device.png");
    m_accountIcon = QIcon(":/images/datablock.png");

    setWindowTitle(STRANS(STRING_POWER_GROUP_TEXT));

    m_PowerGroupForBrowser.setConfiger(configer);
    m_PowerGroupForBrowser.setObjTreeWindow(this);
}

bool CMDPowerGroupTreeWindow::selectAccounts(const QStringList& sourceList, QStringList& destList)
{
    return (m_UIImp->m_accountUI ? m_UIImp->m_accountUI->selectAccounts(sourceList, destList) : false);
}

/*!
\section 复制与粘贴
*/
QString CMDPowerGroupTreeWindow::createName(const QString& sourceName)
{
    int count = m_configer->count();
    for(int i= 0; i < count; i++)
    {
      QString name = QString("%1_%2").arg(sourceName).arg(QString::number(i+1));
      if(!m_configer->find(name))
          return name;
    }
    return QString("%1_%2").arg(sourceName).arg(QString::number(count+1));
}

/*!
\section 重载的接口
*/
bool CMDPowerGroupTreeWindow::save()
{
    if(m_configer)
        return m_configer->saveToFile();
    return false;
}

void CMDPowerGroupTreeWindow::buildObjectTree(CMDObjectTreeItem* rootItem)
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

bool CMDPowerGroupTreeWindow::addObject(void* objectUI, void *objectConfiger, qint16 subType)
{
    bool result = false;
    CMDPowerGroupMainDialog dlg(this);
    dlg.initial(this, objectConfiger, subType);
    dlg.exec();
    result = (QDialog::Accepted == dlg.result());
    return result;
}

bool CMDPowerGroupTreeWindow::configObject(void* objectUI, void *objectConfiger, QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    bool result = false;
    if(1 == selectedTreeItems.size())
    {
        CMDObjectTreeItem* item = (CMDObjectTreeItem*)selectedTreeItems.first();
        if(item->type() == (qint16)MDObjectType::powerGroup)
        {
            CMDPowerGroupMainDialog dlg(this);
            dlg.initial(this, objectConfiger, item->ptr());
            dlg.setWindowTitle(STRANS(STRING_POWER_GROUP_TEXT) + " - " + objectName(item->ptr()));
            dlg.exec();
            result = (QDialog::Accepted == dlg.result());
        }
    }
    else
    {
        QList<CMDPowerGroup*> MDObjects;
        foreach (CMDObjectTreeItem* item, selectedTreeItems)
            MDObjects.append((CMDPowerGroup*)item->ptr());
        m_PowerGroupForBrowser.setMDObjects(MDObjects);

        QObject* objForBatchConfig = &m_PowerGroupForBrowser;
        QString title = QObject::tr("批量编辑 - 权限组");
        result = (QDialog::Accepted == showBatchConfigDialog(objForBatchConfig, title));
    }
    return result;
}

bool CMDPowerGroupTreeWindow::configObjectInBrowser(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    bool result = false;
    if(selectedTreeItem->type() == (qint16)MDObjectType::powerGroup)
    {
        CMDPowerGroupUIImp* uiImp = (CMDPowerGroupUIImp*)objectUI;
        if(uiImp)
        {
            m_PowerGroupForBrowser.setMDObject((CMDPowerGroup*)selectedTreeItem->ptr());
            CPropertyBrowser* browser = uiImp->m_studioInterface->propertyBrowser();
            browser->setObject(&m_PowerGroupForBrowser);
            browser->updateProperty();
        }
    }
    return result;
}

bool CMDPowerGroupTreeWindow::deleteObject(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    QString str;
    if(1 == selectedTreeItems.size())
    {
        QString name = objectName(selectedTreeItems.first()->ptr());
        str = QString(STRANS(STRING_CONFIRM_DELETE_POWER_GROUP) + ": %1 ?").arg(name);
    }
    else
        str = QString(STRANS(STRING_CONFIRM_DELETE_MULTI_POWER_GROUP)).arg(selectedTreeItems.size());

    QMessageBox::StandardButton reply = QMessageBox::question(this, STRANS(STRING_OK), str, QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No)
        return false;

    foreach (CMDObjectTreeItem* item, selectedTreeItems)
    {
        QString name = objectName(item->ptr());
        m_configer->delete1(name);
    }
    return true;
}

void CMDPowerGroupTreeWindow::objectDeleted(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    if(!m_UIImp)
        return;

    if(selectedTreeItems.size() <= 0)
        return;

    m_PowerGroupForBrowser.setMDObject((CMDPowerGroup*)selectedTreeItems.first()->ptr());
    CPropertyBrowser* browser = m_UIImp->m_studioInterface->propertyBrowser();
    browser->setObject(&m_PowerGroupForBrowser);
    browser->updateProperty();
}

bool CMDPowerGroupTreeWindow::pasteObject(void* objectPtr)
{
    CMDPowerGroup* source = (CMDPowerGroup*)objectPtr;

    QString destName = createName(source->name());
    CMDPowerGroup dest(*source);
    dest.setName(destName);
    return m_configer->add(&dest);
}

QString CMDPowerGroupTreeWindow::objectName(void* objectPtr)
{
    CMDPowerGroup* cfg = (CMDPowerGroup*)objectPtr;
    return (cfg ? cfg->name() : "");
}

bool CMDPowerGroupTreeWindow::resultOnSetData(qint16 objectType)
{
    bool result = false;
    if(objectType == (qint16)MDObjectType::powerGroup)
    {
        result = true;
    }
    return result;
}

QVariant CMDPowerGroupTreeWindow::objectData(bool enableChecked, CMDObjectTreeItem *item, int column, int role) const
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

/*!
\section 批量增加
*/
bool CMDPowerGroupTreeWindow::batchAddObject(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    bool result = false;
    if(selectedTreeItem->type() == (qint16)MDObjectType::powerGroup)
    {
        CMDPowerGroup* sourceProperty = (CMDPowerGroup*)selectedTreeItem->ptr();
        if(m_batchNewProperty)
            delete m_batchNewProperty;
        m_batchNewProperty = new CMDPowerGroup(*sourceProperty);
        QString name = sourceProperty->name();
        QString comment = sourceProperty->comment();
        QJsonObject jsName = propToJson("Name", QObject::tr("名字"), name, name + "(*)", 1, 1, 10);
        QJsonObject jsComment = propToJson("Comment", QObject::tr("说明"), comment, comment + "(*)", 1, 1, 10);

        QJsonArray ja;
        ja.append(jsName);
        ja.append(jsComment);

        showBatchAddDialog(ja);
    }
    return result;
}

bool CMDPowerGroupTreeWindow::onBatchAddOneObject(const QJsonArray &ja, QString& resultMsg)
{
    if(!m_batchNewProperty)
    {
        resultMsg = "!m_batchNewProperty";
        return false;
    }

    foreach (const QJsonValue& v , ja)
    {
        QString propID = v.toObject().value("PropID").toString();
        QString value = v.toObject().value("Value").toString();
        if("Name" == propID)
        {
            m_batchNewProperty->setName(value);
        }
        else if("Comment" == propID)
        {
            m_batchNewProperty->setComment(value);
        }
    }

    bool result = m_configer->add(m_batchNewProperty);
    if(!result)
        resultMsg = m_batchNewProperty->name();
    return result;
}

// 实际的列，参考 initHeader()
QVariant CMDPowerGroupTreeWindow::powerGroupData(bool enableChecked, CMDPowerGroup* prop, int propIndex) const
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
        else
            return QVariant();
    }
}

// 权限组绑定的帐户
QVariant CMDPowerGroupTreeWindow::accountData(const QString& accountName, int propIndex) const
{
    if(propIndex<=0)
        return QVariant(accountName);
    else
        return QVariant();
}

bool CMDPowerGroupTreeWindow::isTopObject(qint16 objectType)
{
    return (objectType == (qint16)MDObjectType::powerGroup);
}

void CMDPowerGroupTreeWindow::initTreeColumnWidth(bool enableChecked, QTreeView *treeView)
{
    if(enableChecked)
    {
        treeView->setColumnWidth(0, 200);
        treeView->setColumnWidth(1, 140);
    }
    else
    {
        treeView->setColumnWidth(0, 200);
        treeView->setColumnWidth(1, 140);
    }
}

void CMDPowerGroupTreeWindow::initHeader(bool enableChecked, QStringList& headerList)
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
    }
}

void CMDPowerGroupTreeWindow::onMenuItemVisible(CMDObjectTreeItem *selectedTreeItem)
{
    bool visible = false;
    if (!selectedTreeItem)
        visible = false;
    else
        visible = (selectedTreeItem->type() == (qint16)MDObjectType::powerGroup);

    setMenuItemVisible(visible);
}

bool CMDPowerGroupTreeWindow::initAddAction()
{
    return CMDObjectTreeWindow::initAddAction();
}

void CMDPowerGroupTreeWindow::initSubMenuOfAdd(QMenu* subMenu)
{

}

/*!
\section 多语言
*/
void CMDPowerGroupTreeWindow::changeLanguage()
{
    setWindowTitle(STRANS(STRING_POWER_GROUP_TEXT));

    CMDObjectTreeWindow::changeLanguage();
}
