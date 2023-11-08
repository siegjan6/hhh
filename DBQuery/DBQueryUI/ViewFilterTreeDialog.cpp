#include "ViewFilterTreeDialog.h"
#include "ui_ViewFilterTreeDialog.h"
#include "ViewFilterFieldDialog.h"
#include "ViewFilterAndOrNodeDialog.h"
#include <QMessageBox>
#include <QLabel>
#include <QDebug>

class QTreeFilterTreeTraverse : public DBQ::IFilterTreeTraverse
{
public:
    QTreeFilterTreeTraverse(FilterTreeWidget * pTreeWidget)
        :m_pTreeWidget(pTreeWidget)
    {}
    void * VisitRootNode(DBQ::ViewFilterNode * pFilter, void * parentNode);
    void * VisitAndNode(DBQ::ViewFilterNode * pFilter, void * parentNode);
    void * VisitOrNode(DBQ::ViewFilterNode * pFilter, void * parentNode);
    void * VisitLeafNode(DBQ::ViewFilterNode * pFilter, void * parentNode);
protected:
    FilterTreeWidget * m_pTreeWidget;
};

void * QTreeFilterTreeTraverse::VisitRootNode(DBQ::ViewFilterNode * pFilter, void * parentNode)
{
    Q_UNUSED(pFilter);
    return parentNode;
}

void * QTreeFilterTreeTraverse::VisitAndNode(DBQ::ViewFilterNode * pFilter, void * parentNode)
{
    QTreeWidgetItem * parentItem = (QTreeWidgetItem *)parentNode;

    QTreeWidgetItem *pAndItem =  new QTreeWidgetItem;
    QVariant var;
    var.setValue(pFilter);
    pAndItem->setData(0, Qt::UserRole, QVariant(var));
    pAndItem->setText(0,"and");
    parentItem->addChild(pAndItem);

    return pAndItem;
}

void * QTreeFilterTreeTraverse::VisitOrNode(DBQ::ViewFilterNode * pFilter, void * parentNode)
{
    QTreeWidgetItem * parentItem = (QTreeWidgetItem *)parentNode;

    QTreeWidgetItem *pOrItem =  new QTreeWidgetItem;
    QVariant var;
    var.setValue(pFilter);
    pOrItem->setData(0, Qt::UserRole, QVariant(var));
    pOrItem->setText(0,"or");
    parentItem->addChild(pOrItem);

    return pOrItem;
}

void * QTreeFilterTreeTraverse::VisitLeafNode(DBQ::ViewFilterNode * pFilter, void * parentNode)
{
    QTreeWidgetItem * parentItem = (QTreeWidgetItem *)parentNode;

    QTreeWidgetItem *pItem =  new QTreeWidgetItem;
    QVariant var;
    var.setValue(pFilter);
    pItem->setData(0, Qt::UserRole, var);
    parentItem->addChild(pItem);

    m_pTreeWidget->setItemTextWithFormat(pItem, pFilter);
    return pItem;
}

ViewFilterTreeDialog::ViewFilterTreeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewFilterTreeDialog),
    m_filter(new DBQ::ViewFilterNode),
    m_pRoot(NULL),
    m_Filter_Menu(NULL),
    m_addAndNode_Action(NULL),
    m_addOrNode_Action(NULL),
    m_addFilterNode_Action(NULL),
    m_ModifyNode_Action(NULL),
    m_DeleteNode_Action(NULL),
    m_DBQueryPath(""),
    m_pView(0),
    m_hasRootNode(false)

