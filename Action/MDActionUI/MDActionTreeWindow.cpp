#include "MDActionTreeWindow.h"
#include "MDActionConfiger.h"
#include "../../include/MDProjectConfiger/MDProjectConfiger.h"

#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonArray>

#include "MDActionMainDialog.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "MDObjectTreeItem.h"
#include "MDActionUI.h"
#include "MDActionUIImp.h"
#include "MDStringResource.h"
#include "PropertyBrowser.h"

CMDActionTreeWindow::CMDActionTreeWindow(QWidget *parent)
{
    m_actionUI = NULL;
    m_actionConfiger = NULL;
    m_batchNewProperty = NULL;
}

CMDActionTreeWindow::~CMDActionTreeWindow()
{
    if(m_batchNewProperty)
        delete m_batchNewProperty;
}

/*!
\section 初始化
*/
void CMDActionTreeWindow::init(CMDActionUIImp* actionUI, CMDActionConfiger *actionConfiger)
{
    m_actionUI = actionUI;
    m_actionConfiger = actionConfiger;
    CMDObjectTreeWindow::init(actionUI, actionConfiger);

    m_actionIcon =QIcon(":/images/device.png");
    setWindowTitle(STRANS(STRING_ACTION_TEXT));

    m_ActionProperty_ConfigForBrowser.setConfiger(actionConfiger);
    m_ActionProperty_ConfigForBrowser.setObjTreeWindow(this);

    m_VariableWriteValueForBrowser.setConfiger(actionConfiger);
    m_VariableWriteValueForBrowser.setObjTreeWindow(this);

    m_WindowOperationForBrowser.setConfiger(actionConfiger);
    m_WindowOperationForBrowser.setObjTreeWindow(this);
}

bool CMDActionTreeWindow::save()
{
    if(m_actionConfiger)
        return m_actionConfiger->saveToFile();
    return false;
}

bool CMDActionTreeWindow::selectSingleObject(MDActionType type, const QString& source, QString& dest)
{
    CMDActionUIContacter* contacter = (m_actionUI ? m_actionUI->contacter() : NULL);
    return (contacter ? contacter->onSelectSingleObject(type, source, dest) : false);
}

/*!
\section 复制与粘贴
*/
QString CMDActionTreeWindow::createName(const QString& sourceName)
{
    int count = m_actionConfiger->count();
    for(int i= 0; i < count; i++)
    {
      QString name = QString("%1_%2").arg(sourceName).arg(QString::number(i+1));
      if(!m_actionConfiger->findAction(name))
          return name;
    }
    return QString("%1_%2").arg(sourceName).arg(QString::number(count+1));
}

/*!
\section 重载的接口
*/
void CMDActionTreeWindow::buildObjectTree(CMDObjectTreeItem* rootItem)
{
    if(!m_actionConfiger)
        return;

    QList<CMDActionProperty_Config*> actionList;
    m_actionConfiger->configedActionList(actionList);

    // 原始的动作 ---》通用的 treeitem，rootItem 管理全部 treeitem
    // --------------------------------------------------------------------------------
    QList<CMDActionProperty_Config*>::iterator iAction;
    for (iAction = actionList.begin(); iAction != actionList.end(); ++iAction)
    {
        CMDActionProperty_Config* actionProperty = *iAction;
        CMDObjectTreeItem* actionItem = new CMDObjectTreeItem((qint16)MDObjectType::action, actionProperty, rootItem);
        rootItem->appendChild(actionItem);
    }
}

bool CMDActionTreeWindow::addObject(void* objectUI, void *objectConfiger, qint16 subType)
{
    bool result = false;
    CMDActionMainDialog dlg(this);
    CMDActionUIImp* uiImp = (CMDActionUIImp*)objectUI;
    dlg.setPowerGroupConfiger(uiImp->m_studioInterface->projectConfiger()->powerGroupConfiger());
    dlg.initial(this, objectConfiger, subType);
    dlg.exec();
    result = (QDialog::Accepted == dlg.result());
    return result;
}

bool CMDActionTreeWindow::configObject(void* objectUI, void *objectConfiger, QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    bool result = false;
    if(1 == selectedTreeItems.size())
    {
        CMDObjectTreeItem* item = (CMDObjectTreeItem*)selectedTreeItems.first();
        if(item->type() == (qint16)MDObjectType::action)
        {
            CMDActionMainDialog dlg(this);
            CMDActionUIImp* uiImp = (CMDActionUIImp*)objectUI;
            dlg.setPowerGroupConfiger(uiImp->m_studioInterface->projectConfiger()->powerGroupConfiger());
            dlg.initial(this, objectConfiger, item->ptr());
            dlg.setWindowTitle(STRANS(STRING_ACTION_TEXT) +  " - " + objectName(item->ptr()));
            dlg.exec();
            result = (QDialog::Accepted == dlg.result());
        }
    }
    else
    {
        QList<CMDActionProperty_Config*> MDObjects;
        foreach (CMDObjectTreeItem* item, selectedTreeItems)
            MDObjects.append((CMDActionProperty_Config*)item->ptr());

        QObject* objForBatchConfig = getMDObjectForBactch(MDObjects);
        QString title = QObject::tr("批量编辑 - 动作");
        result = (QDialog::Accepted == showBatchConfigDialog(objForBatchConfig, title));
    }
    return result;
}

