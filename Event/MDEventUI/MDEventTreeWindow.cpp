#include "MDEventTreeWindow.h"

#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonArray>

#include "MDEventMainDialog.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "MDObjectTreeItem.h"

#include "MDEventUI.h"
#include "MDEventUIImp.h"
#include "EPC.h"
#include "MDStringResource.h"
#include "PropertyBrowser.h"

CMDEventTreeWindow::CMDEventTreeWindow(QWidget *parent)
{
    m_eventConfiger = NULL;
    m_eventUI = NULL;
    m_batchNewProperty = NULL;
}
CMDEventTreeWindow::~CMDEventTreeWindow()
{
    if(m_batchNewProperty)
        delete m_batchNewProperty;
}

/*!
\section 初始化与关闭
*/
void CMDEventTreeWindow::init(CMDEventUIImp* eventUI, CMDEventConfiger *eventConfiger)
{
    m_eventUI = eventUI;
    m_eventConfiger = eventConfiger;

    CMDObjectTreeWindow::init(eventUI, eventConfiger);

    m_eventIcon =QIcon(":/images/device.png");
    m_actionIcon = QIcon(":/images/datablock.png");

    setWindowTitle(STRANS(STRING_EVENT_TEXT));

    m_EventProperty_ConfigForBrowser.setConfiger(eventConfiger);
    m_EventProperty_ConfigForBrowser.setObjTreeWindow(this);

    m_TimeCycleForBrowser.setConfiger(eventConfiger);
    m_TimeCycleForBrowser.setObjTreeWindow(this);

    m_VariableQualityChangedForBrowser.setConfiger(eventConfiger);
    m_VariableQualityChangedForBrowser.setObjTreeWindow(this);

    m_VariableValueChangedInRangeForBrowser.setConfiger(eventConfiger);
    m_VariableValueChangedInRangeForBrowser.setObjTreeWindow(this);

    m_VariableValueWriteForBrowser.setConfiger(eventConfiger);
    m_VariableValueWriteForBrowser.setObjTreeWindow(this);
}

bool CMDEventTreeWindow::selectActions(const QStringList& sourceList, QStringList& destList)
{
    CMDEventUIContacter* contacter = (m_eventUI ? m_eventUI->contacter() : NULL);
    return (contacter ? contacter->onSelectActions(sourceList, destList) : false);
}

bool CMDEventTreeWindow::selectSingleObject(MDEventType type, const QString& source, QString& dest)
{
    CMDEventUIContacter* contacter = (m_eventUI ? m_eventUI->contacter() : NULL);
    return (contacter ? contacter->onSelectSingleObject(type, source, dest) : false);
}

void CMDEventTreeWindow::closeEvent(QCloseEvent *e)
{
    CBaseForm::closeEvent(e);
}

/*!
\section 复制与粘贴
*/
QString CMDEventTreeWindow::createName(const QString& sourceName)
{
    int count = m_eventConfiger->count();
    for(int i= 0; i < count; i++)
    {
      QString name = QString("%1_%2").arg(sourceName).arg(QString::number(i+1));
      if(!m_eventConfiger->findEvent(name))
          return name;
    }
    return QString("%1_%2").arg(sourceName).arg(QString::number(count+1));
}

/*!
\section 重载的接口
*/
bool CMDEventTreeWindow::save()
{
    if(m_eventConfiger)
        return m_eventConfiger->saveToFile();
    return false;
}

void CMDEventTreeWindow::buildObjectTree(CMDObjectTreeItem* rootItem)
{
    if(!m_eventConfiger)
        return;

    // 获取原始的事件配置
    // -----------------------------------------------------
    QList<CMDEventProperty_Config*> eventList;
    m_eventConfiger->configedEventList(eventList);

    // 原始的事件 ---》通用的 treeitem，rootItem 管理全部 treeitem
    // --------------------------------------------------------------------------------
    QList<CMDEventProperty_Config*>::iterator iEvent;
    for (iEvent = eventList.begin(); iEvent != eventList.end(); ++iEvent)
    {
        CMDEventProperty_Config* eventProperty = *iEvent;
        CMDObjectTreeItem* eventItem = new CMDObjectTreeItem((qint16)MDObjectType::event, eventProperty, rootItem);

        const QStringList& actionList = eventProperty->actionNameList();
        QList<QString>::const_iterator iAction;
        for (iAction = actionList.constBegin(); iAction != actionList.constEnd(); ++iAction)
        {
            CMDObjectTreeItem* actionItem = new CMDObjectTreeItem((qint16)MDObjectType::action, (void*)&(*iAction), eventItem);
            eventItem->appendChild(actionItem);
        }

        rootItem->appendChild(eventItem);
    }
}

