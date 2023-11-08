#include "MDObjectTreeWindow.h"
#include "MDObjectTreeModel.h"
#include "MDObjectTreeWindowParaImp.h"
#include "MDSelectObjectDialog.h"
#include "MDStringResource.h"
#include "PropertyBrowser.h"

#include <QAction>
#include <QMenu>
#include <QTreeWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QLineEdit>
#include <QVBoxLayout>

CMDObjectTreeWindow::CMDObjectTreeWindow(QWidget *parent)
{

}
CMDObjectTreeWindow::~CMDObjectTreeWindow()
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;

    if(p->m_objectTreeModel)
        delete p->m_objectTreeModel;
}

/*!
\section 初始化与关闭
*/
void CMDObjectTreeWindow::init(void* objectUI, void *objectConfiger)
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;

    p->m_treeWindow = this;

    p->m_objectUI = objectUI;
    p->m_objectConfiger = objectConfiger;

    if(!p->m_objectTreeModel)
    {
        p->m_objectTreeModel = new CMDObjectTreeModel;
        QStringList sourceList;
        p->m_objectTreeModel->initial(this, sourceList);
    }

    initTreeView();
    initMenu();

    connect(&p->m_batchAdd, &CMDObjBatchOperationExpert::newJsonCreate, p, &CMDObjectTreeWindowParaImp::addOneOnBatch);
}

bool CMDObjectTreeWindow::doModalSelectObjectUI(const QString& title, qint16 objectType, const QStringList& sourceList, QStringList& destList)
{
    bool result = false;
    CMDSelectObjectDialog dlg;
    dlg.init(title, this, sourceList, m_para.isAddObjOnSelect());
    CMDMultiLanguage::instance()->translateWidget(&dlg);
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        dlg.selectedObjects_Name(destList, objectType);
        result = true;
    }
    return result;
}

bool CMDObjectTreeWindow::doModalSelectObjectUI(const QString& title, qint16 objectType, const QStringList& sourceList, QList<void*>& destList)
{
    bool result = false;
    CMDSelectObjectDialog dlg;
    dlg.init(title, this, sourceList, m_para.isAddObjOnSelect());
    CMDMultiLanguage::instance()->translateWidget(&dlg);
    dlg.exec();
    if(QDialog::Accepted == dlg.result())
    {
        dlg.selectedObjects_Ptr(destList, objectType);
        result = true;
    }
    return result;
}

void CMDObjectTreeWindow::closeEvent(QCloseEvent *e)
{
    CBaseForm::closeEvent(e);
}

bool CMDObjectTreeWindow::save()
{
    return false;
}

/*!
\section 初始化树控件；处理树控件事件
*/
void CMDObjectTreeWindow::initTreeView()
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;

    QTreeView* treeView = new QTreeView();
    p->m_treeView = treeView;

    setCentralWidget(treeView);
    treeView->setAlternatingRowColors(true);
    treeView->setAnimated(false);
    treeView->setAllColumnsShowFocus(true);
    treeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    treeView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(treeView, &QTreeView::clicked, this, &CMDObjectTreeWindow::treeViewClicked);
    connect(treeView, &QTreeView::doubleClicked, this, &CMDObjectTreeWindow::treeViewDoubleClicked);
    connect(treeView, &QTreeView::pressed, this, &CMDObjectTreeWindow::treeViewNodeMouseClicked);

    treeView->setModel(p->m_objectTreeModel);

    initTreeColumnWidth(false, treeView);
}

void CMDObjectTreeWindow::treeViewClicked(const QModelIndex &index)
{
    doConfigInBrowser();
}

void CMDObjectTreeWindow::treeViewDoubleClicked(const QModelIndex &index)
{
    doConfig();
}

void CMDObjectTreeWindow::treeViewNodeMouseClicked(const QModelIndex &index)
{
    doConfigInBrowser();
}

void CMDObjectTreeWindow::resetModelData()
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;
    p->resetModelData();
}

void CMDObjectTreeWindow::initTreeColumnWidth(bool enableChecked, QTreeView *treeView)
{

}

void CMDObjectTreeWindow::initHeader(bool enableChecked, QStringList& headerList)
{
}

/*!
\section 批量增加与配置
*/
void CMDObjectTreeWindow::showBatchAddDialog(const QJsonArray& ja)
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;
    p->showBatchAddDialog(ja);
}

QJsonObject CMDObjectTreeWindow::propToJson(const QString& propID, const QString& propComment,
                                          const QString& refValue, const QString& formatValue, int start, int step, int base)
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;
    return p->propToJson(propID, propComment, refValue, formatValue, start, step, base);
}

int CMDObjectTreeWindow::showBatchConfigDialog(QObject* objForPropertyBrowser, const QString& title)
{
    CPropertyBrowser* browser = new CPropertyBrowser;
    browser->setObject(objForPropertyBrowser);
    browser->setExpandAll();
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setMargin(1);
    layout->addWidget(browser);
    QDialog dlg;
    dlg.setLayout(layout);
    dlg.setWindowTitle(title);
    dlg.setFixedSize(340, 400);
    dlg.exec();
    return dlg.result();
}

