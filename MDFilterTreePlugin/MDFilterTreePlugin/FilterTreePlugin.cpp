#include "FilterTreePlugin.h"

#include "MDFilterTreePlugin.h"
#include "MDFilterConditionSetting.h"

#include "../../include/interface/Common/IMDDataType.h"
#include "IMDDataTable.h"
#include "IMDDataField.h"
#include "MDDataSourceFactory.h"
#include "MDMultiLanguage.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QAction>
#include <QMenu>
#include <QDateTime>
#include <QTimer>
#include <QFile>
#include <QDataStream>
#include <QCloseEvent>
#include <QDebug>

CFilterTreePlugin::CFilterTreePlugin(QDialog *parent) :
    QDialog(parent)
{
    initData();
    createUI();
    createMenu();
    initLanguage();
    signalsBindSlots();
}

CFilterTreePlugin::CFilterTreePlugin(IMDDataTable *pTable, QDialog *parent) :
    QDialog(parent)
{
    initData();
    createUI();
    createMenu();
    initLanguage();
    signalsBindSlots();
    initData(pTable);
}

CFilterTreePlugin::~CFilterTreePlugin()
{
    if (m_pMulLnaguageTool)
    {
        delete m_pMulLnaguageTool;
        m_pMulLnaguageTool = NULL;
    }

    m_pConditionTree->clear();
    m_pGroupTree->clear();
}

bool CFilterTreePlugin::save(CMDFilterTreeData *pFilterData)
{
    if (NULL == m_pConditionTree->topLevelItem(0)
            || 0 == m_pConditionTree->topLevelItemCount())
    {
        return false;
    }

    pFilterData->setCondition(getFilterCondition());
    CMDFilterTreeData *TmpData = new CMDFilterTreeData();

    QTreeWidgetItem *pItem = m_pConditionTree->topLevelItem(0);
    CMDFilterTreeDataItem *pFilterItem = pItem->data(0, Qt::UserRole).value<CMDFilterTreeDataItem*>();
    if (NULL == pFilterItem)
    {
        return false;
    }
//    pDataItem->setName(m_pConditionTree->topLevelItem(0)->text(0));
    pFilterItem->setCount(pItem->childCount());
    TmpData->insert(pFilterItem->clone());

    save(pItem, TmpData);

    pFilterData->insert(TmpData);

    return true;
}

bool CFilterTreePlugin::load(CMDFilterTreeData *pFilterData)
{
    if (NULL == pFilterData)
    {
        return false;
    }

    if (0 == pFilterData->count())
    {
        return false;
    }

    m_pConditionTree->clear();

    CMDFilterTreeDataItem *pFilterItem = pFilterData->getItem(0);

    QString strText = pFilterItem->getName();
    if (1 == pFilterData->count())
    {
        strText = CMDMultiLanguage::instance()->value(pFilterItem->getDescripe());
        if (strText.isEmpty())
        {
            strText = CMDMultiLanguage::instance()->value(pFilterItem->getName());
        }

        strText += " " + pFilterItem->getOperator() + " " + pFilterItem->getValue();
    }

    QTreeWidgetItem *pItem = new QTreeWidgetItem(m_pConditionTree);
    pItem->setText(0, strText);
    QVariant var;
    var.setValue(pFilterItem);
    pItem->setData(0, Qt::UserRole, var);
    pItem->setExpanded(true);
    m_pConditionTree->insertTopLevelItem(0, pItem);

    pFilterData->setPosition(1);
    load(pItem, pFilterData, pFilterItem->count());

    return true;
}           

void CFilterTreePlugin::load(QTreeWidgetItem *root, CMDFilterTreeData *pFilterData, int count)
{
    QString strText = "";
    for (int i = 0; i < count; ++i)
    {
        CMDFilterTreeDataItem *pFilterItem = pFilterData->getItem(pFilterData->getPosition());
        if (NULL == pFilterItem)
        {
            continue;
        }

        int childCount = pFilterItem->count();
        pFilterData->setPosition(pFilterData->getPosition() + 1);

        QTreeWidgetItem *pItem = new QTreeWidgetItem(root);
        QVariant var;
        var.setValue (pFilterItem);
        pItem->setData(0, Qt::UserRole, var);
        root->addChild(pItem);

        strText = CMDMultiLanguage::instance()->value(pFilterItem->getDescripe());
        if (strText.isEmpty())
        {
            strText = CMDMultiLanguage::instance()->value(pFilterItem->getName());
        }

        if (0 >= childCount)
        {
            strText += " " + pFilterItem->getOperator() + " " + pFilterItem->getValue();
            pItem->setText(0, strText);
//            createChildItem(root, pFilterItem);
        }
        else
        {       
            pItem->setText(0, strText);
            pItem->setExpanded(true);
            load(pItem, pFilterData, childCount);
        }
    }
}