bool CMDEventTreeWindow::addObject(void* objectUI, void *objectConfiger, qint16 subType)
{
    bool result = false;
    CMDEventMainDialog dlg(this);
    dlg.initial(this, objectConfiger, subType);
    dlg.exec();
    result = (QDialog::Accepted == dlg.result());
    return result;
}

bool CMDEventTreeWindow::configObject(void* objectUI, void *objectConfiger, QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    bool result = false;
    if(1 == selectedTreeItems.size())
    {
        CMDObjectTreeItem* item = (CMDObjectTreeItem*)selectedTreeItems.first();
        if(item->type() == (qint16)MDObjectType::event)
        {
            CMDEventMainDialog dlg(this);
            dlg.initial(this, objectConfiger, item->ptr());
            dlg.setWindowTitle(STRANS(STRING_EVENT_TEXT) +  " - " + objectName(item->ptr()));
            dlg.exec();
            result = (QDialog::Accepted == dlg.result());
        }
    }
    else
    {
        QList<CMDEventProperty_Config*> MDObjects;
        foreach (CMDObjectTreeItem* item, selectedTreeItems)
            MDObjects.append((CMDEventProperty_Config*)item->ptr());

        QObject* objForBatchConfig = getMDObjectForBactch(MDObjects);
        QString title = QObject::tr("批量编辑 - 事件");
        result = (QDialog::Accepted == showBatchConfigDialog(objForBatchConfig, title));
    }
    return result;
}

bool CMDEventTreeWindow::configObjectInBrowser(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    bool result = false;
    if(selectedTreeItem->type() == (qint16)MDObjectType::event)
    {
        CMDEventUIImp* uiImp = (CMDEventUIImp*)objectUI;
        if(uiImp)
        {
            CMDEventProperty_Config* baseProp = ((CMDEventProperty_Config*)selectedTreeItem->ptr());
            CPropertyBrowser* browser = uiImp->m_studioInterface->propertyBrowser();
            browser->setObject(getMDObject(baseProp));
            browser->updateProperty();
        }
    }
    return result;
}

QObject* CMDEventTreeWindow::getMDObjectForBactch(QList<CMDEventProperty_Config*>& MDObjects)
{
    if(MDObjects.size() <= 0)
        return &m_EventProperty_ConfigForBrowser;

    bool sameType = true;
    MDEventType theType = MDObjects.first()->type();
    foreach (CMDEventProperty_Config* ep, MDObjects)
    {
        if(theType != ep->type())
        {
            sameType = false;
            break;
        }
    }

    CMDEventProperty_ConfigForBrowser* EPForBrowser = (sameType ?
                                                            (CMDEventProperty_ConfigForBrowser*)getMDObject(MDObjects.first()) :
                                                            &m_EventProperty_ConfigForBrowser);
    EPForBrowser->setMDObjects(MDObjects);
    return EPForBrowser;
}

QObject* CMDEventTreeWindow::getMDObject(CMDEventProperty_Config* baseProp)
{
    QObject* theObject = NULL;

    m_EventProperty_ConfigForBrowser.setMDObject(baseProp);
    switch (baseProp->type())
    {
    case MDEventType::variable_ValueChangedInRange:
        m_VariableValueChangedInRangeForBrowser.setMDObject((CEPC_VariableValueChangedInRange*)baseProp);
        theObject = &m_VariableValueChangedInRangeForBrowser;
    break;

    case MDEventType::variable_ValueChanged:
        theObject = &m_EventProperty_ConfigForBrowser;
    break;

    case MDEventType::variable_QualityChanged:
        m_VariableQualityChangedForBrowser.setMDObject((CEPC_VariableQualityChanged*)baseProp);
        theObject = &m_VariableQualityChangedForBrowser;
    break;

    case MDEventType::variable_ValueWrite:
        m_VariableValueWriteForBrowser.setMDObject((CEPC_VariableValueWrite*)baseProp);
        theObject = &m_VariableValueWriteForBrowser;
    break;

    case MDEventType::timer_Cycle:
        m_TimeCycleForBrowser.setMDObject((CEPC_TimeCycle*)baseProp);
        theObject = &m_TimeCycleForBrowser;
    break;

    case MDEventType::variable_ValueRead:
        theObject = &m_EventProperty_ConfigForBrowser;
    break;

    case MDEventType::combined:
        theObject = &m_EventProperty_ConfigForBrowser;
    break;
    default:
        theObject = &m_EventProperty_ConfigForBrowser;
        break;
    }
    return theObject;
}

