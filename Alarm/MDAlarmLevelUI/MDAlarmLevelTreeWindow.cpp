#include "MDAlarmLevelTreeWindow.h"

#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QJsonArray>
#include <QJsonObject>

#include "MDProjectConfiger/MDProjectConfiger.h"
#include "MDAlarmConfiger.h"
#include "MDAlarmLevelConfiger.h"
#include "MDAlarmLevel.h"

#include "MDAlarmLevelMainDialog.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "MDObjectTreeItem.h"

#include "MDAlarmLevelUIImp.h"
#include "MDStringResource.h"
#include "PropertyBrowser.h"

CMDAlarmLevelTreeWindow::CMDAlarmLevelTreeWindow(QWidget *parent)
{
    m_UIImp = NULL;
    m_alarmConfiger = NULL;
    m_batchNewProperty = NULL;
}

CMDAlarmLevelTreeWindow::~CMDAlarmLevelTreeWindow()
{
    if(m_batchNewProperty)
        delete m_batchNewProperty;
}

/*!
\section 初始化
*/
void CMDAlarmLevelTreeWindow::init(CMDAlarmLevelUIImp* UI, CMDAlarmConfiger *configer)
{
    m_UIImp = UI;
    m_alarmConfiger = configer;
    CMDObjectTreeWindow::init(UI, configer);

    m_icon =QIcon(":/images/device.png");
    setWindowTitle(STRANS(STRING_LEVEL_TEXT));

    m_AlarmLevelForBrowser.setConfiger(configer);
    m_AlarmLevelForBrowser.setObjTreeWindow(this);
}

/*!
\section 复制与粘贴
*/
QString CMDAlarmLevelTreeWindow::createName(const QString& sourceName)
{
    int count = m_alarmConfiger->levelConfiger()->count();
    for(int i= 0; i < count; i++)
    {
      QString name = QString("%1_%2").arg(sourceName).arg(QString::number(i+1));
      if(!m_alarmConfiger->levelConfiger()->find(name))
          return name;
    }
    return QString("%1_%2").arg(sourceName).arg(QString::number(count+1));
}

/*!
\section 重载的接口
*/
bool CMDAlarmLevelTreeWindow::save()
{
    if(m_alarmConfiger)
        return m_alarmConfiger->levelConfiger()->saveToFile();
    return false;
}

void CMDAlarmLevelTreeWindow::buildObjectTree(CMDObjectTreeItem* rootItem)
{
    if(!m_alarmConfiger)
        return;

    QList<CMDAlarmLevel*> objectList;
    m_alarmConfiger->levelConfiger()->configedList(objectList);

    // 原始的动作 ---》通用的 treeitem，rootItem 管理全部 treeitem
    // --------------------------------------------------------------------------------
    QList<CMDAlarmLevel*>::iterator iObject;
    for (iObject = objectList.begin(); iObject != objectList.end(); ++iObject)
    {
        CMDAlarmLevel* configProperty = *iObject;
        CMDObjectTreeItem* objectItem = new CMDObjectTreeItem((qint16)MDObjectType::alarmLevel, configProperty, rootItem);
        rootItem->appendChild(objectItem);
    }
}

bool CMDAlarmLevelTreeWindow::addObject(void* objectUI, void *objectConfiger, qint16 subType)
{
    bool result = false;
    CMDAlarmLevelMainDialog dlg(this);
    CMDAlarmLevelUIImp* uiImp = (CMDAlarmLevelUIImp*)objectUI;
    dlg.initial(this, uiImp->m_studioInterface->projectConfiger()->alarmConfiger(), subType);
    CMDMultiLanguage::instance()->translateWidget(&dlg);
    dlg.exec();
    result = (QDialog::Accepted == dlg.result());
    return result;
}

bool CMDAlarmLevelTreeWindow::configObject(void* objectUI, void *objectConfiger, QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    bool result = false;
    if(1 == selectedTreeItems.size())
    {
        CMDObjectTreeItem* item = (CMDObjectTreeItem*)selectedTreeItems.first();
        if(item->type() == (qint16)MDObjectType::alarmLevel)
        {
            CMDAlarmLevelMainDialog dlg(this);
            CMDAlarmLevelUIImp* uiImp = (CMDAlarmLevelUIImp*)objectUI;
            dlg.initial(this, uiImp->m_studioInterface->projectConfiger()->alarmConfiger(), item->ptr());
            dlg.setWindowTitle(STRANS(STRING_LEVEL_TEXT) + " - " + objectName(item->ptr()));
            dlg.exec();
            result = (QDialog::Accepted == dlg.result());
        }
    }
    else
    {
        QList<CMDAlarmLevel*> MDObjects;
        foreach (CMDObjectTreeItem* item, selectedTreeItems)
            MDObjects.append((CMDAlarmLevel*)item->ptr());
        m_AlarmLevelForBrowser.setMDObjects(MDObjects);

        QObject* objForBatchConfig = &m_AlarmLevelForBrowser;
        QString title = QObject::tr("批量编辑 - 报警等级");
        result = (QDialog::Accepted == showBatchConfigDialog(objForBatchConfig, title));
    }
    return result;
}

bool CMDAlarmLevelTreeWindow::configObjectInBrowser(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    bool result = false;
    if(selectedTreeItem->type() == (qint16)MDObjectType::alarmLevel)
    {
        CMDAlarmLevelUIImp* uiImp = (CMDAlarmLevelUIImp*)objectUI;
        if(uiImp)
        {
            m_AlarmLevelForBrowser.setMDObject((CMDAlarmLevel*)selectedTreeItem->ptr());
            CPropertyBrowser* browser = uiImp->m_studioInterface->propertyBrowser();
            browser->setObject(&m_AlarmLevelForBrowser);
            browser->updateProperty();
        }
    }
    return result;
}