void CFilterTreePlugin::initData(IMDDataTable *pTable)
{
    if (NULL == pTable)
    {
        return;
    }

    m_pGroupTree->clear();
    m_pConditionTree->clear();
    QTreeWidgetItem *pRootItem = createGroupTreeRoot(pTable->name());
    IMDDataField *pField = NULL;
    for (int i = 0; i < pTable->dataFieldCount(); ++i)
    {
        pField = pTable->getDataField(i);
        if (nullptr == pField)
        {
            continue;
        }

        QVariant vExtType = pField->ext("type");
        if(!vExtType.isNull() && vExtType.isValid())
        {
            if ("virtual" == vExtType.toString())
            {
                continue;
            }
        }

        createChildItem(pRootItem, pField);
    }

    if (0 >= m_pGroupTree->topLevelItem(0)->childCount())
    {
        delete pRootItem;
        pRootItem = NULL;
        m_pGroupTree->clear();
    }
}

void CFilterTreePlugin::initData(const QString &tableName, QList<CMDFilterTreeDataItem*> &customFieldList)
{
    if (0 == customFieldList.size())
    {
        return;
    }

    m_pGroupTree->clear();
    m_pConditionTree->clear();

    QTreeWidgetItem *pRootItem = createGroupTreeRoot(tableName);
    pRootItem->setExpanded(true);
    CMDFilterTreeDataItem *pCustomField = NULL;
    QTreeWidgetItem *pItem = NULL;
    for (int i = 0; i < customFieldList.size(); ++i)
    {
        pCustomField = customFieldList.at(i);
        if (NULL == pCustomField)
        {
            continue;
        }

        QVariant var;
        var.setValue(pCustomField);

        pItem = new QTreeWidgetItem(pRootItem);
        QString strText  = pCustomField->getDescripe();
        if (strText.isEmpty())
        {
            strText = pCustomField->getName();
        }
        pItem->setText(0, CMDMultiLanguage::instance()->value(strText));
        pItem->setData(0, Qt::UserRole, var);
        pRootItem->addChild(pItem);
    }

    if (0 >= m_pGroupTree->topLevelItem(0)->childCount())
    {
        m_pGroupTree->clear();
    }
}

QString CFilterTreePlugin::getFilterCondition()
{
    if (0 == m_pConditionTree->topLevelItemCount())
    {
        return QString();
    }

    m_strFilterCondition.clear();

    m_strFilterCondition += "(";
    QTreeWidgetItem *ptopItem = m_pConditionTree->topLevelItem(0);    
    if (0 == ptopItem->childCount())
    {
        CMDFilterTreeDataItem *pFilterItem = ptopItem->data(0, Qt::UserRole).value<CMDFilterTreeDataItem*>();
        QString strText = pFilterItem->getName() + pFilterItem->getOperator() + pFilterItem->getValue();
        return (m_strFilterCondition += strText + ")");
    }

    traversalTree(ptopItem);
    m_strFilterCondition += ")";
    return m_strFilterCondition;
}

void CFilterTreePlugin::changeLanguage()
{
    initLanguage();
}

void CFilterTreePlugin::groupTreeDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
    if (item == m_pGroupTree->topLevelItem(0) || NULL == item)
    {
        return;
    }

    CMDFilterTreeDataItem *pFilterItem = item->data(0, Qt::UserRole).value<CMDFilterTreeDataItem*>();
    if (NULL == pFilterItem)
    {
        return;
    }

    pFilterItem = pFilterItem->clone();

    QTreeWidgetItem *pRootItem  = NULL;

    if (0 == m_pConditionTree->topLevelItemCount())
    {
        pRootItem = new QTreeWidgetItem(m_pConditionTree);

        QString strText  = pFilterItem->getDescripe();
        if (strText.isEmpty())
        {
            strText = pFilterItem->getName();
        }

        pRootItem->setText(0, CMDMultiLanguage::instance()->value(strText));
        QVariant var;
        var.setValue(pFilterItem);
        pRootItem->setData(0, Qt::UserRole, var);
        m_pConditionTree->insertTopLevelItem(0, pRootItem);
    }
    else
    {
        QList<QTreeWidgetItem*> selectItemList = m_pConditionTree->selectedItems();
        if (selectItemList.isEmpty())
        {
            pRootItem = m_pConditionTree->topLevelItem(0);
        }
        else
        {
            pRootItem = selectItemList.at(0);
        }

        if (0 == pRootItem->childCount())
        {
            CMDFilterTreeDataItem *pTmpFilterItem = pRootItem->data(0, Qt::UserRole).value<CMDFilterTreeDataItem*>();
            pTmpFilterItem = pTmpFilterItem->clone();
            QString strText = CMDMultiLanguage::instance()->value(FILTER_STRING_AND);
            pRootItem->setText(0, strText);

            CMDFilterTreeDataItem *pDataItem = new CMDFilterTreeDataItem();
            pDataItem->setName(FILTER_STRING_AND);
            pDataItem->setDescripe(FILTER_STRING_AND);

            QVariant var;
            var.setValue(pDataItem);
            pRootItem->setExpanded(true);
            pRootItem->setData(0, Qt::UserRole, var);

            createChildItem(pRootItem, pTmpFilterItem);
            createChildItem(pRootItem, pFilterItem);
        }
        else
        {
            createChildItem(pRootItem, pFilterItem);
        }
    }
}