{
    ui->setupUi(this);
//    ui->treeWidget->header()->setMinimumSectionSize(500);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_showMenu(QPoint)));
    m_filter->type = DBQ::VIEW_FILTER_NODE_TYPE_ROOT;

    QString strTreeStyle = "QTreeView::branch:has-siblings:!adjoins-item {\
                border-image: url(:/res/stylesheet-vline.png) 0;\
            }\
            QTreeView::branch:has-siblings:adjoins-item {\
                border-image: url(:/res/stylesheet-branch-more.png) 0;\
            }\
            QTreeView::branch:!has-children:!has-siblings:adjoins-item {\
                border-image: url(:/res/stylesheet-branch-end.png) 0;\
            }\
            QTreeView::branch:has-children:!has-siblings:closed,\
            QTreeView::branch:closed:has-children:has-siblings {\
                    border-image: none;\
                    image: url(:/res/stylesheet-branch-closed.png);\
            }\
            QTreeView::branch:open:has-children:!has-siblings,\
            QTreeView::branch:open:has-children:has-siblings  {\
                    border-image: none;\
                    image: url(:/res/stylesheet-branch-open.png);\
            }";

    ui->treeWidget->setStyleSheet(strTreeStyle);

    QWidget::setTabOrder(ui->treeWidget,ui->pbt_Up);
    QWidget::setTabOrder(ui->pbt_Up,ui->pbt_Down);
    QWidget::setTabOrder(ui->pbt_Down,ui->pbt_Left);
    QWidget::setTabOrder(ui->pbt_Left,ui->pbt_Right);
    QWidget::setTabOrder(ui->pbt_Right,ui->pbt_Confirm);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->treeWidget);

}

ViewFilterTreeDialog::~ViewFilterTreeDialog()
{
    delete ui;
}

DBQ::ViewFilterNode *ViewFilterTreeDialog::copyFilterTree()
{
    return DBQ::FilterTree_Copy(m_filter);
}

void ViewFilterTreeDialog::setFilter(DBQ::ViewFilterNode *filter)
{
    if(filter)
    {
        DBQ::FilterTree_Delete(0, m_filter);
        m_filter = DBQ::FilterTree_Copy(filter);
    }
}

//! 创建过滤条件树结构
void ViewFilterTreeDialog::initFilterTree()
{
    ui->treeWidget->setHeaderHidden(true);

    QVariant var;
    var.setValue(m_filter);
    m_pRoot = new QTreeWidgetItem(ui->treeWidget, QStringList(QString("过滤条件")));
    m_pRoot->setData(0, Qt::UserRole, QVariant(var));

    if(NULL == m_filter)
        return;

    QTreeFilterTreeTraverse traverse(ui->treeWidget);
    DBQ::FilterTree_Traverse(m_filter, m_pRoot, &traverse);
    ui->treeWidget->expandAll();
}

void ViewFilterTreeDialog::setDBQueryPath(const QString &DBQueryPath)
{
    m_DBQueryPath = DBQueryPath;
    ui->treeWidget->setDBQueryPath(m_DBQueryPath);
}

void ViewFilterTreeDialog::setView(DBQ::View * pView)
{
    m_pView = pView;
    ui->treeWidget->setDataView(m_pView);
}

//!如果存在字段视图则生成对应的视图文件
bool ViewFilterTreeDialog::generateFieldViewFile()
{
    bool bExistView = false;
    QString errStr;
    foreach(DBQ::View * pView, m_FieldViewList)
    {
        bExistView = true;
        DBQ::View_WriteToDir(m_DBQueryPath, pView->viewName, pView, errStr);
        delete pView;
    }
    m_FieldViewList.clear();
    return bExistView;
}

void ViewFilterTreeDialog::newAndOrFilterNode(DBQ::VIEW_FILTER_NODE_TYPE nodeType, DBQ::ViewFilterNode * currentFilter, QTreeWidgetItem* currentItem)
{
    if(!currentItem)
        return ; //TODO:log

    if(nodeType!=DBQ::VIEW_FILTER_NODE_TYPE_AND
       &&nodeType!=DBQ::VIEW_FILTER_NODE_TYPE_OR)
        return ; //TODO:log

    DBQ::ViewFilterNode *filter = new DBQ::ViewFilterNode;
    filter->type = nodeType;
    QVariant var;
    var.setValue(filter);

    QTreeWidgetItem *pItem = new QTreeWidgetItem;
    pItem->setData(0, Qt::UserRole, QVariant(var));
    if(DBQ::VIEW_FILTER_NODE_TYPE_AND == nodeType)
        pItem->setText(0,"and");
    else
        pItem->setText(0, "or");

    currentItem->addChild(pItem);
    DBQ::FilterTree_Add(currentFilter, filter);
}

