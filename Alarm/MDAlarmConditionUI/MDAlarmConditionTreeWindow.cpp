#include "MDAlarmConditionTreeWindow.h"

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
#include "MDAlarmConditionConfiger.h"
#include "MDAlarmCondition.h"

#include "MDAlarmConditionMainDialog.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "MDObjectTreeItem.h"

#include "MDAlarmConditionUI.h"
#include "MDAlarmConditionUIImp.h"
#include "MDStringResource.h"

#include "PropertyBrowser.h"

CMDAlarmConditionTreeWindow::CMDAlarmConditionTreeWindow(QWidget *parent)
{
    m_configer = NULL;
    m_conditionUI = NULL;
    m_batchNewProperty = NULL;
}

CMDAlarmConditionTreeWindow::~CMDAlarmConditionTreeWindow()
{
    if(m_batchNewProperty)
        delete m_batchNewProperty;
}

/*!
\section 初始化
*/
void CMDAlarmConditionTreeWindow::init(CMDAlarmConditionUIImp* UI, CMDAlarmConditionConfiger *configer)
{
    m_conditionUI = UI;
    m_configer = configer;
    CMDObjectTreeWindow::init(UI, configer);

    m_icon =QIcon(":/images/device.png");
    setWindowTitle(STRANS(STRING_COND_TEXT));

    m_AlarmConditionForBrowser.setConfiger(configer);
    m_AlarmConditionForBrowser.setObjTreeWindow(this);
}

bool CMDAlarmConditionTreeWindow::selectAlarmLevel(const QString& source, QString& dest)
{
    CMDAlarmConditionUIContacter* contacter = (m_conditionUI ? m_conditionUI->contacter() : NULL);
    return (contacter ? contacter->onSelectAlarmLevel(source, dest) : false);
}

bool CMDAlarmConditionTreeWindow::selectVariable(const QString& source, QString& dest)
{
    CMDAlarmConditionUIContacter* contacter = (m_conditionUI ? m_conditionUI->contacter() : NULL);
    return (contacter ? contacter->onSelectVariable(source, dest) : false);
}

/*!
\section 复制与粘贴
*/
QString CMDAlarmConditionTreeWindow::createName(const QString& sourceName)
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
bool CMDAlarmConditionTreeWindow::save()
{
    if(m_configer)
        return m_configer->saveToFile();
    return false;
}

void CMDAlarmConditionTreeWindow::buildObjectTree(CMDObjectTreeItem* rootItem)
{
    if(!m_configer)
        return;

    QList<CMDAlarmCondition*> objectList;
    m_configer->configedList(objectList);

    // 原始的动作 ---》通用的 treeitem，rootItem 管理全部 treeitem
    // --------------------------------------------------------------------------------
    QList<CMDAlarmCondition*>::iterator iObject;
    for (iObject = objectList.begin(); iObject != objectList.end(); ++iObject)
    {
        CMDAlarmCondition* configProperty = *iObject;
        CMDObjectTreeItem* objectItem = new CMDObjectTreeItem((qint16)MDObjectType::alarmCondition, configProperty, rootItem);
        rootItem->appendChild(objectItem);
    }
}

bool CMDAlarmConditionTreeWindow::addObject(void* objectUI, void *objectConfiger, qint16 subType)
{
    bool result = false;
    CMDAlarmConditionMainDialog dlg(this);
    CMDAlarmConditionUIImp* uiImp = (CMDAlarmConditionUIImp*)objectUI;
    dlg.setPowerGroupConfiger(uiImp->m_studioInterface->projectConfiger()->powerGroupConfiger());
    dlg.setAlarmConditionUIImp(uiImp);
    dlg.initial(this, objectConfiger, subType);
    dlg.exec();
    result = (QDialog::Accepted == dlg.result());
    return result;
}