bool CMDAlarmLevelTreeWindow::deleteObject(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    QString str;
    if(1 == selectedTreeItems.size())
    {
        QString name = objectName(selectedTreeItems.first()->ptr());
        str = QString(STRANS(STRING_CONFIRM_DELETE) + ": %1 ?").arg(name);
    }
    else
        str = QString(STRANS(STRING_CONFIRM_DELETE_MULTI)).arg(selectedTreeItems.size());

    QMessageBox::StandardButton reply = QMessageBox::question(this, STRANS(STRING_OK), str, QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No)
        return false;

    foreach (CMDObjectTreeItem* item, selectedTreeItems)
    {
        QString name = objectName(item->ptr());
        m_alarmConfiger->levelConfiger()->delete1(name);
    }
    return true;
}

void CMDAlarmLevelTreeWindow::objectDeleted(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    if(!m_UIImp)
        return;

    if(selectedTreeItems.size() <= 0)
        return;

    m_AlarmLevelForBrowser.setMDObject((CMDAlarmLevel*)selectedTreeItems.first()->ptr());
    CPropertyBrowser* browser = m_UIImp->m_studioInterface->propertyBrowser();
    browser->setObject(&m_AlarmLevelForBrowser);
    browser->updateProperty();
}

bool CMDAlarmLevelTreeWindow::pasteObject(void* objectPtr)
{
    CMDAlarmLevel* source = (CMDAlarmLevel*)objectPtr;

    QString destName = createName(source->name());
    CMDAlarmLevel dest(*source);
    dest.setName(destName);
    return m_alarmConfiger->levelConfiger()->add(&dest);
}

QString CMDAlarmLevelTreeWindow::objectName(void* objectPtr)
{
    CMDAlarmLevel* cfg = (CMDAlarmLevel*)objectPtr;
    return (cfg ? cfg->name() : "");
}

bool CMDAlarmLevelTreeWindow::resultOnSetData(qint16 objectType)
{
    bool result = false;
    if(objectType == (qint16)MDObjectType::alarmLevel)
    {
        result = true;
    }
    return result;
}

QVariant CMDAlarmLevelTreeWindow::objectData(bool enableChecked, CMDObjectTreeItem *item, int column, int role) const
{
    if(item->type() == (qint16)MDObjectType::alarmLevel)
    {
        if(role==Qt::DecorationRole)
            return (column==0 ? m_icon : QVariant());
        else
        {
            CMDAlarmLevel* prop  = static_cast<CMDAlarmLevel*>(item->ptr());
            return levelData(enableChecked, prop, column);
        }
    }
    else return QVariant();
}

/*!
\section 批量增加
*/
bool CMDAlarmLevelTreeWindow::batchAddObject(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    bool result = false;
    if(selectedTreeItem->type() == (qint16)MDObjectType::alarmLevel)
    {
        CMDAlarmLevel* sourceProperty = (CMDAlarmLevel*)selectedTreeItem->ptr();
        if(m_batchNewProperty)
            delete m_batchNewProperty;
        m_batchNewProperty = new CMDAlarmLevel(*sourceProperty);
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

bool CMDAlarmLevelTreeWindow::onBatchAddOneObject(const QJsonArray &ja, QString& resultMsg)
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

    bool result = m_alarmConfiger->levelConfiger()->add(m_batchNewProperty);
    if(!result)
        resultMsg = m_batchNewProperty->name();
    return result;
}

// 实际的列，参考 initHeader()
QVariant CMDAlarmLevelTreeWindow::levelData(bool enableChecked, CMDAlarmLevel* prop, int propIndex) const
{
    if(enableChecked) // 用于选择的界面，可减少列数
    {
        if(propIndex<=0)
            return QVariant(prop->name());
        else if(propIndex==1)
            return QVariant(CMDMultiLanguage::instance()->value(prop->comment(), false));
        else
            return QVariant();
    }
    else
    {
        if(propIndex<=0)
            return QVariant(prop->name());
        else if(propIndex==1)
            return QVariant(CMDMultiLanguage::instance()->value(prop->comment(), false));
        else
            return QVariant();
    }
}

bool CMDAlarmLevelTreeWindow::isTopObject(qint16 objectType)
{
    return (objectType == (qint16)MDObjectType::alarmLevel);
}

void CMDAlarmLevelTreeWindow::initTreeColumnWidth(bool enableChecked, QTreeView *treeView)
{
    if(enableChecked)
    {
        treeView->setColumnWidth(0, 200);
        treeView->setColumnWidth(1, 100);
    }
    else
    {
        treeView->setColumnWidth(0, 200);
        treeView->setColumnWidth(1, 100);
    }
}

void CMDAlarmLevelTreeWindow::initHeader(bool enableChecked, QStringList& headerList)
{
    // 多语言：此处作为原始串，翻译由 MDObjectManagerUI 完成
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

void CMDAlarmLevelTreeWindow::onMenuItemVisible(CMDObjectTreeItem *selectedTreeItem)
{
    bool visible = false;
    if (!selectedTreeItem)
        visible = false;
    else
        visible = (selectedTreeItem->type() == (qint16)MDObjectType::alarmLevel);

    setMenuItemVisible(visible);
}

bool CMDAlarmLevelTreeWindow::initAddAction()
{
    return CMDObjectTreeWindow::initAddAction();
}

void CMDAlarmLevelTreeWindow::initSubMenuOfAdd(QMenu* subMenu)
{

}

/*!
\section 多语言
*/
void CMDAlarmLevelTreeWindow::changeLanguage()
{
    setWindowTitle(STRANS(STRING_LEVEL_TEXT));

    CMDObjectTreeWindow::changeLanguage();
}