void ViewFilterTreeDialog::slot_showMenu(const QPoint &pos)
{
    QTreeWidgetItem* currentItem = ui->treeWidget->currentItem();
    if(!currentItem)
        return ; //TODO:log

    if(!m_Filter_Menu)
    {
        m_Filter_Menu = new QMenu(tr("新建"),ui->treeWidget);

        if(!m_addAndNode_Action)
        {
            m_addAndNode_Action = new QAction(tr("添加And节点"),ui->treeWidget);
            connect(m_addAndNode_Action,SIGNAL(triggered()),this,SLOT(slot_addAndNodeAction()));
            m_Filter_Menu->addAction(m_addAndNode_Action);
        }
        if(!m_addOrNode_Action)
        {
            m_addOrNode_Action = new QAction(tr("添加Or节点"),ui->treeWidget);
            connect(m_addOrNode_Action,SIGNAL(triggered()),this,SLOT(slot_addOrNodeAction()));
            m_Filter_Menu->addAction(m_addOrNode_Action);
        }
        if(!m_addFilterNode_Action)
        {
            m_addFilterNode_Action = new QAction(tr("添加过滤项"),ui->treeWidget);
            connect(m_addFilterNode_Action,SIGNAL(triggered()),this,SLOT(slot_addFilterNodeAction()));
            m_Filter_Menu->addAction(m_addFilterNode_Action);
        }
        if(!m_ModifyNode_Action)
        {
            m_ModifyNode_Action = new QAction(tr("修改"),ui->treeWidget);
            connect(m_ModifyNode_Action,SIGNAL(triggered()),this,SLOT(slot_modifyNodeAction()));
            m_Filter_Menu->addAction(m_ModifyNode_Action);
        }
        if(!m_DeleteNode_Action)
        {
            m_DeleteNode_Action = new QAction(tr("删除"),ui->treeWidget);
            connect(m_DeleteNode_Action,SIGNAL(triggered()),this,SLOT(slot_deleteNodeAction()));
            m_Filter_Menu->addAction(m_DeleteNode_Action);
        }
    }

    m_Filter_Menu->exec(ui->treeWidget->mapToGlobal(pos));
}

//nodeType 要添加的节点的类型 只能是And或Or节点
void ViewFilterTreeDialog::addAndOrNode(DBQ::VIEW_FILTER_NODE_TYPE nodeType)
{
    QTreeWidgetItem* currentItem = ui->treeWidget->currentItem();
    if(!currentItem)
        return ; //TODO:log

    //过滤树的根节点 对应"过滤条件"节点
    //根节点下要么有且只有一个And/or节点 要么有1个叶子节点
    DBQ::ViewFilterNode *currentFilter = currentItem->data(0, Qt::UserRole).value<DBQ::ViewFilterNode*>();
    if(!currentFilter)
        return ;//TODO:log 不可能出现的情况

    if(!FilterTree_CheckType(currentFilter))
            return ; //TODO:log 不可能出现的情况

    //currentFilter为叶子节点时
    if(DBQ::VIEW_FILTER_NODE_TYPE_LEAF == currentFilter->type)
    {
        QMessageBox msgBox;
        msgBox.setText("不能在过滤项上添加节点");
        msgBox.exec();
        return ;
    }

    if(DBQ::VIEW_FILTER_NODE_TYPE_ROOT == currentFilter->type)
    {
        bool bExistAndOrNode = false;
        for(int i = 0; i < currentFilter->child.size(); i++)
        {
            if(m_filter->child.at(i)->type == DBQ::VIEW_FILTER_NODE_TYPE_AND
               ||m_filter->child.at(i)->type == DBQ::VIEW_FILTER_NODE_TYPE_OR)
            {
                bExistAndOrNode = true;
                break;
            }
        }

        if(bExistAndOrNode) //根结点下已经存在了一个and/or节点 不能再添加节点了
        {
            QMessageBox msgBox;
            msgBox.setText("根节点下只能有一个And/Or节点");
            msgBox.exec();
            return ;
        }
    }

    //currentFilter为root/and/or节点时
    //nodeType一定为and或or节点类型
    newAndOrFilterNode(nodeType,currentFilter,currentItem);
}