/*!
\section 管理菜单；新建-》具体菜单
*/
void CMDObjectTreeWindow::showSubMenuForAdd()
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;

    if(p->m_subMenuForAdd)
        p->m_subMenuForAdd->exec(QCursor::pos());
    else
    {
        doAdd(-1);
    }
}

void CMDObjectTreeWindow::initMenu()
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;

    p->m_actAdd = new QAction(QIcon(":/rs/image/add"), STRING_NEW, this);
    p->m_actConfig = new QAction(QIcon(":/rs/image/config"), STRING_CONFIG, this);
    p->m_actCopy = new QAction(QIcon(":/rs/image/copy"), STRING_COPY, this);
    p->m_actPaste = new QAction(QIcon(":/rs/image/Paste"), STRING_PASTE, this);
    p->m_actDelete = new QAction(QIcon(":/rs/image/delete"), STRING_DELETE, this);
    p->m_actBatchAdd = new QAction(QIcon(":/rs/image/add"), STRING_BATCH_NEW, this);

    p->m_actCopy->setShortcut(QKeySequence::Copy);
    p->m_actPaste->setShortcut(QKeySequence::Paste);
    p->m_actDelete->setShortcut(QKeySequence::Delete);

    connect(p->m_actAdd, &QAction::triggered, this, &CMDObjectTreeWindow::doAdd);
    connect(p->m_actConfig, &QAction::triggered, this, &CMDObjectTreeWindow::doConfig);
    connect(p->m_actCopy, &QAction::triggered, this, &CMDObjectTreeWindow::doCopy);
    connect(p->m_actPaste, &QAction::triggered, this, &CMDObjectTreeWindow::doPaste);
    connect(p->m_actDelete, &QAction::triggered, this, &CMDObjectTreeWindow::doDelete);
    connect(p->m_actBatchAdd, &QAction::triggered, this, &CMDObjectTreeWindow::doBatchAdd);

    // 对“新建”菜单项做进一步处理：
    // 缺省的行为是直接将“新建”菜单项绑定一个 Qt 动作，
    // 而类似事件和动作的对象将在"新建"菜单项下建立子菜单
    p->m_menu = new QMenu(p->m_treeView);
    if(!initAddAction())
    {
        QMenu* m = p->m_menu->addMenu(STRING_NEW);
        p->m_subMenuForAdd = m;
        initSubMenuOfAdd(m); // 子菜单
    }

    p->m_menu->addAction(p->m_actDelete);
    p->m_menu->addSeparator();
    p->m_menu->addAction(p->m_actConfig);
    p->m_menu->addSeparator();
    p->m_menu->addAction(p->m_actCopy);
    p->m_menu->addAction(p->m_actPaste);
    p->m_menu->addSeparator();
    p->m_menu->addAction(p->m_actBatchAdd);
    p->m_menu->addSeparator();

    p->m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(p->m_treeView, &QTreeView::customContextMenuRequested,
            this, &CMDObjectTreeWindow::onCustomContextMenuRequested);

    p->initTranslateTool(this);
}

QAction* CMDObjectTreeWindow::addAction(QMenu* subMenu, const QString& objectTypeText)
{
    QAction* qaction = new QAction(QIcon(":/rs/image/add"), objectTypeText, this);
    subMenu->addAction(qaction);
    return qaction;
}

void CMDObjectTreeWindow::keyPressEvent(QKeyEvent * event)
{
    if(Qt::ControlModifier == event->modifiers())
    {
        if(event->key() == Qt::Key_C)
        {
            doCopy();
        }
        else if(event->key() == Qt::Key_V)
        {
            doPaste();
        }
    }
    else if(Qt::NoModifier == event->modifiers())
    {
        if(event->key() == Qt::Key_Delete)
        {
            doDelete();
        }
    }
}

//template <typename Func2>
//QAction* CMDObjectTreeWindow::addAction(QMenu* subMenu, const QString& objectTypeText,
//                                       const typename QtPrivate::FunctionPointer<Func2>::Object *receiver, Func2 slot)
//{
//    QAction* qaction = new QAction(QIcon(":/rs/image/add"), objectTypeText, this);
//    connect(qaction, &QAction::triggered, this, slot);
//    subMenu->addAction(qaction);
//    return qaction;
//}

void CMDObjectTreeWindow::onCustomContextMenuRequested(const QPoint &pos)
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;

    QModelIndex index = p->m_treeView->indexAt(pos);
    CMDObjectTreeItem *item = NULL;
    if (index.isValid())
        item = static_cast<CMDObjectTreeItem*>(index.internalPointer());

    onMenuItemVisible(item);
    p->m_actPaste->setVisible(p->m_copyList.count() > 0);

    p->changeLanguage();
    p->m_menu->exec(QCursor::pos());
}