bool CMDEventTreeWindow::deleteObject(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    QString str;
    if(1 == selectedTreeItems.size())
    {
        QString name = objectName(selectedTreeItems.first()->ptr());
        str = QString(STRANS(STRING_CONFIRM_DELETE_EVENT) + ": %1 ?").arg(name);
    }
    else
        str = QString(STRANS(STRING_CONFIRM_DELETE_MULTI_EVENT)).arg(selectedTreeItems.size());

    QMessageBox::StandardButton reply = QMessageBox::question(this, STRANS(STRING_OK), str, QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No)
        return false;

    foreach (CMDObjectTreeItem* item, selectedTreeItems)
    {
        QString name = objectName(item->ptr());
        m_eventConfiger->deleteEvent(name);
    }
    return true;
}

void CMDEventTreeWindow::objectDeleted(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    if(!m_eventUI)
        return;

    if(selectedTreeItems.size() <= 0)
        return;

    CMDEventProperty_Config* baseProp = ((CMDEventProperty_Config*)selectedTreeItems.first()->ptr());
    CPropertyBrowser* browser = m_eventUI->m_studioInterface->propertyBrowser();
    browser->setObject(getMDObject(baseProp));
    browser->updateProperty();
}

bool CMDEventTreeWindow::pasteObject(void* objectPtr)
{
    CMDEventProperty_Config* source = (CMDEventProperty_Config*)objectPtr;
    CMDEventProperty_Config* dest = CEPC_Factory::newConfig(source->type(), source);
    if(!dest)
        return false;

    QString destName = createName(source->name());
    dest->setName(destName);
    bool result = m_eventConfiger->addEvent(dest);
    delete dest;
    return result;
}

QString CMDEventTreeWindow::objectName(void* objectPtr)
{
    CMDEventProperty_Config* cfg = (CMDEventProperty_Config*)objectPtr;
    return (cfg ? cfg->name() : "");
}

bool CMDEventTreeWindow::resultOnSetData(qint16 objectType)
{
    bool result = false;
    if(objectType == (qint16)MDObjectType::event ||
            objectType == (qint16)MDObjectType::action)
    {
        result = true;
    }

    return result;
}

QVariant CMDEventTreeWindow::objectData(bool enableChecked, CMDObjectTreeItem *item, int column, int role) const
{
    if(item->type() == (qint16)MDObjectType::event)
    {
        if(role==Qt::DecorationRole)
            return (column == 0 ? m_eventIcon : QVariant());
        else
        {
            CMDEventProperty_Config* eventProperty  = static_cast<CMDEventProperty_Config*>(item->ptr());
            return eventData(enableChecked, eventProperty, column);
        }
    }
    else if(item->type() == (qint16)MDObjectType::action)
    {
        if(role==Qt::DecorationRole)
            return (column == 0 ? m_actionIcon : QVariant());
        else
        {
            QString* actionName  = static_cast<QString*>(item->ptr());
            return actionData(*actionName, column);
        }
    }
    else return QVariant();
}

/*!
\section 批量增加
*/
bool CMDEventTreeWindow::batchAddObject(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    bool result = false;
    if(selectedTreeItem->type() == (qint16)MDObjectType::event)
    {
        CMDEventProperty_Config* sourceProperty = (CMDEventProperty_Config*)selectedTreeItem->ptr();
        if(m_batchNewProperty)
            delete m_batchNewProperty;
        m_batchNewProperty = CEPC_Factory::newConfig(sourceProperty->type(), sourceProperty);
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

bool CMDEventTreeWindow::onBatchAddOneObject(const QJsonArray &ja, QString& resultMsg)
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

    bool result = m_eventConfiger->addEvent(m_batchNewProperty);
    if(!result)
        resultMsg = m_batchNewProperty->name();
    return result;
}

// 实际的列，参考 initHeader()
QVariant CMDEventTreeWindow::eventData(bool enableChecked, CMDEventProperty_Config* eventProperty, int propIndex) const
{
    if(enableChecked)
    {
        if(propIndex<=0)
            return QVariant(eventProperty->name());
        else if(propIndex==1)
            return QVariant(eventProperty->objName());
        else
            return QVariant();
    }
    else
    {
        if(propIndex<=0)
            return QVariant(eventProperty->name());
        else if(propIndex==1)
            return QVariant(PTRANS(eventProperty->comment()));
        else if(propIndex==2)
              return QVariant(STRANS(CMDEventProperty_Config::typeToString(eventProperty->type())));
        else if(propIndex==3)
            return QVariant(eventProperty->objName());
        else if(propIndex==4)
            return QVariant(eventProperty->condition());
        else
            return QVariant();
    }
}

// 事件所触发的动作
QVariant CMDEventTreeWindow::actionData(const QString& actionName, int propIndex) const
{
    if(propIndex<=0)
        return QVariant(actionName);
    else
        return QVariant();
}

bool CMDEventTreeWindow::isTopObject(qint16 objectType)
{
    return (objectType == (qint16)MDObjectType::event);
}

void CMDEventTreeWindow::initTreeColumnWidth(bool enableChecked, QTreeView *treeView)
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
        treeView->setColumnWidth(2, 140);
        treeView->setColumnWidth(3, 100);
        treeView->setColumnWidth(4, 100);
    }
}