DBQ::ViewFilterNode* ViewFilterTreeDialog::generateFilterItemNode()
{
    ViewFilterFieldDialog dlg;
    dlg.setDBQueryPath(m_DBQueryPath);
    dlg.setView(m_pView);

    if(dlg.exec() != QDialog::Accepted) //取消
        return 0;

    //DBQ::ViewFilterNode *filterNode = new DBQ::ViewFilterNode;      //! 视图过滤项指针
    DBQ::ViewFilterNode * filterNode = dlg.getResult();
    if(!filterNode)
    {
        QMessageBox msg;
        msg.setText("生成过滤项失败");
        msg.exec();
        return 0;
    }

    //filterNode->type = DBQ::VIEW_FILTER_NODE_TYPE_LEAF;
    //filterNode->leftType = dlg.getLeftValueType();
    //filterNode->leftValue =dlg.getLeftValue_FieldName();
    //filterNode->compare = dlg.getCompare();
    //filterNode->rightValue = dlg.getRightValue();


    //如果过滤项的右值是视图变量 且视图变量的类型为字段
    //则自动生成一个视图
    QString viewVarName = DBQ::FilterTree_GetViewVarNameFromRightValue(filterNode);
    if(!viewVarName.isEmpty())
    {
        DBQ::ViewVariable * pVar = DBQ::ViewVariable_GetVarFromDir(m_DBQueryPath, viewVarName);
        if(!pVar)
        {
            //TODO:log
            return filterNode;
        }

        if(DBQ::VIEW_VARIABLE_TYPE_FIELD == pVar->type)
        {
            generateFieldView(m_pView, pVar);
            //通知刷新左侧视图列表
        }
    }

    return filterNode;
}

bool ViewFilterTreeDialog::createAndInseterQTreeItemToTree(QTreeWidgetItem * parentItem, DBQ::ViewFilterNode * pNode)
{
    if(0 == parentItem || 0==pNode)
        return false;

    QVariant var;
    var.setValue(pNode);

    QTreeWidgetItem *pItem = new QTreeWidgetItem;
    pItem->setData(0, Qt::UserRole, QVariant(var));

    parentItem->addChild(pItem);
    ui->treeWidget->setItemTextWithFormat(pItem, pNode);

    return true;
}

void ViewFilterTreeDialog::slot_addAndNodeAction()
{
    addAndOrNode(DBQ::VIEW_FILTER_NODE_TYPE_AND);
}

void ViewFilterTreeDialog::slot_addOrNodeAction()
{
    addAndOrNode(DBQ::VIEW_FILTER_NODE_TYPE_OR);
}

