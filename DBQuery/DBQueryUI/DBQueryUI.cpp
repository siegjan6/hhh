#include "DBQueryUI.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDir>
#include <QDebug>
#include "MDMultiLanguage.h"
#include "RuntimeSourceDialog.h"
#include "ViewVarTableDialog.h"
#include "ViewFieldTableDialog.h"
#include "ViewFilterTreeDialog.h"
#include "ViewDBSourceDialog.h"
#include "ViewListDialog.h"
#include <QMessageBox>

DBQueryUI::DBQueryUI():
    m_rootInStudio(NULL),
    m_studioInterface(NULL),
    m_pRoot(NULL),
    m_DBSourceRoot(NULL),
    m_ViewRoot(NULL),
    m_ViewVar(NULL),
    m_ViewList(NULL),
    m_pDBView_PopupMenu(NULL),
    m_pDBView_NewAction(NULL),
    m_pDBView_Rename(NULL),
    m_pDBView_DeleteAction(NULL),
    sRootName(QString(tr("数据视图"))),
    sDBSourceName(QString(tr("数据源"))),
    sViewName(QString(tr("视图"))),
    sViewVarName(QString(tr("变量"))),
    sViewListName(QString(tr("视图列表"))),
    m_DBSourceFilePath(""),
    m_ViewVarFilePath(""),
    m_transTool(CMDMultiLanguage::createWidgetTool())
{
    m_transTool->init(this);
    //必须是Qt::QueuedConnection类型因为在rebuildViewTreeSlot会删除节点 再新建节点
    connect(this, SIGNAL(rebuildViewTreeSignal()), this, SLOT(rebuildViewTreeSlot()), Qt::QueuedConnection);
}

DBQueryUI::~DBQueryUI()
{
    unLoad();
}

void DBQueryUI::load()
{
    connect(m_studioInterface->treeWidget(), &QTreeWidget::doubleClicked, this, &DBQueryUI::onMouseDbClick);
    connect(m_studioInterface->treeWidget(), &QTreeWidget::customContextMenuRequested, this, &DBQueryUI::onMouseRClick);
    buildDBQueryUIDirTree(treeRoot());
}

void DBQueryUI::unLoad()
{
    //    if(m_rootInStudio)
    //    {
    //        delete m_rootInStudio;
    //        m_rootInStudio = NULL;
    //    }
    //    if(m_studioInterface)
    //    {
    //        delete m_studioInterface;
    //        m_studioInterface = NULL;
    //    }
    //    if(m_pRoot)
    //    {
    //        delete m_pRoot;
    //        m_pRoot = NULL;
    //    }
    //    if(m_DBSourceRoot)
    //    {
    //        delete m_DBSourceRoot;
    //        m_DBSourceRoot = NULL;
    //    }
    //    if(m_ViewRoot)
    //    {
    //        delete m_ViewRoot;
    //        m_ViewRoot = NULL;
    //    }
    //    if(m_ViewVar)
    //    {
    //        delete m_ViewVar;
    //        m_ViewVar = NULL;
    //    }
    //    if(m_ViewList)
    //    {
    //        delete m_ViewList;
    //        m_ViewList = NULL;
    //    }
    //    if(m_pDBView_PopupMenu)
    //    {
    //        delete m_pDBView_PopupMenu;
    //        m_pDBView_PopupMenu = NULL;
    //    }
    //    if(m_pDBView_NewAction)
    //    {
    //        delete m_pDBView_NewAction;
    //        m_pDBView_NewAction = NULL;
    //    }if(m_pDBView_NewAction)
    //    {
    //        delete m_pDBView_DeleteAction;
    //        m_pDBView_DeleteAction = NULL;
    //    }
}

QTreeWidget *DBQueryUI::treeWidget()
{
    return (m_studioInterface ? m_studioInterface->treeWidget() : NULL);
}

QTreeWidgetItem *DBQueryUI::treeRoot()
{
    return m_rootInStudio;
}

QTreeWidgetItem *DBQueryUI::selectNode()
{
    if (treeWidget() != NULL)
    {
        QTreeWidgetItem* node = treeWidget()->currentItem();
        if (isItemInChildren(node,m_pRoot))
            return  node;
    }
    return NULL;
}

bool DBQueryUI::isDBQueryUINode(QTreeWidgetItem *node)
{
    return (QString::compare(node->data(0, Qt::UserRole).toString(), sRootName) == 0);
}