void CMDObjectTreeWindow::setMenuItemVisible(bool visible)
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;

    p->m_actConfig->setVisible(visible);
    p->m_actCopy->setVisible(visible);
    p->m_actPaste->setVisible(visible);
    p->m_actDelete->setVisible(visible);
    p->m_actBatchAdd->setVisible(visible);
}

void CMDObjectTreeWindow::onMenuItemVisible(CMDObjectTreeItem *selectedTreeItem)
{
}

bool CMDObjectTreeWindow::initAddAction()
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;
    p->m_menu->addAction(p->m_actAdd);
    p->m_menu->addSeparator();

    return true;
}

void CMDObjectTreeWindow::initSubMenuOfAdd(QMenu* subMenu)
{
}

/*!
\section 派生类需要重载的接口
*/
void CMDObjectTreeWindow::buildObjectTree(CMDObjectTreeItem* rootItem)
{

}

void CMDObjectTreeWindow::doAdd(qint16 subType)
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;

    if(true == addObject(p->m_objectUI, p->m_objectConfiger, subType))
    {
        save();

        p->resetModelData();
    }
}

bool CMDObjectTreeWindow::addObject(void* objectUI, void *objectConfiger, qint16 subType)
{
    return false;
}

void CMDObjectTreeWindow::doConfig()
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;
    QList<CMDObjectTreeItem*> items;
    p->selectedTopItems(items);
    if(items.size() <= 0)
        return;

    if(true == configObject(p->m_objectUI, p->m_objectConfiger, items))
    {
        save();

        p->resetModelData();
    }
}

bool CMDObjectTreeWindow::configObject(void* objectUI, void *objectConfiger, QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    return false;
}

void CMDObjectTreeWindow::doConfigInBrowser()
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;
    QList<CMDObjectTreeItem*> items;
    p->selectedTopItems(items);
    if(items.size() <= 0)
        return;

    configObjectInBrowser(p->m_objectUI, p->m_objectConfiger, (CMDObjectTreeItem*)items.first());
}

bool CMDObjectTreeWindow::configObjectInBrowser(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    return false;
}

void CMDObjectTreeWindow::doCopy()
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;
    QList<CMDObjectTreeItem*> items;
    p->selectedTopItems(items);
    if(items.size() <= 0)
        return;

    QList<CMDObjectTreeItem*> copyList;
    foreach (CMDObjectTreeItem* item, items)
    {
        if(isTopObject(item->type()))
            copyList.append(item);
    }
    p->prepareCopy(copyList);
}

void CMDObjectTreeWindow::doPaste()
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;
    if(p->m_copyList.count() <= 0)
        return;

    int count = p->m_copyList.count();
    for(int i = 0; i < count; i++)
        pasteObject(p->m_copyList.at(i));

    save();

    p->resetModelData();
}

bool CMDObjectTreeWindow::pasteObject(void *objectPtr)
{
    return false;
}

void CMDObjectTreeWindow::doDelete()
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;
    QList<CMDObjectTreeItem*> items;
    p->selectedTopItems(items);

    if(true == deleteObject(items))
    {
        save();

        p->clearCopyList();
        p->resetModelData();

        QList<CMDObjectTreeItem*> items1;
        p->selectedTopItems(items1);
        objectDeleted(items1);
    }
}

bool CMDObjectTreeWindow::deleteObject(QList<CMDObjectTreeItem*>& selectedTreeItems)
{
    return false;
}

void CMDObjectTreeWindow::objectDeleted(QList<CMDObjectTreeItem*>& selectedTreeItems)
{

}

void CMDObjectTreeWindow::doBatchAdd()
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;
    QList<CMDObjectTreeItem*> items;
    p->selectedTopItems(items);
    if(items.size() <= 0)
        return;

    if(true == batchAddObject(p->m_objectUI, p->m_objectConfiger, (CMDObjectTreeItem *)items.first()))
    {
        save();

        p->resetModelData();
    }
}

bool CMDObjectTreeWindow::batchAddObject(void* objectUI, void *objectConfiger, CMDObjectTreeItem *selectedTreeItem)
{
    return false;
}

bool CMDObjectTreeWindow::onBatchAddOneObject(const QJsonArray &ja, QString& resultMsg)
{
    return false;
}

QString CMDObjectTreeWindow::objectName(void* objectPtr)
{
    return "";
}

bool CMDObjectTreeWindow::resultOnSetData(qint16 objectType)
{
    return true;
}

QVariant CMDObjectTreeWindow::objectData(bool enableChecked, CMDObjectTreeItem *item, int column, int role) const
{
    return QVariant();
}

bool CMDObjectTreeWindow::isTopObject(qint16 objectType)
{
    return false;
}

/*!
\section 多语言
*/
void CMDObjectTreeWindow::changeLanguage()
{
    CMDObjectTreeWindowParaImp* p = m_para.m_imp;
    p->resetModelData();
}