//添加过滤项
void ViewFilterTreeDialog::slot_addFilterNodeAction()
{
    QTreeWidgetItem* currentItem = ui->treeWidget->currentItem();
    if(!currentItem)
        return ; //TODO:log

    DBQ::ViewFilterNode *currentFilter = currentItem->data(0, Qt::UserRole).value<DBQ::ViewFilterNode*>();
    if(!currentFilter)
        return ;//TODO:log 不可能出现的情况

    //如果是根节点
    //根节点下要么有且只有一个And/or节点 要么有1个叶子节点
    //当根节点下有一个叶子节点时 而要添加的节点是叶子节点
    //则创建一个and节点 并将两个叶子节点移动到此and节点下
    if(DBQ::VIEW_FILTER_NODE_TYPE_ROOT == currentFilter->type)
    {
        bool bExistAndOrNode = false;
        for(int i = 0; i < currentFilter->child.size(); i++)
        {
            if(currentFilter->child.at(i)->type == DBQ::VIEW_FILTER_NODE_TYPE_AND
               ||currentFilter->child.at(i)->type == DBQ::VIEW_FILTER_NODE_TYPE_OR)
            {
                bExistAndOrNode = true;
                break;
            }
        }

        if(bExistAndOrNode)
        {
            QMessageBox msg;
            msg.setText("请在And/Or节点中添加过滤项");
            msg.exec();
            return ;
        }

        //运行到这里时 currentFilter要么没有子节点 要么有子节点且子节点的类型为叶子节点
        DBQ::ViewFilterNode * pNode = generateFilterItemNode();
        if(!pNode) //用户取消了添加过滤项的操作
            return ;

        if(currentFilter->child.size() == 0) //没有子节点
        {
            DBQ::FilterTree_Add(currentFilter, pNode);
            createAndInseterQTreeItemToTree(currentItem, pNode);
            return ;
        }

        //有子节点且子节点的类型为叶子节点
        //新建一个And节点 将根节点下的叶子节点移动到此And节点下.
        DBQ::ViewFilterNode * andNode = new DBQ::ViewFilterNode;
        andNode->type = DBQ::VIEW_FILTER_NODE_TYPE_AND;

        for(int i = 0; i < currentFilter->child.size(); i++)
        {
            DBQ::FilterTree_Move(andNode, currentFilter, currentFilter->child[i]);
        }
        DBQ::FilterTree_Add(andNode, pNode);
        DBQ::FilterTree_Add(currentFilter, andNode);

        //根据FilterTree重新创建 QTree
        recreateQTree(currentFilter); //TODO:recreateQTree(FilterTree_Copy(currentFilter));
        return ;
    }

    //如果是and/or节点 则将过滤项节点添加到此and/or节点中
    if(DBQ::VIEW_FILTER_NODE_TYPE_AND == currentFilter->type
       ||DBQ::VIEW_FILTER_NODE_TYPE_OR == currentFilter->type)
    {
        DBQ::ViewFilterNode * pNode = generateFilterItemNode();
        if(!pNode) //用户取消了添加过滤项的操作
            return ;

        DBQ::FilterTree_Add(currentFilter, pNode);
        createAndInseterQTreeItemToTree(currentItem, pNode);
        return ;
    }

    //如果是子节点 则禁止添加
    if(DBQ::VIEW_FILTER_NODE_TYPE_LEAF == currentFilter->type)
    {
        QMessageBox msg;
        msg.setText("不能在过滤项上添加过滤项");
        msg.exec();
        return ;
    }

    //TODO:不可能运行到此处 要加log
}

//! 由FilterTree重建QtTree
void ViewFilterTreeDialog::recreateQTree(DBQ::ViewFilterNode *pFilter)
{
    if(NULL == pFilter)
        return;

    //![1] 重置
    ui->treeWidget->clear(); //TODO:会自动释放m_pRoot么
    //TODO:释放之前的FilterTree
    ui->treeWidget->setHeaderHidden(true);

    QVariant var;
    var.setValue(pFilter);
    m_pRoot = new QTreeWidgetItem(ui->treeWidget, QStringList(QString("过滤条件")));
    m_pRoot->setData(0, Qt::UserRole, QVariant(var));

    QTreeFilterTreeTraverse traverse(ui->treeWidget);
    DBQ::FilterTree_Traverse(pFilter, m_pRoot, &traverse);
    m_filter = pFilter;
    ui->treeWidget->expandAll();
}