bool DBQueryUI::isDBSourceNode(QTreeWidgetItem *node)
{
    return (QString::compare(node->data(0, Qt::UserRole).toString(), sDBSourceName) == 0 || QString::compare(node->parent()->data(0, Qt::UserRole).toString(), sDBSourceName) == 0);
}

bool DBQueryUI::isViewNode(QTreeWidgetItem *node)
{
    return (QString::compare(node->data(0, Qt::UserRole).toString(), sViewName) == 0);
}

bool DBQueryUI::isViewVarNode(QTreeWidgetItem *node)
{
    return (QString::compare(node->data(0, Qt::UserRole).toString(), sViewVarName) == 0);
}

bool DBQueryUI::isViewListNode(QTreeWidgetItem *node)
{
    return (QString::compare(node->data(0, Qt::UserRole).toString(), sViewListName) == 0);
}

bool DBQueryUI::isViewListChildNode(QTreeWidgetItem *node)
{
    return (QString::compare(node->parent()->data(0, Qt::UserRole).toString(), sViewListName) == 0);
}

void DBQueryUI::clearNode(QTreeWidgetItem *node)
{
    for (int i = 0; i < node->childCount(); i++)
    {
        QTreeWidgetItem* child = node->child(i);
        clearNode(child);
        delete child;
    }
    node->takeChildren();
}

bool DBQueryUI::isItemInChildren(QTreeWidgetItem *hItem, QTreeWidgetItem *htrDestParent)
{
    if(hItem && htrDestParent)
    {
        do
        {
            if (hItem == htrDestParent)
                break;
        }
        while ((hItem = hItem->parent())!= NULL);
        return (hItem != NULL);
    }
    return false;
}

void DBQueryUI::buildDBQueryUIDirTree(QTreeWidgetItem *pTreeRoot)
{
    clearNode(pTreeRoot);
    pTreeRoot->setText(0, sRootName);
    pTreeRoot->setData(0, Qt::UserRole, QVariant(sRootName));
    CMDProjectConfiger *pProjectConfiger = m_studioInterface->projectConfiger();
    if(pTreeRoot && pProjectConfiger)
    {
        m_pRoot = pTreeRoot;
        m_DBQueryPath = pProjectConfiger->path()+"/"+"DBQuery";
        QDir dir(m_DBQueryPath);
        if (!dir.exists())
        {
            if (!QDir::root().mkpath(m_DBQueryPath))
            {
                qDebug()<<"Dir create failed!";
                return;
            }
        }

        if(creatDBSourceFile())
        {
            qDebug()<<"Create global_dbsources.json file succeed!";
        }

        if(creatViewVarFile())
        {
            qDebug()<<"Create global_view_vars.json file succeed!";
        }
    }

    if(isDBQueryUINode(m_pRoot))
    {
        buildDBSourceTree();
        buildViewTree();
    }
}

