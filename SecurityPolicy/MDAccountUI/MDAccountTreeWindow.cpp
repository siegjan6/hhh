#include "MDAccountTreeWindow.h"

#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonArray>

#include "MDProjectConfiger.h"
#include "MDAccountConfiger.h"
#include "MDAccountProperty_Config.h"
#include "MDSecurityPolicyConfiger.h"

#include "MDAccountMainDialog.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "MDObjectTreeItem.h"

#include "MDAccountUIImp.h"
#include "MDStringResource.h"
#include "PropertyBrowser.h"

CMDAccountTreeWindow::CMDAccountTreeWindow(QWidget *parent)
{
    m_UIImp = NULL;
    m_batchNewProperty = NULL;
}

CMDAccountTreeWindow::~CMDAccountTreeWindow()
{
    if(m_batchNewProperty)
        delete m_batchNewProperty;
}

/*!
\section 初始化
*/
void CMDAccountTreeWindow::init(CMDAccountUIImp* UI, CMDAccountConfiger *configer)
{
    m_UIImp = UI;
    m_configer = configer;
    CMDObjectTreeWindow::init(UI, configer);

    m_icon =QIcon(":/images/device.png");
    setWindowTitle(STRANS(STRING_ACCOUNT_TEXT));

    m_AccountForBrowser.setConfiger(m_UIImp->m_studioInterface->projectConfiger()->securityConfiger());
    m_AccountForBrowser.setObjTreeWindow(this);
}

/*!
\section 复制与粘贴
*/
QString CMDAccountTreeWindow::createName(const QString& sourceName)
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
bool CMDAccountTreeWindow::save()
{
    if(m_configer)
        return m_configer->saveToFile();
    return false;
}

void CMDAccountTreeWindow::buildObjectTree(CMDObjectTreeItem* rootItem)
{
    if(!m_configer)
        return;

    QList<CMDAccountProperty_Config*> objectList;
    m_configer->configedList(objectList);

    // 原始的动作 ---》通用的 treeitem，rootItem 管理全部 treeitem
    // --------------------------------------------------------------------------------
    QList<CMDAccountProperty_Config*>::iterator iObject;
    for (iObject = objectList.begin(); iObject != objectList.end(); ++iObject)
    {
        CMDAccountProperty_Config* configProperty = *iObject;
        CMDObjectTreeItem* objectItem = new CMDObjectTreeItem((qint16)MDObjectType::account, configProperty, rootItem);
        rootItem->appendChild(objectItem);
    }
}

bool CMDAccountTreeWindow::addObject(void* objectUI, void *objectConfiger, qint16 subType)
{
    bool result = false;
    CMDAccountMainDialog dlg(this);
    CMDAccountUIImp* uiImp = (CMDAccountUIImp*)objectUI;
    dlg.initial(this, uiImp->m_studioInterface->projectConfiger()->securityConfiger(), subType);
    dlg.exec();
    result = (QDialog::Accepted == dlg.result());
    return result;
}

bool CMDAccountTreeWindow::configObject(void* objectUI, void *objectConfiger, QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    bool result = false;
    if(1 == selectedTreeItems.size())
    {
        CMDObjectTreeItem* item = (CMDObjectTreeItem*)selectedTreeItems.first();
        if(item->type() == (qint16)MDObjectType::account)
        {
            CMDAccountMainDialog dlg(this);
            CMDAccountUIImp* uiImp = (CMDAccountUIImp*)objectUI;
            dlg.initial(this, uiImp->m_studioInterface->projectConfiger()->securityConfiger(), item->ptr());
            dlg.setWindowTitle(STRANS(STRING_ACCOUNT_TEXT) + "- " + objectName(item->ptr()));
            dlg.exec();
            result = (QDialog::Accepted == dlg.result());
        }
    }
    else
    {
        QList<CMDAccountProperty_Config*> MDObjects;
        foreach (CMDObjectTreeItem* item, selectedTreeItems)
            MDObjects.append((CMDAccountProperty_Config*)item->ptr());
        m_AccountForBrowser.setMDObjects(MDObjects);

        QObject* objForBatchConfig = &m_AccountForBrowser;
        QString title = QObject::tr("批量编辑 - 帐户");
        result = (QDialog::Accepted == showBatchConfigDialog(objForBatchConfig, title));
    }
    return result;
}

bool CMDAccountTreeWindow::configObjectInBrowser(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    bool result = false;
    if(selectedTreeItem->type() == (qint16)MDObjectType::account)
    {
        CMDAccountUIImp* uiImp = (CMDAccountUIImp*)objectUI;
        if(uiImp)
        {
            m_AccountForBrowser.setMDObject((CMDAccountProperty_Config*)selectedTreeItem->ptr());
            CPropertyBrowser* browser = uiImp->m_studioInterface->propertyBrowser();
            browser->setObject(&m_AccountForBrowser);
            browser->updateProperty();
        }
    }
    return result;
}