bool CMDAlarmConditionTreeWindow::configObject(void* objectUI, void *objectConfiger, QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    bool result = false;
    if(1 == selectedTreeItems.size())
    {
        CMDObjectTreeItem* item = (CMDObjectTreeItem*)selectedTreeItems.first();
        if(item->type() == (qint16)MDObjectType::alarmCondition)
        {
            CMDAlarmConditionMainDialog dlg(this);
            CMDAlarmConditionUIImp* uiImp = (CMDAlarmConditionUIImp*)objectUI;
            dlg.setPowerGroupConfiger(uiImp->m_studioInterface->projectConfiger()->powerGroupConfiger());
            dlg.setAlarmConditionUIImp(uiImp);
            dlg.initial(this, objectConfiger, item->ptr());
            dlg.setWindowTitle(STRANS(STRING_PROP_COND) +  " - " + objectName(item->ptr()));
            dlg.exec();
            result = (QDialog::Accepted == dlg.result());
        }
    }
    else
    {
        QList<CMDAlarmCondition*> MDObjects;
        foreach (CMDObjectTreeItem* item, selectedTreeItems)
            MDObjects.append((CMDAlarmCondition*)item->ptr());
        m_AlarmConditionForBrowser.setMDObjects(MDObjects);

        QObject* objForBatchConfig = &m_AlarmConditionForBrowser;
        QString title = QObject::tr("批量编辑 - 报警条件");
        result = (QDialog::Accepted == showBatchConfigDialog(objForBatchConfig, title));
    }
    return result;
}

bool CMDAlarmConditionTreeWindow::configObjectInBrowser(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    bool result = false;
    if(selectedTreeItem->type() == (qint16)MDObjectType::alarmCondition)
    {
        CMDAlarmConditionUIImp* uiImp = (CMDAlarmConditionUIImp*)objectUI;
        if(uiImp)
        {
            m_AlarmConditionForBrowser.setMDObject((CMDAlarmCondition*)selectedTreeItem->ptr());
            CPropertyBrowser* browser = uiImp->m_studioInterface->propertyBrowser();
            browser->setObject(&m_AlarmConditionForBrowser);
            browser->updateProperty();
        }
    }
    return result;
}