void CFilterTreePlugin::conditionTreeDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{  
    CMDFilterTreeDataItem *pFilterItem = item->data(0, Qt::UserRole).value<CMDFilterTreeDataItem*>();
    if (NULL == pFilterItem)
    {
        return;
    }        

    QString strText = pFilterItem->getDescripe();
    if (FILTER_STRING_AND == strText)
    {
        item->setText(0, CMDMultiLanguage::instance()->value(strText));
        pFilterItem->setName(FILTER_STRING_OR);
        pFilterItem->setDescripe(FILTER_STRING_OR);
    }
    else if (FILTER_STRING_OR == strText)
    {
        item->setText(0, CMDMultiLanguage::instance()->value(strText));
        pFilterItem->setName(FILTER_STRING_AND);
        pFilterItem->setDescripe(FILTER_STRING_AND);
    }
    else
    {
        modifyItem(item);
    }
}

void CFilterTreePlugin::conditionTreeClicked(QTreeWidgetItem *item, int /*column*/)
{
    if (NULL == m_pSelectItem)
    {
        return;
    }

    if (findChildItem(m_pSelectItem, item))
    {
        return;
    }

    if (0 == item->childCount())
    {
        if (0 == m_pSelectItem->childCount())
        {
            QString strText = m_pSelectItem->text(0);
            m_pSelectItem->setText(0, FILTER_STRING_AND);

            createChildItem(m_pSelectItem, strText, item->data(0, Qt::UserRole).toInt());
            createChildItem(m_pSelectItem, item->text(0), item->data(0, Qt::UserRole).toInt());
        }
        else
        {
            createChildItem(m_pSelectItem, item->text(0), item->data(0, Qt::UserRole).toInt());
        }

        delete item;
        item = NULL;
    }
    else
    {
        if (0 == m_pSelectItem->childCount())
        {
            createChildItem(item, m_pSelectItem->text(0), m_pSelectItem->data(0, Qt::UserRole).toInt());
            delete m_pSelectItem;
            m_pSelectItem = NULL;
        }
        else
        {
            QTreeWidgetItem *pNewItem = new QTreeWidgetItem(item);
            pNewItem->setText(0, m_pSelectItem->text(0));
            pNewItem->setExpanded(true);
            item->addChild(pNewItem);

            createChildItem(pNewItem, m_pSelectItem);
            m_pSelectItem->takeChildren();
            delete m_pSelectItem;
            m_pSelectItem = NULL;
        }
    }

    m_pSelectItem = NULL;
}

void CFilterTreePlugin::conditionTreeMenuClicked(const QPoint &/*pos*/)
{
    if (NULL == m_pConditionTree->currentItem())
    {
        return;
    }

    m_pMenu->exec(QCursor::pos());
}

void CFilterTreePlugin::andActionTriggered()
{
    QList<QTreeWidgetItem*> selectItemList = m_pConditionTree->selectedItems();
    if (selectItemList.isEmpty())
    {
        return;
    }

    QTreeWidgetItem *pItem = selectItemList.at(0);
    if (FILTER_STRING_OR == pItem->text(0))
    {
        pItem->setText(0, FILTER_STRING_AND);
    }
}

void CFilterTreePlugin::orActionTriggered()
{
    QList<QTreeWidgetItem*> selectItemList = m_pConditionTree->selectedItems();
    if (selectItemList.isEmpty())
    {
        return;
    }

    QTreeWidgetItem *pItem = selectItemList.at(0);
    if (FILTER_STRING_AND == pItem->text(0))
    {
        pItem->setText(0, FILTER_STRING_OR);
    }
}

void CFilterTreePlugin::groupActionTriggered()
{
    QList<QTreeWidgetItem*> selectItemList = m_pConditionTree->selectedItems();
    if (selectItemList.isEmpty())
    {
        return;
    }

    m_pSelectItem = selectItemList.at(0);
}

void CFilterTreePlugin::modifyActionTriggered()
{
    QList<QTreeWidgetItem*> selectItemList = m_pConditionTree->selectedItems();
    if (selectItemList.isEmpty())
    {
        return;
    }

    QString strText = selectItemList.at(0)->text(0);
    if (FILTER_STRING_AND == strText || FILTER_STRING_OR == strText)
    {
        return;
    }

    modifyItem(selectItemList.at(0));
}