void CMDEventTreeWindow::initHeader(bool enableChecked, QStringList& headerList)
{
    // 多语言：此处作为原始串，翻译由 MDObjectManagerUI 完成
    if(enableChecked) // 用于选择的界面，可减少列数
    {
        headerList.append(STRING_NAME);
        headerList.append(STRING_OBJECT);
    }
    else
    {
        headerList.append(STRING_NAME);
        headerList.append(STRING_COMMENT);
        headerList.append(STRING_TYPE);
        headerList.append(STRING_OBJECT);
        headerList.append(STRING_PROP_COND);
    }
}

void CMDEventTreeWindow::onMenuItemVisible(CMDObjectTreeItem *selectedTreeItem)
{
    bool visible = false;
    if (!selectedTreeItem)
        visible = false;
    else
        visible = (selectedTreeItem->type() == (qint16)MDObjectType::event);

    setMenuItemVisible(visible);
}

bool CMDEventTreeWindow::initAddAction()
{
    return false;
}

void CMDEventTreeWindow::initSubMenuOfAdd(QMenu* subMenu)
{
    QAction* qAction = NULL;

    qAction = addAction(subMenu, CMDEventProperty_Config::typeToString(MDEventType::variable_ValueChanged));
    connect(qAction, &QAction::triggered, this, &CMDEventTreeWindow::doAdd_Variable_ValueChanged);

    qAction = addAction(subMenu, CMDEventProperty_Config::typeToString(MDEventType::variable_ValueChangedInRange));
    connect(qAction, &QAction::triggered, this, &CMDEventTreeWindow::doAdd_Variable_ValueChangedInRang);

    qAction = addAction(subMenu, CMDEventProperty_Config::typeToString(MDEventType::variable_QualityChanged));
    connect(qAction, &QAction::triggered, this, &CMDEventTreeWindow::doAdd_Variable_QualityChanged);

    qAction = addAction(subMenu, CMDEventProperty_Config::typeToString(MDEventType::timer_Cycle));
    connect(qAction, &QAction::triggered, this, &CMDEventTreeWindow::doAdd_Timer_Cycle);

    qAction = addAction(subMenu, CMDEventProperty_Config::typeToString(MDEventType::combined));
    connect(qAction, &QAction::triggered, this, &CMDEventTreeWindow::doAdd_Combined);
}

/*!
\section 子菜单：新建具体的事件（变量数值变化、周期 ...）
*/
void CMDEventTreeWindow::doAdd_Variable_ValueChanged()
{
    doAdd((qint16)MDEventType::variable_ValueChanged);
}

void CMDEventTreeWindow::doAdd_Variable_ValueChangedInRang()
{
    doAdd((qint16)MDEventType::variable_ValueChangedInRange);
}

void CMDEventTreeWindow::doAdd_Variable_QualityChanged()
{
    doAdd((qint16)MDEventType::variable_QualityChanged);
}

void CMDEventTreeWindow::doAdd_Timer_Cycle()
{
    doAdd((qint16)MDEventType::timer_Cycle);
}

void CMDEventTreeWindow::doAdd_Combined()
{
    doAdd((qint16)MDEventType::combined);
}

/*!
\section 多语言
*/
void CMDEventTreeWindow::changeLanguage()
{
    setWindowTitle(STRANS(STRING_EVENT_TEXT));

    CMDObjectTreeWindow::changeLanguage();
}