bool CMDAlarmConditionTreeWindow::deleteObject(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    QString str;
    if(1 == selectedTreeItems.size())
    {
        QString name = objectName(selectedTreeItems.first()->ptr());
        str = QString(STRANS(STRING_CONFIRM_DELETE_COND) + ": %1 ?").arg(name);
    }
    else
        str = QString(STRANS(STRING_CONFIRM_DELETE_MULTI_COND)).arg(selectedTreeItems.size());

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

void CMDAlarmConditionTreeWindow::objectDeleted(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    if(!m_conditionUI)
        return;

    if(selectedTreeItems.size() <= 0)
        return;

    m_AlarmConditionForBrowser.setMDObject((CMDAlarmCondition*)selectedTreeItems.first()->ptr());
    CPropertyBrowser* browser = m_conditionUI->m_studioInterface->propertyBrowser();
    browser->setObject(&m_AlarmConditionForBrowser);
    browser->updateProperty();
}

bool CMDAlarmConditionTreeWindow::pasteObject(void* objectPtr)
{
    CMDAlarmCondition* source = (CMDAlarmCondition*)objectPtr;

    QString destName = createName(source->name());
    CMDAlarmCondition dest(*source);
    dest.setName(destName);
    return m_configer->add(&dest);
}

QString CMDAlarmConditionTreeWindow::objectName(void* objectPtr)
{
    CMDAlarmCondition* cfg = (CMDAlarmCondition*)objectPtr;
    return (cfg ? cfg->name() : "");
}

bool CMDAlarmConditionTreeWindow::resultOnSetData(qint16 objectType)
{
    bool result = false;
    if(objectType == (qint16)MDObjectType::alarmCondition)
    {
        result = true;
    }
    return result;
}

QVariant CMDAlarmConditionTreeWindow::objectData(bool enableChecked, CMDObjectTreeItem *item, int column, int role) const
{
    if(item->type() == (qint16)MDObjectType::alarmCondition)
    {
        if(role==Qt::DecorationRole)
            return (column==0 ? m_icon : QVariant());
        else
        {
            CMDAlarmCondition* prop  = static_cast<CMDAlarmCondition*>(item->ptr());
            return conditionData(enableChecked, prop, column);
        }
    }
    else return QVariant();
}

/*!
\section 批量增加
*/
bool CMDAlarmConditionTreeWindow::batchAddObject(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    bool result = false;
    if(selectedTreeItem->type() == (qint16)MDObjectType::alarmCondition)
    {
        CMDAlarmCondition* sourceProperty = (CMDAlarmCondition*)selectedTreeItem->ptr();
        if(m_batchNewProperty)
            delete m_batchNewProperty;
        m_batchNewProperty = new CMDAlarmCondition(*sourceProperty);
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

bool CMDAlarmConditionTreeWindow::onBatchAddOneObject(const QJsonArray &ja, QString& resultMsg)
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
QVariant CMDAlarmConditionTreeWindow::conditionData(bool enableChecked, CMDAlarmCondition* prop, int propIndex) const
{
    if(enableChecked) // 用于选择的界面，可减少列数
    {
        if(propIndex<=0)
            return QVariant(prop->customId());
        else if(propIndex==1)
            return QVariant(prop->name());
        else if(propIndex==2)
            return QVariant(prop->comment());
        else
            return QVariant();
    }
    else
    {
        if(propIndex<=0)
            return QVariant(prop->customId());
        else if(propIndex==1)
            return QVariant(prop->name());
        else if(propIndex==2)
            return QVariant(prop->comment());
        else if(propIndex==3)
            return QVariant(prop->levelName());
        else if(propIndex==4)
            return QVariant(prop->serverity());
        else if(propIndex==5)
            return QVariant(prop->expression());
        else if(propIndex==6)
            return QVariant(prop->enabled());
        else if(propIndex==7)
            return QVariant(prop->enableAlarmExp());
        else if(propIndex==8)
            return QVariant(prop->acknowledgeRequired());
        else if(propIndex==9)
            return QVariant(prop->latched());
        else
            return QVariant();
    }
}

bool CMDAlarmConditionTreeWindow::isTopObject(qint16 objectType)
{
    return (objectType == (qint16)MDObjectType::alarmCondition);
}

void CMDAlarmConditionTreeWindow::initTreeColumnWidth(bool enableChecked, QTreeView *treeView)
{
    if(enableChecked)
    {
        treeView->setColumnWidth(0, 150);
        treeView->setColumnWidth(1, 200);
        treeView->setColumnWidth(2, 100);
    }
    else
    {
        treeView->setColumnWidth(0, 150);
        treeView->setColumnWidth(1, 200);
        treeView->setColumnWidth(2, 100);
        treeView->setColumnWidth(3, 100);
        treeView->setColumnWidth(4, 50);
        treeView->setColumnWidth(5, 200);
        treeView->setColumnWidth(6, 50);
        treeView->setColumnWidth(7, 200);
        treeView->setColumnWidth(8, 50);
        treeView->setColumnWidth(9, 50);
    }
}

void CMDAlarmConditionTreeWindow::initHeader(bool enableChecked, QStringList& headerList)
{
    // 多语言：此处作为原始串，翻译由 MDObjectManagerUI 完成
    if(enableChecked) // 用于选择的界面，可减少列数
    {
        headerList.append(STRING_ID);
        headerList.append(STRING_NAME);
        headerList.append(STRING_COMMENT);
    }
    else
    {
        headerList.append(STRING_ID);
        headerList.append(STRING_NAME);
        headerList.append(STRING_COMMENT);
        headerList.append(STRING_LEVEL);
        headerList.append(STRING_SEVERITY);
        headerList.append(STRING_PROP_COND);
        headerList.append(STRING_ENABLE);
        headerList.append(STRING_ENABLEEXP);
        headerList.append(STRING_ACK);
        headerList.append(STRING_CONFIRM);
    }
}

void CMDAlarmConditionTreeWindow::onMenuItemVisible(CMDObjectTreeItem *selectedTreeItem)
{
    bool visible = false;
    if (!selectedTreeItem)
        visible = false;
    else
        visible = (selectedTreeItem->type() == (qint16)MDObjectType::alarmCondition);

    setMenuItemVisible(visible);
}

bool CMDAlarmConditionTreeWindow::initAddAction()
{
    return CMDObjectTreeWindow::initAddAction();
}

void CMDAlarmConditionTreeWindow::initSubMenuOfAdd(QMenu* subMenu)
{

}