void CFilterTreePlugin::deleteActionTriggered()
{
    QList<QTreeWidgetItem*> selectItemList = m_pConditionTree->selectedItems();
    if (selectItemList.isEmpty())
    {
        return;
    }

    QTreeWidgetItem *pSelectItem = selectItemList.at(0);
    deleteChildItem(pSelectItem);

    QTreeWidgetItem *pParent = pSelectItem->parent();
    if ((NULL != pParent)
         && (0 == pParent->childCount())
         && (FILTER_STRING_AND == pParent->text(0)
         || FILTER_STRING_OR == pParent->text(0)))
    {
        delete pParent;
        pParent = NULL;
    }

    delete pSelectItem;
    pSelectItem = NULL;
}

void CFilterTreePlugin::createUI()
{
    setWindowTitle(tr("过滤条件"));

    m_pHTopLayout    = new QHBoxLayout;
    m_pHBottomLayout = new QHBoxLayout;
    m_pVMainLayout   = new QVBoxLayout;

    m_pGroupTree      = new QTreeWidget(this);
    m_pConditionTree  = new QTreeWidget(this);

    m_pBtnOk = new QPushButton(tr("确定"), this);
    m_pBtnCancel = new QPushButton(tr("取消"), this);

    m_pGroupTree->setHeaderLabel(tr("字段名"));
    m_pConditionTree->setHeaderLabel(tr("过滤条件"));

//    QFont font;
//    font.setPixelSize(14);
//    m_pGroupTree->setFont(font);
//    m_pConditionTree->setFont(font);

    QString strTreeStyle = "QTreeView::branch:has-siblings:!adjoins-item {\
            border-image: url(:/resource/resource/stylesheet-vline.png) 0;\
        }\
        QTreeView::branch:has-siblings:adjoins-item {\
            border-image: url(:/resource/resource/stylesheet-branch-more.png) 0;\
        }\
        QTreeView::branch:!has-children:!has-siblings:adjoins-item {\
            border-image: url(:/resource/resource/stylesheet-branch-end.png) 0;\
        }\
        QTreeView::branch:has-children:!has-siblings:closed,\
        QTreeView::branch:closed:has-children:has-siblings {\
                border-image: none;\
                image: url(:/resource/resource/stylesheet-branch-closed.png);\
        }\
        QTreeView::branch:open:has-children:!has-siblings,\
        QTreeView::branch:open:has-children:has-siblings  {\
                border-image: none;\
                image: url(:/resource/resource/stylesheet-branch-open.png);\
        }";

    setStyleSheet(strTreeStyle);
//    m_pGroupTree->setStyleSheet(strTreeStyle);
//    m_pConditionTree->setStyleSheet(strTreeStyle);

    m_pConditionTree->setContextMenuPolicy(Qt::CustomContextMenu);

    m_pHTopLayout->addWidget(m_pGroupTree);
    m_pHTopLayout->addWidget(m_pConditionTree);

    m_pHBottomLayout->addStretch(0);
    m_pHBottomLayout->addWidget(m_pBtnOk);
    m_pHBottomLayout->addWidget(m_pBtnCancel);

    m_pVMainLayout->addLayout(m_pHTopLayout);
    m_pVMainLayout->addLayout(m_pHBottomLayout);

    setLayout(m_pVMainLayout);
}

void CFilterTreePlugin::createMenu()
{
    m_pMenu               = new QMenu(m_pConditionTree);

    m_pAndAction          = new QAction(tr("And"), m_pMenu);
    m_pOrAction           = new QAction(tr("Or"), m_pMenu);
    m_pGroupAction        = new QAction(tr("组"), m_pMenu);
    m_pModifyAction       = new QAction(tr("修改"), m_pMenu);
    m_pDeleteAction       = new QAction(tr("删除"), m_pMenu);

    m_pMenu->addAction(m_pAndAction);
    m_pMenu->addAction(m_pOrAction);
    m_pMenu->addSeparator();
    m_pMenu->addAction(m_pGroupAction);
    m_pMenu->addAction(m_pModifyAction);
    m_pMenu->addSeparator();
    m_pMenu->addAction(m_pDeleteAction);
}