void ViewFilterTreeDialog::slot_modifyNodeAction()
{
    QTreeWidgetItem * currentItem = ui->treeWidget->currentItem();
    if(0 == currentItem)
        return ; //TODO:log

    //根节点不允许修改 即"过滤条件节点"
    if(!currentItem->parent())
    {
        QMessageBox msgBox;
        msgBox.setText("过滤条件节点不能被修改");
        msgBox.exec();
        return ;
    }

    DBQ::ViewFilterNode * currentFilter = currentItem->data(0, Qt::UserRole).value<DBQ::ViewFilterNode*>();
    if(0 == currentFilter)
        return ; //TODO:log

    //![1]修改and/or节点
    if(DBQ::VIEW_FILTER_NODE_TYPE_AND == currentFilter->type
        ||DBQ::VIEW_FILTER_NODE_TYPE_OR == currentFilter->type)
    {
        //![1.1]取消修改
        ViewFilterAndOrNodeDialog dlg;
        dlg.setType(currentFilter->type);
        if(dlg.exec() != QDialog::Accepted)
            return ; //用户取消修改

        //![1.2]未修改
        DBQ::VIEW_FILTER_NODE_TYPE type = dlg.getType();
        if(type ==currentFilter->type)
            return ;//没有修改 不用更新界面

        //![1.3]修改了
        if(DBQ::VIEW_FILTER_NODE_TYPE_AND == type)
            currentItem->setText(0,"and");
        else if(DBQ::VIEW_FILTER_NODE_TYPE_OR == type)
            currentItem->setText(0, "or");
        else
        {
            //TODO:log 不可能出现
            return ;
        }

        currentFilter->type = type;
    }
    //![2]修改过滤项
    if(DBQ::VIEW_FILTER_NODE_TYPE_LEAF == currentFilter->type)
    {
        ui->treeWidget->updateFilterItemNode(currentItem, currentFilter);
        return ;
    }

    //TODO:不可能运行到此处
}

void ViewFilterTreeDialog::slot_deleteNodeAction()
{
    QTreeWidgetItem * currentItem = ui->treeWidget->currentItem();
    if(0 == currentItem)
        return ; //TODO:log

    //根节点不允许删除 即"过滤条件节点"
    if(!currentItem->parent())
    {
        QMessageBox msgBox;
        msgBox.setText("过滤条件节点不能删除");
        msgBox.exec();
        return ;
    }

    DBQ::ViewFilterNode * currentFilter = currentItem->data(0, Qt::UserRole).value<DBQ::ViewFilterNode*>();
    if(0 == currentFilter)
        return ; //TODO:log


    DBQ::ViewFilterNode *  parentFilter = currentItem->parent()->data(0, Qt::UserRole).value<DBQ::ViewFilterNode*>();
    DBQ::FilterTree_Delete(parentFilter, currentFilter);

    //重新建Qt Tree
    recreateQTree(m_filter);
}

void ViewFilterTreeDialog::on_pbt_Confirm_clicked()
{
    this->accept();
}

void ViewFilterTreeDialog::on_pbt_Cancel_clicked()
{
    this->reject();
}

//父节点不存在时 parentNode为0
bool ViewFilterTreeDialog::getParentAndCurrentNode(DBQ::ViewFilterNode *& parentNode, DBQ::ViewFilterNode *& currentNode)
{
    //得到当前选中的item
    QTreeWidgetItem * currentItem = ui->treeWidget->currentItem();
    if(0 == currentItem)
        return false; //TODO:log

    //得到item对应的filterNode
    currentNode = currentItem->data(0, Qt::UserRole).value<DBQ::ViewFilterNode*>();
    if(!currentNode)
        return false;//TODO:log 不可能出现的情况

    //得到filterNode的父节点
    if(!currentItem->parent())
    {
        parentNode = 0;
        return true;
    }

    parentNode = currentItem->parent()->data(0, Qt::UserRole).value<DBQ::ViewFilterNode*>();
    if(!parentNode)
        return false;//TODO:log 不可能出现的情况

    return true;
}

bool ViewFilterTreeDialog::generateFieldView(DBQ::View * pView, DBQ::ViewVariable * pVar)
{
    if(0 == pView || 0 == pVar)
        return false;

    DBQ::View * pFieldView = new DBQ::View(); //DBQ::View_Create(pView->viewName + "."+pVar->name);
    pFieldView->viewName = pView->viewName + "."+pVar->name;
    DBQ::ViewField viewField;
    viewField.tableName = DBQ::ViewVariable_GetTableName(pVar);
    viewField.fieldName = DBQ::ViewVariable_GetFieldName(pVar);
    viewField.alias = pVar->name;
    viewField.type = pVar->valueType;

    DBQ::View_AddField(pFieldView, &viewField);
    DBQ::View_ReplaceDBSources(pFieldView, pView);

    m_FieldViewList.append(pFieldView);

    return true;
}