bool CMDAccountTreeWindow::deleteObject(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    QString str;
    if(1 == selectedTreeItems.size())
    {
        QString name = objectName(selectedTreeItems.first()->ptr());
        str = QString(STRANS(STRING_CONFIRM_DELETE_ACCOUNT) + ": %1 ?").arg(name);
    }
    else
        str = QString(STRANS(STRING_CONFIRM_DELETE_MULTI_ACCOUNT)).arg(selectedTreeItems.size());

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

void CMDAccountTreeWindow::objectDeleted(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    if(!m_UIImp)
        return;

    if(selectedTreeItems.size() <= 0)
        return;

    m_AccountForBrowser.setMDObject((CMDAccountProperty_Config*)selectedTreeItems.first()->ptr());
    CPropertyBrowser* browser = m_UIImp->m_studioInterface->propertyBrowser();
    browser->setObject(&m_AccountForBrowser);
    browser->updateProperty();
}

bool CMDAccountTreeWindow::pasteObject(void* objectPtr)
{
    CMDAccountProperty_Config* source = (CMDAccountProperty_Config*)objectPtr;

    QString destName = createName(source->name());
    CMDAccountProperty_Config dest(*source);
    dest.setName(destName);
    return m_configer->add(&dest);
}

QString CMDAccountTreeWindow::objectName(void* objectPtr)
{
    CMDAccountProperty_Config* cfg = (CMDAccountProperty_Config*)objectPtr;
    return (cfg ? cfg->name() : "");
}

bool CMDAccountTreeWindow::resultOnSetData(qint16 objectType)
{
    bool result = false;
    if(objectType == (qint16)MDObjectType::account)
    {
        result = true;
    }
    return result;
}

QVariant CMDAccountTreeWindow::objectData(bool enableChecked, CMDObjectTreeItem *item, int column, int role) const
{
    if(item->type() == (qint16)MDObjectType::account)
    {
        if(role==Qt::DecorationRole)
            return (column==0 ? m_icon : QVariant());
        else
        {
            CMDAccountProperty_Config* prop  = static_cast<CMDAccountProperty_Config*>(item->ptr());
            return accountData(enableChecked, prop, column);
        }
    }
    else return QVariant();
}

/*!
\section 批量增加
*/
bool CMDAccountTreeWindow::batchAddObject(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    bool result = false;
    if(selectedTreeItem->type() == (qint16)MDObjectType::account)
    {
        CMDAccountProperty_Config* sourceProperty = (CMDAccountProperty_Config*)selectedTreeItem->ptr();
        if(m_batchNewProperty)
            delete m_batchNewProperty;
        m_batchNewProperty = new CMDAccountProperty_Config(*sourceProperty);
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

bool CMDAccountTreeWindow::onBatchAddOneObject(const QJsonArray &ja, QString& resultMsg)
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
QVariant CMDAccountTreeWindow::accountData(bool enableChecked, CMDAccountProperty_Config* prop, int propIndex) const
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

bool CMDAccountTreeWindow::isTopObject(qint16 objectType)
{
    return (objectType == (qint16)MDObjectType::account);
}

void CMDAccountTreeWindow::initTreeColumnWidth(bool enableChecked, QTreeView *treeView)
{
    if(enableChecked)
    {
        treeView->setColumnWidth(0, 160);
        treeView->setColumnWidth(1, 100);
    }
    else
    {
        treeView->setColumnWidth(0, 160);
        treeView->setColumnWidth(2, 140);
        treeView->setColumnWidth(3, 100);
    }
}

void CMDAccountTreeWindow::initHeader(bool enableChecked, QStringList& headerList)
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

void CMDAccountTreeWindow::onMenuItemVisible(CMDObjectTreeItem *selectedTreeItem)
{
    bool visible = false;
    if (!selectedTreeItem)
        visible = false;
    else
        visible = (selectedTreeItem->type() == (qint16)MDObjectType::account);

    setMenuItemVisible(visible);
}

bool CMDAccountTreeWindow::initAddAction()
{
    return CMDObjectTreeWindow::initAddAction();
}

void CMDAccountTreeWindow::initSubMenuOfAdd(QMenu* subMenu)
{

}

/*!
\section 多语言
*/
void CMDAccountTreeWindow::changeLanguage()
{
    setWindowTitle(STRANS(STRING_ACCOUNT_TEXT));

    CMDObjectTreeWindow::changeLanguage();
}