void CFilterTreePlugin::signalsBindSlots()
{
    connect(m_pGroupTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this, SLOT(groupTreeDoubleClicked(QTreeWidgetItem*, int)));

    connect(m_pConditionTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this, SLOT(conditionTreeDoubleClicked(QTreeWidgetItem*, int)));

    connect(m_pConditionTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(conditionTreeClicked(QTreeWidgetItem*, int)));

    connect(m_pConditionTree, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(conditionTreeMenuClicked(QPoint)));

    connect(m_pAndAction, SIGNAL(triggered()), this, SLOT(andActionTriggered()));
    connect(m_pOrAction, SIGNAL(triggered()), this, SLOT(orActionTriggered()));
    connect(m_pGroupAction, SIGNAL(triggered()), this, SLOT(groupActionTriggered()));
    connect(m_pModifyAction, SIGNAL(triggered()), this, SLOT(modifyActionTriggered()));
    connect(m_pDeleteAction, SIGNAL(triggered()), this, SLOT(deleteActionTriggered()));

    connect(m_pBtnOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_pBtnCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void CFilterTreePlugin::initData()
{
    m_pHTopLayout       = NULL; //! 顶部水平布局管理器
    m_pHBottomLayout    = NULL; //! 底部水平布局管理器
    m_pVMainLayout      = NULL; //! 整体水平布局管理器

    m_pGroupTree        = NULL; //! 选择字段树
    m_pConditionTree    = NULL; //! 条件配置树
    m_pSelectItem       = NULL; //! 条件配置树组选择结点

    m_pBtnOk            = NULL; //! 确定按钮
    m_pBtnCancel        = NULL; //! 取消按钮

    m_pMenu             = NULL; //! 条件配置树菜单
    m_pAndAction        = NULL; //! and菜单选项
    m_pOrAction         = NULL; //! or菜单选项
    m_pGroupAction      = NULL; //! 组菜单选项
    m_pModifyAction     = NULL; //! 修改菜单选项
    m_pDeleteAction     = NULL; //! 删除菜单选项

    m_pParent = NULL;

    m_pMulLnaguageTool = NULL;

    m_strFilterCondition = "";            //! 过滤条件
    m_strFile = "";                       //! 序列化文件
    m_iLoadFlag = false;
}

void CFilterTreePlugin::initLanguage()
{
    if (NULL == m_pMulLnaguageTool)
    {
        m_pMulLnaguageTool = CMDMultiLanguage::createWidgetTool();
        m_pMulLnaguageTool->init(this);
    }

    m_pMulLnaguageTool->changeLanguage();
}

QVariant::Type CFilterTreePlugin::datatypeChangeVariant(int type)
{
    QVariant::Type varType = QVariant::Invalid;
    switch (type)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 5:
    case 7:
        varType = QVariant::Int;
        break;
    case 4:
    case 6:
    case 8:
        varType = QVariant::UInt;
        break;
    case 9:
    case 10:
        varType = QVariant::Double;
        break;
    case 11:
        varType = QVariant::DateTime;
        break;
    case 12:
        varType = QVariant::String;
        break;
    default:
        varType = QVariant::String;
        break;
    }

    return varType;
}

int CFilterTreePlugin::calculateType(const QString &text)
{
    int index = text.indexOf("\'");
    int type = 0;
    if (-1 != index)
    {
        QDateTime dateTime = QDateTime::fromString(text.mid(index + 1, text.size() - index - 2),
                                                   "yyyy-MM-dd hh:mm:ss");
        if (dateTime.isValid())
        {
            type = QVariant::DateTime;
        }
        else
        {
            type = QVariant::String;
        }
    }
    else
    {
        if (-1 == text.indexOf("."))
        {
            type = QVariant::Int;
        }
        else
        {
            type = QVariant::Double;
        }
    }

    return type;
}

QTreeWidgetItem *CFilterTreePlugin::createGroupTreeRoot(const QString &text)
{
    QTreeWidgetItem *pRootItem  = new QTreeWidgetItem(m_pGroupTree);
    m_pGroupTree->insertTopLevelItem(0, pRootItem);
    pRootItem->setText(0, text);
    pRootItem->setExpanded(true);

    return pRootItem;
}

void CFilterTreePlugin::createChildItem(QTreeWidgetItem *parent, IMDDataField *pField)
{
    QTreeWidgetItem *pItem = new QTreeWidgetItem(parent);

    CMDFilterTreeDataItem *pFilterItem = new CMDFilterTreeDataItem();
    QString strName = pField->name();
    QString strDescrip = pField->description();
    if (strDescrip.isEmpty())
    {
      strDescrip = strName;
    }

    pItem->setText(0, CMDMultiLanguage::instance()->value(strDescrip));

    pFilterItem->setDescripe(strDescrip);
    pFilterItem->setName(strName);
    pFilterItem->setDataType((int)(datatypeChangeVariant((int)pField->dataType())));

    QVariant var;
    var.setValue (pFilterItem);
    pItem->setData(0, Qt::UserRole, var);
    parent->addChild(pItem);
}

void CFilterTreePlugin::createChildItem(QTreeWidgetItem *parent, CMDFilterTreeDataItem *pFilterItem)
{
    QTreeWidgetItem *pItem = new QTreeWidgetItem(parent);

    QString strText = CMDMultiLanguage::instance()->value(pFilterItem->getDescripe());
    if (strText.isEmpty())
    {
        strText = CMDMultiLanguage::instance()->value(pFilterItem->getName());
    }

    if (!pFilterItem->getOperator().isEmpty())
    {
        strText += " " + pFilterItem->getOperator();
    }

    if (!pFilterItem->getOperator().isEmpty())
    {
        strText += " " + pFilterItem->getValue();
    }

    pItem->setText(0, strText);

    QVariant var;
    var.setValue (pFilterItem);
    pItem->setData(0, Qt::UserRole, var);
    parent->addChild(pItem);
}

void CFilterTreePlugin::createChildItem(QTreeWidgetItem *parent, const QString &text, int type)
{
    QTreeWidgetItem *pItem = new QTreeWidgetItem(parent);
    pItem->setText(0, text);
    pItem->setData(0, Qt::UserRole, type);
    parent->addChild(pItem);
}

void CFilterTreePlugin::createChildItem(QTreeWidgetItem *parent, QTreeWidgetItem *item)
{
    QTreeWidgetItem *pChildItem = NULL;
    QTreeWidgetItem *pNewItem   = NULL;
    for (int i = 0; i < item->childCount(); ++i)
    {
        pChildItem = item->child(i);
        if (NULL == pChildItem)
        {
            continue;
        }

        if (0 == pChildItem->childCount())
        {
            createChildItem(parent, pChildItem->text(0), pChildItem->data(0, Qt::UserRole).toInt());
        }
        else
        {
            pNewItem = new QTreeWidgetItem(parent);
            pNewItem->setText(0, pChildItem->text(0));
            pNewItem->setExpanded(true);
            parent->addChild(pNewItem);
            createChildItem(pNewItem, pChildItem);
        }
    }
}

void CFilterTreePlugin::deleteChildItem(QTreeWidgetItem *item)
{
    QList<QTreeWidgetItem*> childItemList = item->takeChildren();
    qDeleteAll(childItemList.begin(), childItemList.end());
    childItemList.clear();
//    QTreeWidgetItem *pChildItem = NULL;
//    for (int i = 0; i < item->childCount(); ++i)
//    {
//        pChildItem = item->child(i);
//        if (0 ==  pChildItem->childCount())
//        {
//            item->removeChild(pChildItem);
//            delete pChildItem;
//            pChildItem = NULL;
//        }
//        else
//        {
//            deleteChildItem(pChildItem);
//        }
    //    }
}

void CFilterTreePlugin::modifyItem(QTreeWidgetItem *item)
{
    CMDFilterTreeDataItem *pFilterItem = item->data(0, Qt::UserRole).value<CMDFilterTreeDataItem*>();
    if (NULL == pFilterItem)
    {
        return;
    }

    CMDFilterConditionSetting setting(this);
    setting.setPropertyInfo(pFilterItem);

    if (QDialog::Accepted == setting.exec())
    {                
        QString strText = CMDMultiLanguage::instance()->value(pFilterItem->getDescripe());
        if (strText.isEmpty())
        {
            strText = CMDMultiLanguage::instance()->value(pFilterItem->getName());
        }

        strText += " " + pFilterItem->getOperator() + " " + pFilterItem->getValue();
        item->setText(0, strText);
    }
}

void CFilterTreePlugin::traversalTree(QTreeWidgetItem *root)
{
    QTreeWidgetItem *pChildItem = NULL;
    QString strText = "";
    for (int i = 0; i < root->childCount(); ++i)
    {
        pChildItem = root->child(i);
        if (NULL == pChildItem)
        {
            continue;
        }

        CMDFilterTreeDataItem *pFilterItem = pChildItem->data(0, Qt::UserRole).value<CMDFilterTreeDataItem*>();
        if (NULL == pFilterItem)
        {
            continue;
        }

        if (0 == pChildItem->childCount())
        {
            strText = pFilterItem->getName() + pFilterItem->getOperator() + pFilterItem->getValue();
            if (i == root->childCount() - 1)
            {
                m_strFilterCondition += strText;
            }
            else
            {                
                m_strFilterCondition += strText + " " + pChildItem->parent()->text(0) + " ";
            }            
        }
        else
        {
            if (FILTER_STRING_AND == pChildItem->text(0)
               || FILTER_STRING_OR == pChildItem->text(0))
            {
                m_strFilterCondition += "(";
            }
            else
            {
                m_strFilterCondition +=  " " + pChildItem->parent()->text(0) + " (";
            }
            traversalTree(pChildItem);
            m_strFilterCondition += ")";
        }
    }
}

bool CFilterTreePlugin::findChildItem(QTreeWidgetItem *root, QTreeWidgetItem *item)
{
    if (0 == root->childCount())
    {
        return false;
    }

    QTreeWidgetItemIterator it(root);
    while (*it)
    {
        if ((*it) == item)
        {
            return true;
        }
        ++it;
    }

    return false;
//    if (root == item)
//    {
//        return true;
//    }

//    int count = root->childCount();
//    QTreeWidgetItem *pChildItem = NULL;

//    for (int i = 0; i < count; ++i)
//    {
//        pChildItem = root->child(i);
//        if (NULL == pChildItem)
//        {
//            continue;
//        }

//        if (0 ==  pChildItem->childCount())
//        {
//            if (item == pChildItem)
//            {
//                return true;
//            }
//            else
//            {
//                continue;
//            }
//        }
//        else
//        {
//            return findChildItem(pChildItem, item);
//        }
//    }

    //    return false;
}

void CFilterTreePlugin::save(QTreeWidgetItem *root, CMDFilterTreeData *pFilterData)
{
    for (int i = 0; i < root->childCount(); ++i)
    {
        QTreeWidgetItem *pChildItem  = root->child(i);
        if (NULL == pChildItem)
        {
            continue;
        }

        CMDFilterTreeDataItem *pFilterItem = pChildItem->data(0, Qt::UserRole).value<CMDFilterTreeDataItem*>();
        if (NULL == pFilterItem)
        {
            continue;
        }

        pFilterItem->setCount(pChildItem->childCount());
        pFilterData->insert(pFilterItem->clone());

        if (0 < pChildItem->childCount())
        {
            save(pChildItem, pFilterData);
        }
    }
}

QString CFilterTreePlugin::splicingCurrentItemText(QStringList &itemList, int &start)
{
    QString strItem = "";
    QString strTmp = "";
    for (int j = start; j < itemList.size(); ++j)
    {
        strTmp = itemList.at(j);
        if (strTmp.isEmpty())
        {
            continue;
        }
        if (FILTER_STRING_AND == strTmp
                || FILTER_STRING_OR == strTmp
                || ")" == strTmp)
        {
            start = --j;
            break;
        }

        strItem += strTmp + " ";
    }

    strItem = strItem.remove(strItem.size() - 1, 1);
    return strItem;
}

QString CFilterTreePlugin::splicingString(QStringList &itemList, int start, int &end)
{
    QString strItem = "";
    QString strTmp = "";
    for (int i = start; i < itemList.size(); ++i)
    {
        strTmp = itemList.at(i);
        if (strTmp.isEmpty())
        {
            continue;
        }

        strItem += strTmp;
        if ("(" != strTmp)
        {
            end = i;
            break;
        }
    }

    return strItem;
}

void CFilterTreePlugin::getParentItem(QTreeWidgetItem *pChild)
{
    if (NULL == m_pParent)
    {
        m_pParent = pChild;
        m_pConditionTree->insertTopLevelItem(0, pChild);
    }
    else
    {
        if (m_pParent != pChild)
        {
            m_pParent->addChild(pChild);
            m_pParent = pChild;
        }
    }

    m_pParent->setExpanded(true);
}

void CFilterTreePlugin::findChildItem(QStringList &itemList, int &start)
{
    QString strItem = "";
    QString strTmp = "";
    QString strSplit = "";

    QList<int> left;
    left.clear();

    QStringList splitItemList;
    splitItemList.clear();

    int offset = start;
    QTreeWidgetItem *pChildItem = NULL;
    for (int i = start; i < itemList.size(); ++i)
    {
        strItem = itemList.at(i);
        if (strItem.isEmpty())
        {
            continue;
        }

        if ("(" == strItem)
        {
            left.append(i);
        }
        else if (")" == strItem)
        {
            if (left.size() > 0)
            {
                if (!left.isEmpty())
                {
                    offset = left.at(0);
                }

                left.pop_back();
                if (left.size() == 0)
                {
                    strSplit.clear();
                    for (int index = offset; index <= i; ++index)
                    {
                        strTmp = itemList.at(index);
                        if (strTmp.isEmpty())
                        {
                            continue;
                        }

                        if (strSplit.isEmpty())
                        {
                            strSplit += strTmp;
                        }
                        else
                        {
                            strSplit += " " + strTmp;
                        }
                    }

                    bool endFlag = false;
                    for (int j = i + 1; j < itemList.size(); ++j)
                    {
                        strTmp = itemList.at(j);
                        if (strTmp.isEmpty())
                        {
                            continue;
                        }

                        if (")" == strTmp)
                        {
                            endFlag = true;
                        }
                        else
                        {
                            endFlag = false;
                            break;
                        }
                    }

                    if (endFlag)
                    {
                        for (int pos = start - 1; pos >= 0; --pos)
                        {
                            strTmp = itemList.at(start - 1);
                            if (strTmp.isEmpty())
                            {
                                continue;
                            }

                            if (FILTER_STRING_AND == strTmp
                                    || FILTER_STRING_OR == strTmp)
                            {
                                pChildItem = new QTreeWidgetItem;
                                pChildItem->setText(0, strTmp);
                                pChildItem->setData(0, Qt::UserRole, calculateType(strTmp));
                                getParentItem(pChildItem);
                                strSplit += " " + strTmp;
                                start = i;
                                offset = start;
                                left.clear();
                                break;
                            }
                            else if ("(" == strTmp)
                            {

                            }
                            else
                            {
                                break;
                            }
                        }
                    }

                    for (int j = i + 1; j < itemList.size(); ++j)
                    {
                        strTmp = itemList.at(j);
                        if (strTmp.isEmpty())
                        {
                            continue;
                        }

                        if (")" == strTmp)
                        {
                            strSplit += strTmp;
                        }
                        else if (FILTER_STRING_AND == strTmp
                                || FILTER_STRING_OR == strTmp)
                        {
                            pChildItem = new QTreeWidgetItem;
                            pChildItem->setText(0, strTmp);
                            pChildItem->setData(0, Qt::UserRole, calculateType(strTmp));
                            getParentItem(pChildItem);
                            strSplit += " " + strTmp;
                            i = j;
                            start = i;
                            offset = start;
                            left.clear();
                            break;
                        }
                    }

                    load(strSplit);
                    break;
                }
            }
        }
    }
}

bool CFilterTreePlugin::load(const QString &condition)
{
    if (!m_iLoadFlag)
    {
        m_pParent = NULL;
        m_iLoadFlag = true;
    }

    QStringList splitItemList = condition.split(" ", QString::SkipEmptyParts);

    QStringList itemList;
    itemList.clear();

    int countRigMark = 0;
    int countLeftMark = 0;
    int splitSize = splitItemList.size();

    QString strSplit = "";
    QString strItem = "";
    QString strTmp = "";

    QTreeWidgetItem *pChildItem = NULL;

    for (int i = 0; i < splitSize; ++i)
    {
        strSplit = splitItemList.at(i);
        if (strSplit.isEmpty())
        {
            continue;
        }

        if ("(" == strSplit.left(1))
        {
            countLeftMark = strSplit.count("(");
            while (countLeftMark)
            {
                itemList.append("(");
                --countLeftMark;
            }

            if (!strSplit.remove("(").isEmpty())
            {
                strSplit = strSplit.remove("(");
                itemList.append(strSplit.remove("("));
            }
        }
        else if (")" == strSplit.right(1))
        {
            countRigMark = strSplit.count(")");
            if (!strSplit.remove(")").isEmpty())
            {
                itemList.append(strSplit.remove(")"));
            }
            while(countRigMark--)
            {
                itemList.append(")");
            }
        }
        else
        {
            itemList.append(strSplit);
        }
    }

    int end = 0;
    for (int k  = 0; k < itemList.size(); ++k)
    {
        strTmp = itemList.at(k);
        if (strTmp.isEmpty())
        {
            continue;
        }

        strSplit = splicingString(itemList, k, end);
        countLeftMark = strSplit.count("(");
        if (1 == countLeftMark)
        {
            k = end;
            for (int pos = k + 1; pos < itemList.size(); ++pos)
            {
                strTmp = itemList.at(pos);
                if (strTmp.isEmpty())
                {
                    continue;
                }
                if (FILTER_STRING_AND == strTmp
                        || FILTER_STRING_OR == strTmp
                        || ")" == strTmp)
                {
                    strItem.clear();
                    strItem = itemList.at(pos);
                    pChildItem = new QTreeWidgetItem();
                    pChildItem->setText(0, strItem);
                    pChildItem->setData(0, Qt::UserRole, calculateType(strItem));
                    getParentItem(pChildItem);
                    break;
                }
            }

            strItem.clear();
            strItem = splicingCurrentItemText(itemList, k);
            countRigMark = strItem.count(")");
            strItem = strItem.remove("(");
            strItem = strItem.remove(")");
//            pChildItem = new QTreeWidgetItem();
//            pChildItem->setText(0, strItem);
//            pChildItem->setData(0, Qt::UserRole, calculateType(strItem));
//            m_pParent->addChild(pChildItem);

            createChildItem(m_pParent, strItem, calculateType(strItem));

            while (countRigMark)
            {
                if (m_pParent->parent())
                {
                    m_pParent = m_pParent->parent();
                }
                --countRigMark;
            }

            ++k;
        }
        else if (0 >= countLeftMark)
        {
            if (")" == strTmp)
            {
                if (m_pParent->parent())
                {
                    m_pParent = m_pParent->parent();
                }
            }
            else if (FILTER_STRING_AND == strTmp
                     || FILTER_STRING_OR == strTmp)
            {
                continue;
            }
            else
            {
                strItem.clear();
                strItem = splicingCurrentItemText(itemList, k);
                countRigMark = strItem.count(")");
                strItem = strItem.remove("(");
                strItem = strItem.remove(")");
//                pChildItem = new QTreeWidgetItem();
//                pChildItem->setText(0, strItem);
//                pChildItem->setData(0, Qt::UserRole, calculateType(strItem));
//                m_pParent->addChild(pChildItem);
                createChildItem(m_pParent, strItem, calculateType(strItem));

//                while (countRigMark)
//                {
//                    if (m_pParent->parent())
//                    {
//                        m_pParent = m_pParent->parent();
//                    }
//                    --countRigMark;
//                }
            }
        }
        else
        {
            itemList.removeAt(k);
            findChildItem(itemList, k);
        }
    }

    m_iLoadFlag = false;

    return true;
}