/*!
\section 属性框支持；批量修改属性
*/
bool CMDActionTreeWindow::configObjectInBrowser(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    bool result = false;
    if(selectedTreeItem->type() == (qint16)MDObjectType::action)
    {
        CMDActionUIImp* uiImp = (CMDActionUIImp*)objectUI;
        if(uiImp)
        {
            CMDActionProperty_Config* baseProp = ((CMDActionProperty_Config*)selectedTreeItem->ptr());
            CPropertyBrowser* browser = uiImp->m_studioInterface->propertyBrowser();
            browser->setObject(getMDObject(baseProp));
            browser->updateProperty();
        }
    }
    return result;
}

QObject* CMDActionTreeWindow::getMDObjectForBactch(QList<CMDActionProperty_Config*>& MDObjects)
{
    if(MDObjects.size() <= 0)
        return &m_ActionProperty_ConfigForBrowser;

    bool sameType = true;
    MDActionType theType = MDObjects.first()->actionID().type();
    foreach (CMDActionProperty_Config* ap, MDObjects)
    {
        if(theType != ap->actionID().type())
        {
            sameType = false;
            break;
        }
    }

    CMDActionProperty_ConfigForBrowser* APForBrowser = (sameType ?
                                                            (CMDActionProperty_ConfigForBrowser*)getMDObject(MDObjects.first()) :
                                                            &m_ActionProperty_ConfigForBrowser);
    APForBrowser->setMDObjects(MDObjects);
    return APForBrowser;
}

QObject* CMDActionTreeWindow::getMDObject(CMDActionProperty_Config* baseProp)
{
    QObject* theObject = NULL;

    m_ActionProperty_ConfigForBrowser.setMDObject(baseProp);
    switch (baseProp->actionID().type())
    {
    case MDActionType::variable_WriteValue:
        m_VariableWriteValueForBrowser.setMDObject((CAPC_VariableWriteValue*)baseProp);
        theObject = &m_VariableWriteValueForBrowser;
    break;

    case MDActionType::tts_Play:
        theObject = &m_ActionProperty_ConfigForBrowser;
    break;

    case MDActionType::historyGroup_Save:
        theObject = &m_ActionProperty_ConfigForBrowser;
    break;

    case MDActionType::window_Open:
    case MDActionType::window_Close:
    case MDActionType::window_Move:
    case MDActionType::window_Hide:
    {
        m_WindowOperationForBrowser.setMDObject((CAPC_WindowOperation*)baseProp);
        theObject = &m_WindowOperationForBrowser;
    }
    break;
    default:
        theObject = &m_ActionProperty_ConfigForBrowser;
        break;
    }
    return theObject;
}

bool CMDActionTreeWindow::deleteObject(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    QString str;
    if(1 == selectedTreeItems.size())
    {
        QString name = objectName(selectedTreeItems.first()->ptr());
        str = QString(STRANS(STRING_CONFIRM_DELETE_ACTION) + ": %1 ?").arg(name);
    }
    else
        str = QString(STRANS(STRING_CONFIRM_DELETE_MULTI_ACTION)).arg(selectedTreeItems.size());

    QMessageBox::StandardButton reply = QMessageBox::question(this, STRANS(STRING_OK), str, QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No)
        return false;

    foreach (CMDObjectTreeItem* item, selectedTreeItems)
    {
        QString name = objectName(item->ptr());
        m_actionConfiger->deleteAction(name);
    }
    return true;
}

void CMDActionTreeWindow::objectDeleted(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    if(!m_actionUI)
        return;

    if(selectedTreeItems.size() <= 0)
        return;

    CMDActionProperty_Config* baseProp = ((CMDActionProperty_Config*)selectedTreeItems.first()->ptr());
    CPropertyBrowser* browser = m_actionUI->m_studioInterface->propertyBrowser();
    browser->setObject(getMDObject(baseProp));
    browser->updateProperty();
}

bool CMDActionTreeWindow::pasteObject(void* objectPtr)
{
    CMDActionProperty_Config* source = (CMDActionProperty_Config*)objectPtr;
    CMDActionProperty_Config* dest = CAPC_Factory::newConfig(source->actionID().type(), source);
    if(!dest)
        return false;

    QString destName = createName(source->name());
    dest->setName(destName);
    bool result = m_actionConfiger->addAction(dest);
    delete dest;
    return result;
}

QString CMDActionTreeWindow::objectName(void* objectPtr)
{
    CMDActionProperty_Config* cfg = (CMDActionProperty_Config*)objectPtr;
    return (cfg ? cfg->name() : "");
}