bool DBQueryUI::creatDBSourceFile()
{
    m_DBSourceFilePath = m_DBQueryPath+"/"+"global_dbsources.json";
    QFile DBSourceFile(m_DBSourceFilePath);
    if(!DBSourceFile.exists())
    {
        QString errStr = "";
        QList<DBQ::DBSource *> dbsourceList;
        dbsourceList.clear();
        if(DBSourceFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            //! 初始化打开项目时在数据查询文件夹下新建全局数据源 global_dbsources.json 文件
            bool ok = DBQ::DataSource_Write(m_DBSourceFilePath,DBQ::FILE_TYPE_JSON, dbsourceList, errStr);
            {
                if (ok)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool DBQueryUI::creatViewVarFile()
{
    m_ViewVarFilePath =  m_DBQueryPath+"/"+"global_view_vars.json";
    QFile viewVarFile(m_ViewVarFilePath);
    if(!viewVarFile.exists())
    {
        QString errIn;
        QList<DBQ::ViewVariable*> list;
        list.clear();
        if(viewVarFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            //! 初始化打开项目时在数据查询文件夹下新建全局视图变量 global_view_vars.json 文件
            bool ok = DBQ::ViewVariable_Write(m_ViewVarFilePath,DBQ::FILE_TYPE_JSON,list,errIn);
            if(ok)
            {
                return true;
            }
        }
    }
    return false;
}

void DBQueryUI::buildDBSourceTree()
{
    m_DBSourceRoot = new QTreeWidgetItem();
    m_DBSourceRoot->setText(0,sDBSourceName);
    m_DBSourceRoot->setData(0, Qt::UserRole, QVariant(sDBSourceName));
    m_DBSourceRoot->setIcon(0, QIcon(":/images/folder.png"));
    m_pRoot->addChild(m_DBSourceRoot);

#if 0
    QList<DBQ::DBSource *> dbsourceList;
    foreach(DBQ::DBSource * pdbsource, dbsourceList)
    {
        delete pdbsource;
    }
    LoadDBSourceList(dbsourceList);
#endif
}

void DBQueryUI::LoadDBSourceList( QList<DBQ::DBSource *> dbsourcesList)
{
    clearDBSourceChildItems();

    QString errStr = "";
    dbsourcesList.clear();
    //! 加载全局数据源文件，取出数据源链表，创建项目数据源子节点
    bool ok = DBQ::DataSource_Load(m_DBSourceFilePath, DBQ::FILE_TYPE_JSON, dbsourcesList, errStr);
    if(ok)
    {
        int size = dbsourcesList.size();
        if(size > 0)
        {
            for(int i = 0 ; i < size; i++)
            {
                QTreeWidgetItem *pSrcItem =  new QTreeWidgetItem;
                QVariant var;
                var.setValue(dbsourcesList.at(i));
                pSrcItem->setData(0,Qt::UserRole,var);
                pSrcItem->setIcon(0,QIcon(":/images/file.png"));
                pSrcItem->setText(0,dbsourcesList.at(i)->dbSrcName);
                m_DBSourceRoot->addChild(pSrcItem);
            }
        }
    }
    else
    {
        qDebug()<<"加载数据查询数据源失败！";
    }
}

void DBQueryUI::clearDBSourceChildItems()
{
    int count = m_DBSourceRoot->childCount();
    if(count != 0)
    {
        for (int i = count-1 ;i >= 0;--i)
        {
            delete m_DBSourceRoot->takeChild(i);
        }
    }
}

void DBQueryUI::buildViewTree()
{
    m_ViewRoot= new QTreeWidgetItem(QStringList(QObject::tr("视图")));
    m_ViewRoot->setIcon(0, QIcon(":/images/folder.png"));
    m_pRoot->addChild(m_ViewRoot);
    LoadViewList(m_ViewRoot);
}

void DBQueryUI::rebuildViewTreeSlot()
{
    int childrenCount = m_ViewRoot->childCount();
    for(int i=childrenCount-1; i>-1; i--)
        delete m_ViewRoot->takeChild(i);

    LoadViewList(m_ViewRoot);
}

void DBQueryUI::LoadViewList(QTreeWidgetItem *pRoot)
{

    m_ViewVar = new QTreeWidgetItem();
    m_ViewVar->setText(0,sViewVarName);
    m_ViewVar->setData(0, Qt::UserRole, QVariant(sViewVarName));
    m_ViewVar->setIcon(0, QIcon(":/images/file.png"));
    pRoot->addChild(m_ViewVar);

    m_ViewList = new QTreeWidgetItem();
    m_ViewList->setText(0,sViewListName);
    m_ViewList->setData(0, Qt::UserRole, QVariant(sViewListName));
    m_ViewList->setIcon(0, QIcon(":/images/folder.png"));
    pRoot->addChild(m_ViewList);

    m_viewNameList.clear();
    //! 从数据查询目录中遍历视图文件名称，取出视图名称链表
    DBQ::View_GetAllViewNameFromDir(m_DBQueryPath,m_viewNameList);
    //! 在视图节点下创建视图文件子节点
    creatDBQueryViewFilesItem(m_viewNameList);
}

void DBQueryUI::creatDBQueryViewFilesItem(QStringList list)
{
    int size = list.size();
    for(int i = 0; i < size; i++)
    {
        QString viewName = list.at(i);
        DBQ::View *view = NULL;
        QString errstr = "";
        QVariant var;
        viewName = m_DBQueryPath + QString("/") + "view_" + viewName + ".json";
        if(DBQ::View_Load(viewName,DBQ::FILE_TYPE_JSON,&view,errstr))
        {
            if(view)
            {
                var.setValue(view);
                QTreeWidgetItem* viewFileRoot = new QTreeWidgetItem;
                viewFileRoot->setData(0, Qt::UserRole, var);
                viewFileRoot->setIcon(0,QIcon(":/images/folder.png"));
                viewFileRoot->setText(0,view->viewName);

                QTreeWidgetItem* childField = new QTreeWidgetItem;
                childField->setIcon(0,QIcon(":/images/file.png"));
                childField->setText(0,QString(tr("字段")));
                viewFileRoot->addChild(childField);

                QTreeWidgetItem* childFilter = new QTreeWidgetItem;
                childFilter->setIcon(0,QIcon(":/images/file.png"));
                childFilter->setText(0,QString(tr("过滤条件")));
                viewFileRoot->addChild(childFilter);

                QTreeWidgetItem* childDefaultDBSource = new QTreeWidgetItem;
                childDefaultDBSource->setIcon(0,QIcon(":/images/file.png"));
                childDefaultDBSource->setText(0,QString(tr("默认数据源")));
                viewFileRoot->addChild(childDefaultDBSource);

                m_ViewList->addChild(viewFileRoot);
            }
        }
    }
}

bool DBQueryUI::isExistSameViewName(QString name)
{
    bool ok = false;
    int size = m_viewNameList.size();
    for(int i = 0 ; i < size; i++)
    {
        if(name == m_viewNameList.at(i))
        {
            ok = true;
            return ok;
        }
    }
    return ok;
}

bool DBQueryUI::removeCurrentViewName(QString name)
{
    bool ok = false;
    int size = m_viewNameList.size();
    for(int i = 0; i < size; i++)
    {
        if(m_viewNameList.at(i) == name)
        {
            ok= true;
            m_viewNameList.removeAt(i);
            return ok;
        }
    }
    return ok;
}

void DBQueryUI::onMouseRClick()
{
    QTreeWidgetItem *p_Item = treeWidget()->currentItem();
    if(p_Item == NULL)
    {
        return;
    }
    if(isItemInChildren(p_Item,m_pRoot))
    {
        if(!m_pDBView_PopupMenu)
        {
            m_pDBView_PopupMenu = new QMenu(tr("新建"),treeWidget());
            CMDMultiLanguage::instance()->translateWidget(m_pDBView_PopupMenu);
        }
        if(isViewListNode(p_Item))
        {
            if(!m_pDBView_NewAction)
            {
                m_pDBView_NewAction = new QAction(tr("新建视图"),m_pDBView_PopupMenu);
                CMDMultiLanguage::instance()->translateWidget(m_pDBView_NewAction);
                connect(m_pDBView_NewAction,SIGNAL(triggered()),this,SLOT(slotitemActiveNewView()));
            }
            m_pDBView_PopupMenu->addAction(m_pDBView_NewAction);
            if(m_pDBView_Rename)
            {
                m_pDBView_PopupMenu->removeAction(m_pDBView_Rename);
            }
            if(m_pDBView_DeleteAction)
            {
                m_pDBView_PopupMenu->removeAction(m_pDBView_DeleteAction);
            }
            m_pDBView_PopupMenu->exec(QCursor::pos());
        }
        else if(isViewListChildNode(p_Item))
        {
            if(!m_pDBView_Rename)
            {
                m_pDBView_Rename = new QAction(tr("重命名视图"),m_pDBView_PopupMenu);
                CMDMultiLanguage::instance()->translateWidget(m_pDBView_Rename);
                connect(m_pDBView_Rename,SIGNAL(triggered()),this,SLOT(slotitemActiveRenameView()));
            }
            if(!m_pDBView_DeleteAction)
            {
                m_pDBView_DeleteAction = new QAction(tr("删除视图"),m_pDBView_PopupMenu);
                CMDMultiLanguage::instance()->translateWidget(m_pDBView_DeleteAction);
                connect(m_pDBView_DeleteAction,SIGNAL(triggered()),this,SLOT(slotitemActiveDeleteView()));
            }
            if(m_pDBView_NewAction)
            {
                m_pDBView_PopupMenu->removeAction(m_pDBView_NewAction);
            }
            m_pDBView_PopupMenu->addAction(m_pDBView_Rename);
            m_pDBView_PopupMenu->addAction(m_pDBView_DeleteAction);
            m_pDBView_PopupMenu->exec(QCursor::pos());
        }
        else
        {
            return;
        }
    }
}

void DBQueryUI::onMouseDbClick()
{
    QTreeWidgetItem* node = selectNode();
    if(node == NULL)
    {
        return;
    }
    if(node)
    {
        if(isDBSourceNode(node))
        {
            RuntimeSourceDialog dlg;
            dlg.setDBSourcePath(m_DBQueryPath);
            dlg.initUI();
            dlg.exec();
        }
        else if(isViewVarNode(node))
        {
            ViewVarTableDialog dlg;
            dlg.setDBQueryPath(m_DBQueryPath);
            dlg.LoadViewVarTable(m_ViewVarFilePath);
            dlg.creatTableView();
            dlg.exec();
        }
        else if(isItemInChildren(node,m_ViewList))
        {
            QTreeWidgetItem* currentItem = treeWidget()->currentItem();
            if(currentItem)
            {
                if(currentItem->parent())
                {
                    if(currentItem->parent()->parent())
                    {
                        if(currentItem->parent()->parent() == m_ViewList)
                        {
                            DBQ::View *view = currentItem->parent()->data(0,Qt::UserRole).value<DBQ::View*>();
                            if(view)
                            {
                                if(currentItem->text(0) == QString(tr("字段")))
                                {
                                    ViewFieldTableDialog dlg;
                                    dlg.setDataArchivesFilePath(m_DBQueryPath);
                                    dlg.setViewFieldList(view->fields);
                                    dlg.initTableWidget();
                                    if(dlg.exec() == QDialog::Accepted)
                                    {
                                        view->fields.clear();
                                        int size = dlg.getViewFieldList().size();
                                        for(int i = 0 ; i < size; i++)
                                        {
                                            DBQ::ViewField * pField = new DBQ::ViewField;
                                            pField->fieldName = dlg.getViewFieldList().at(i)->fieldName;
                                            pField->alias = dlg.getViewFieldList().at(i)->alias;
                                            pField->type = (DBQ::VIEW_FIELD_TYPE)dlg.getViewFieldList().at(i)->type;
                                            pField->tableName = dlg.getViewFieldList().at(i)->tableName;
                                            pField->description = dlg.getViewFieldList().at(i)->description;
                                            Q_ASSERT(pField);
                                            view->fields.push_back(pField);
                                        }
                                    }
                                }
                                else if(currentItem->text(0) == QString(tr("过滤条件")))
                                {
                                    ViewFilterTreeDialog dlg;
                                    dlg.setDBQueryPath(m_DBQueryPath);
                                    dlg.setView(view);
                                    dlg.setFilter(view->pFilter);
                                    dlg.initFilterTree();

                                    if(dlg.exec() != QDialog::Accepted)
                                        return ;

                                    view->pFilter = dlg.copyFilterTree();
                                    if(dlg.generateFieldViewFile())
                                    {
                                        //生成了字段视图文件 所以要刷新左侧视图的树
                                        emit rebuildViewTreeSignal();
                                        //rebuildViewTree();
                                    }
                                }
                                else if(currentItem->text(0) == QString(tr("默认数据源")))
                                {
                                    ViewDBSourceDialog dlg;
                                    dlg.setDBQueryPath(m_DBQueryPath);
                                    dlg.ViewDBsourceList().clear();
                                    dlg.setViewDBsourceList(view->dbSources);
                                    dlg.initViewDBSource();
                                    if(dlg.exec() == QDialog::Accepted)
                                    {
                                        view->dbSources.clear();
                                        for(int i = 0 ; i < dlg.ViewDBsourceList().count(); i++)
                                        {
                                            view->dbSources.push_back(dlg.ViewDBsourceList().at(i));
                                        }
                                    }
                                }
                                else
                                {
                                    return;
                                }
                                QString errOut = "";
                                QString viewName = m_DBQueryPath + QString("/") + "view_" + view->viewName + ".json";
                                DBQ::View_Write(viewName,DBQ::FILE_TYPE_JSON,view,errOut);
                            }
                        }
                    }
                }
            }
        }
    }
}

void DBQueryUI::slotitemActiveNewView()
{
    NewViewDialog dlg("新建视图");
    dlg.setStudioInterface(m_studioInterface);
    if(dlg.exec() == QDialog::Accepted)
    {
        QTreeWidgetItem *viewFileRoot =  new QTreeWidgetItem;
        QVariant var;
        QString strView = dlg.getViewName();
        if(!isExistSameViewName(strView))
        {
            DBQ::View* view = new DBQ::View;
            *view = * dlg.getView();
            var.setValue(view);
            viewFileRoot->setData(0,Qt::UserRole,var);
            viewFileRoot->setIcon(0,QIcon(":/images/folder.png"));
            viewFileRoot->setText(0,strView);

            QTreeWidgetItem* childField = new QTreeWidgetItem;
            childField->setIcon(0,QIcon(":/images/file.png"));
            childField->setText(0,QString(tr("字段")));
            viewFileRoot->addChild(childField);

            QTreeWidgetItem* childFilter = new QTreeWidgetItem;
            childFilter->setIcon(0,QIcon(":/images/file.png"));
            childFilter->setText(0,QString(tr("过滤条件")));
            viewFileRoot->addChild(childFilter);

            QTreeWidgetItem* childDefaultDBSource = new QTreeWidgetItem;
            childDefaultDBSource->setIcon(0,QIcon(":/images/file.png"));
            childDefaultDBSource->setText(0,QString(tr("默认数据源")));
            viewFileRoot->addChild(childDefaultDBSource);

            m_ViewList->addChild(viewFileRoot);
            m_viewNameList.push_back(strView);
        }
        else
        {
            QString strError = QString(tr("视图名称已经存在!"));
            QMessageBox box;
            box.setText(strError);
            box.setWindowTitle(tr("警告!"));
            box.setStandardButtons(QMessageBox::Ok);
            CMDMultiLanguage::instance()->translateWidget(&box);
            box.exec();
            return;
        }
    }
}

void DBQueryUI::slotitemActiveRenameView()
{
    QTreeWidget *pQTreeWidget = treeWidget();
    if(pQTreeWidget)
    {
        QTreeWidgetItem *p_Item = pQTreeWidget->currentItem();
        QString oldViewName = p_Item->text(0);
        if(p_Item)
        {
            DBQ::View *view = p_Item->data(0,Qt::UserRole).value<DBQ::View*>();
            if(view)
            {
                if(isItemInChildren(p_Item,m_ViewList))
                {
                    NewViewDialog dlg("重命名视图");
                    dlg.setStudioInterface(m_studioInterface);
                    if(dlg.exec() == QDialog::Accepted)
                    {
                        QString newViewName = dlg.getViewName();

                        if(oldViewName == newViewName)
                        {
                            return;
                        }

                        if(!isExistSameViewName(newViewName))
                        {
                            m_viewNameList.removeOne(oldViewName);
                            p_Item->setText(0,newViewName);
                            m_viewNameList.push_back(newViewName);
                            DBQ::View_Rename(m_DBQueryPath,oldViewName,newViewName);
                        }
                        else
                        {
                            QMessageBox::information(NULL,"警告",tr("当前视图变量名称已存在!"));
                            return;
                        }
                    }
                }
            }
        }
    }
}

void DBQueryUI::slotitemActiveDeleteView()
{
    QString absolutefilepath = "";
    QTreeWidget *pQTreeWidget = treeWidget();
    if(pQTreeWidget)
    {
        QTreeWidgetItem *p_Item = pQTreeWidget->currentItem();

        if(isItemInChildren(p_Item,m_ViewList))
        {
            QString currentitem_name = p_Item->data(0,Qt::DisplayRole).toString();
            currentitem_name = QString("view_") + currentitem_name + QString(".json");
            absolutefilepath = m_DBQueryPath+QString("/")+currentitem_name;
            QString content = p_Item->text(0);
            QString str1 = QString(tr("确认要删除视图"));
            QString str2 = content;
            QString str3 = QString(tr("吗?"));
            QString questionString = str1 + str2 + str3;
            QMessageBox box;
            questionString = CMDMultiLanguage::instance()->value(str1)+str2+CMDMultiLanguage::instance()->value(str3);
            box.setText(questionString);
            box.setWindowTitle(tr("询问!"));
            box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
            CMDMultiLanguage::instance()->translateWidget(&box);
            if(box.exec() == QMessageBox::Yes)
            {
                QFileInfo fileinfo;
                fileinfo.setFile(m_DBQueryPath,currentitem_name);
                if(!fileinfo.exists())
                {
                    return;
                }
                QFile file;
                file.setFileName(absolutefilepath);
                if(file.isOpen())
                {
                    QString strError = QString(tr("视图文件已经打开,禁止删除!"));
                    QMessageBox box;
                    box.setText(strError);
                    box.setWindowTitle(tr("警告!"));
                    box.setStandardButtons(QMessageBox::Ok);
                    CMDMultiLanguage::instance()->translateWidget(&box);
                    box.exec();
                    return;
                }
                if(removeCurrentViewName((p_Item->data(0,Qt::DisplayRole).toString())))
                {
                    delete p_Item->parent()->takeChild(pQTreeWidget->currentIndex().row());
                    if(file.remove(absolutefilepath))
                    {
                        qDebug()<<"删除视图文件成功!";
                    }
                }
                return;
            }
        }
    }
}

void DBQueryUI::changeLanguage()
{

}