void ViewFilterTreeDialog::selectTreeNode(DBQ::ViewFilterNode * currentNode)
{
    if(!currentNode)
        return ;

    DBQ::ViewFilterNode * pNode = 0;
    QTreeWidgetItemIterator ite(ui->treeWidget);

    while (*ite)
    {
        pNode = (*ite)->data(0, Qt::UserRole).value<DBQ::ViewFilterNode*>();
        if(pNode == currentNode) //找到了
        {
            ui->treeWidget->setCurrentItem(*ite);
            return ;
        }
        ++ite;
    }
}

//上移节点  同级子节点上下移动 不能从父节点中移出
void ViewFilterTreeDialog::on_pbt_Up_clicked()
{
    DBQ::ViewFilterNode * parentNode = 0;
    DBQ::ViewFilterNode * currentNode = 0;
    if(!getParentAndCurrentNode(parentNode, currentNode))
        return ;

    if(0 == parentNode)
    {
        QMessageBox msg;
        msg.setText("根节点,无须移动.");
        msg.exec();
        return ;
    }

    DBQ::FilterTree_Up(parentNode, currentNode);
    recreateQTree(m_filter);
    selectTreeNode(currentNode);//重新创建树后 当前选中项会消失 重新选中
}

//下移节点 同级子节点上下移动 不能从父节点中移出
void ViewFilterTreeDialog::on_pbt_Down_clicked()
{
    DBQ::ViewFilterNode * parentNode = 0;
    DBQ::ViewFilterNode * currentNode = 0;
    if(!getParentAndCurrentNode(parentNode, currentNode))
        return ;

    if(0 == parentNode)
    {
        QMessageBox msg;
        msg.setText("根节点,无须移动.");
        msg.exec();
        return ;
    }

    DBQ::FilterTree_Down(parentNode, currentNode);
    recreateQTree(m_filter);
    selectTreeNode(currentNode);
}

//节点升级
void ViewFilterTreeDialog::on_pbt_Left_clicked()
{
    DBQ::ViewFilterNode * parentNode = 0;
    DBQ::ViewFilterNode * currentNode = 0;
    if(!getParentAndCurrentNode(parentNode, currentNode))
        return ;

    if(0 == parentNode)
    {
        QMessageBox msg;
        msg.setText("根节点,无须移动.");
        msg.exec();
        return ;
    }

    //找到祖父节点
    QTreeWidgetItem * currentItem = ui->treeWidget->currentItem();
    if(!currentItem)
        return ; //TODO:log
    QTreeWidgetItem * parentItem = currentItem->parent();
    if(!parentItem)
        return ;//TODO:log
    QTreeWidgetItem * grandfatherItem = parentItem->parent();
    if(!grandfatherItem)
        return ;

    //如果grandfatherItem为根节点 则不能升级节点层次
    //因为根节点只能有1个and/or节点
    DBQ::ViewFilterNode * grandfatherNode = grandfatherItem->data(0, Qt::UserRole).value<DBQ::ViewFilterNode*>();
    if(!grandfatherNode)
        return ;//TODO:log
    if(DBQ::VIEW_FILTER_NODE_TYPE_ROOT == grandfatherNode->type)
    {
        QMessageBox msg;
        msg.setText("根节点下只能有一个And/Or节点");
        msg.exec();
        return ;
    }

    //TODO:日后为FilterNode添加上父节点 方便子节点向上访问父节点
    DBQ::FilterTree_Upgrade(grandfatherNode, parentNode, currentNode);
    recreateQTree(m_filter);
    selectTreeNode(currentNode);
}

//节点降级
void ViewFilterTreeDialog::on_pbt_Right_clicked()
{
    DBQ::ViewFilterNode * parentNode = 0;
    DBQ::ViewFilterNode * currentNode = 0;
    if(!getParentAndCurrentNode(parentNode, currentNode))
        return ;

    if(0 == parentNode)
    {
        QMessageBox msg;
        msg.setText("根节点,无须移动.");
        msg.exec();
        return ;
    }

    DBQ::FilterTree_Degrade(parentNode, currentNode); //TODO:日后为FilterNode添加上父节点 方便子节点向上访问父节点
    recreateQTree(m_filter);
    selectTreeNode(currentNode);
}