bool CMDActionTreeWindow::resultOnSetData(qint16 objectType)
{
    bool result = false;
    if(objectType == (qint16)MDObjectType::action)
    {
        result = true;
    }
    return result;
}

QVariant CMDActionTreeWindow::objectData(bool enableChecked, CMDObjectTreeItem *item, int column, int role) const
{
    if(item->type() == (qint16)MDObjectType::action)
    {
        if(role==Qt::DecorationRole)
            return (column==0 ? m_actionIcon : QVariant());
        else
        {
            CMDActionProperty_Config* actionProperty  = static_cast<CMDActionProperty_Config*>(item->ptr());
            return actionData(enableChecked, actionProperty, column);
        }
    }
    else return QVariant();
}

/*!
\section 批量增加
*/
bool CMDActionTreeWindow::batchAddObject(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    bool result = false;
    if(selectedTreeItem->type() == (qint16)MDObjectType::action)
    {
        CMDActionProperty_Config* sourceProperty = (CMDActionProperty_Config*)selectedTreeItem->ptr();
        if(m_batchNewProperty)
            delete m_batchNewProperty;
        m_batchNewProperty = CAPC_Factory::newConfig(sourceProperty->actionID().type(), sourceProperty);
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

bool CMDActionTreeWindow::onBatchAddOneObject(const QJsonArray &ja, QString& resultMsg)
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

    bool result = m_actionConfiger->addAction(m_batchNewProperty);
    if(!result)
        resultMsg = m_batchNewProperty->name();
    return result;
}

// 实际的列，参考 initHeader()
QVariant CMDActionTreeWindow::actionData(bool enableChecked, CMDActionProperty_Config* actionProperty, int propIndex) const
{
    if(enableChecked) // 用于选择的界面，可减少列数
    {
        if(propIndex<=0)
            return QVariant(actionProperty->name());
        else if(propIndex==1)
            return QVariant(actionProperty->actionID().objName());
        else
            return QVariant();
    }
    else
    {
        if(propIndex<=0)
            return QVariant(actionProperty->name());
        else if(propIndex==1)
            return QVariant(PTRANS(actionProperty->comment()));
        else if(propIndex==2)
              return QVariant(STRANS(CMDActionID::typeToString(actionProperty->actionID().type())));
        else if(propIndex==3)
            return QVariant(actionProperty->actionID().objName());
        else
            return QVariant();
    }
}

bool CMDActionTreeWindow::isTopObject(qint16 objectType)
{
    return (objectType == (qint16)MDObjectType::action);
}

void CMDActionTreeWindow::initTreeColumnWidth(bool enableChecked, QTreeView *treeView)
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
    }
}

void CMDActionTreeWindow::initHeader(bool enableChecked, QStringList& headerList)
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
    }
}

void CMDActionTreeWindow::onMenuItemVisible(CMDObjectTreeItem *selectedTreeItem)
{
    bool visible = false;
    if (!selectedTreeItem)
        visible = false;
    else
        visible = (selectedTreeItem->type() == (qint16)MDObjectType::action);

    setMenuItemVisible(visible);
}

bool CMDActionTreeWindow::initAddAction()
{
    return false;
}

void CMDActionTreeWindow::initSubMenuOfAdd(QMenu* subMenu)
{
    QAction* qAction = NULL;

    qAction = addAction(subMenu, CMDActionID::typeToString(MDActionType::historyGroup_Save));
    connect(qAction, &QAction::triggered, this, &CMDActionTreeWindow::doAdd_historyGroup_Save);

    qAction = addAction(subMenu, CMDActionID::typeToString(MDActionType::variable_WriteValue));
    connect(qAction, &QAction::triggered, this, &CMDActionTreeWindow::doAdd_variable_WriteValue);

    qAction = addAction(subMenu, CMDActionID::typeToString(MDActionType::window_Open));
    connect(qAction, &QAction::triggered, this, &CMDActionTreeWindow::doAdd_window_Open);

    qAction = addAction(subMenu, CMDActionID::typeToString(MDActionType::window_Close));
    connect(qAction, &QAction::triggered, this, &CMDActionTreeWindow::doAdd_window_Close);
}

/*!
\section 子菜单：新建具体的动作（写变量、保存历史组 ...）
*/
void CMDActionTreeWindow::doAdd_historyGroup_Save()
{
    doAdd((qint16)MDActionType::historyGroup_Save);
}

void CMDActionTreeWindow::doAdd_variable_WriteValue()
{
    doAdd((qint16)MDActionType::variable_WriteValue);
}

void CMDActionTreeWindow::doAdd_window_Open()
{
    doAdd((qint16)MDActionType::window_Open);
}

void CMDActionTreeWindow::doAdd_window_Close()
{
    doAdd((qint16)MDActionType::window_Close);
}

/*!
\section 多语言
*/
void CMDActionTreeWindow::changeLanguage()
{
    setWindowTitle(STRANS(STRING_ACTION_TEXT));

    CMDObjectTreeWindow::changeLanguage();
}
