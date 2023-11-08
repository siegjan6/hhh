#include "MDSwitchTreeWindow.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "../../include/MDObjectManagerUI/MDObjectTreeItem.h"
#include "MDSwitchFactory.h"
#include <QtWidgets/qtreewidget.h>
#include "../../include/MDMultiLanguage/MDMultiLanguage.h"

CMDSwitchTreeWindow::CMDSwitchTreeWindow(QWidget *parent)
{
    m_f = NULL;
}

CMDSwitchTreeWindow::~CMDSwitchTreeWindow()
{
}

/*!
\section 初始化
*/
void CMDSwitchTreeWindow::init(CMDSwitchFactory *f)
{
    m_f = f;
    CMDObjectTreeWindow::init(NULL, f);

    m_actionIcon =QIcon(":/images/device.png");
    setWindowTitle(STRANS("开关"));
}

bool CMDSwitchTreeWindow::save()
{
    return (m_f ? m_f->save() : false);
}

/*!
\section 重载的接口
*/
void CMDSwitchTreeWindow::buildObjectTree(CMDObjectTreeItem* rootItem)
{
    if(!m_f)
        return;

    QList<CMDMObj*> switchList;
    m_f->enumAll(switchList);

    // 原始的动作 ---》通用的 treeitem，rootItem 管理全部 treeitem
    // --------------------------------------------------------------------------------
    QList<CMDMObj*>::iterator iAction;
    for (iAction = switchList.begin(); iAction != switchList.end(); ++iAction)
    {
        CMDMObj* mobj = *iAction;
        CMDObjectTreeItem* actionItem = new CMDObjectTreeItem((qint16)MDObjectType::mdswitch, mobj, rootItem);
        rootItem->appendChild(actionItem);
    }
}

bool CMDSwitchTreeWindow::addObject(void* objectUI, void *objectConfiger, qint16 subType)
{
    bool result = false;
    return result;
}

bool CMDSwitchTreeWindow::configObject(void* objectUI, void *objectConfiger, QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    bool result = false;
    return result;
}

bool CMDSwitchTreeWindow::configObjectInBrowser(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    return true;
}

bool CMDSwitchTreeWindow::deleteObject(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    return true;
}

void CMDSwitchTreeWindow::objectDeleted(QList<CMDObjectTreeItem*>& selectedTreeItems)
{

}

bool CMDSwitchTreeWindow::pasteObject(void* objectPtr)
{
    return false;
}

QString CMDSwitchTreeWindow::objectName(void* objectPtr)
{
    CMDMObj* cfg = (CMDMObj*)objectPtr;
    return (cfg ? cfg->para().group() : "");
}

bool CMDSwitchTreeWindow::resultOnSetData(qint16 objectType)
{
    bool result = false;
    if(objectType == (qint16)MDObjectType::mdswitch)
    {
        result = true;
    }
    return result;
}

QVariant CMDSwitchTreeWindow::objectData(bool enableChecked, CMDObjectTreeItem *item, int column, int role) const
{
    if(item->type() == (qint16)MDObjectType::mdswitch)
    {
        if(role==Qt::DecorationRole)
            return (column==0 ? m_actionIcon : QVariant());
        else
        {
            CMDMObj* mobj  = static_cast<CMDMObj*>(item->ptr());
            return actionData(enableChecked, mobj, column);
        }
    }
    else return QVariant();
}

// 实际的列，参考 initHeader()
QVariant CMDSwitchTreeWindow::actionData(bool enableChecked, CMDMObj* mobj, int propIndex) const
{
    if(enableChecked) // 用于选择的界面，可减少列数
    {
        if(propIndex<=0)
            return QVariant(mobj->para().group());
        else if(propIndex==1)
            return QVariant(mobj->para().comment());
        else
            return QVariant();
    }
    else
    {
        if(propIndex<=0)
            return QVariant(mobj->para().group());
        else if(propIndex==1)
            return QVariant(mobj->para().comment());
        else
            return QVariant();

//        if(propIndex<=0)
//            return QVariant(actionProperty->name());
//        else if(propIndex==1)
//            return QVariant(PTRANS(actionProperty->comment()));
//        else if(propIndex==2)
//              return QVariant(STRANS(CMDActionID::typeToString(actionProperty->actionID().type())));
//        else if(propIndex==3)
//            return QVariant(actionProperty->actionID().objName());
//        else
//            return QVariant();
    }
}

bool CMDSwitchTreeWindow::isTopObject(qint16 objectType)
{
    return (objectType == (qint16)MDObjectType::mdswitch);
}

void CMDSwitchTreeWindow::initTreeColumnWidth(bool enableChecked, QTreeView *treeView)
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
//        treeView->setColumnWidth(0, 200);
//        treeView->setColumnWidth(1, 140);
//        treeView->setColumnWidth(2, 140);
//        treeView->setColumnWidth(3, 100);
    }
}

void CMDSwitchTreeWindow::initHeader(bool enableChecked, QStringList& headerList)
{
    // 多语言：此处作为原始串，翻译由 MDObjectManagerUI 完成
    if(enableChecked) // 用于选择的界面，可减少列数
    {
        headerList.append("名称");
        headerList.append("注释");
    }
    else
    {
        headerList.append("名称");
        headerList.append("注释");

//        headerList.append(STRING_NAME);
//        headerList.append(STRING_COMMENT);
//        headerList.append(STRING_TYPE);
//        headerList.append(STRING_OBJECT);
    }
}

void CMDSwitchTreeWindow::onMenuItemVisible(CMDObjectTreeItem *selectedTreeItem)
{
    bool visible = false;
    if (!selectedTreeItem)
        visible = false;
    else
        visible = (selectedTreeItem->type() == (qint16)MDObjectType::mdswitch);

    setMenuItemVisible(visible);
}

bool CMDSwitchTreeWindow::initAddAction()
{
    return false;
}

void CMDSwitchTreeWindow::initSubMenuOfAdd(QMenu* subMenu)
{
}

bool CMDSwitchTreeWindow::batchAddObject(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    return true;
}

/*!
\section 多语言
*/
void CMDSwitchTreeWindow::changeLanguage()
{
    setWindowTitle(STRANS("动作"));

    CMDObjectTreeWindow::changeLanguage();
}
