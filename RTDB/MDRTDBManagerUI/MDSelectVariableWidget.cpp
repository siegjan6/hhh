#include "MDSelectVariableWidget.h"
#include "ui_SelectVariableWidget.h"
#include "MDRTDBManagerFactory.h"
#include "IMDTreeItem.h"
#include <QHeaderView>
#include "MDMultiLanguage.h"

CMDSelectVariableWidget::CMDSelectVariableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CMDSelectVariableWidget)
{
    ui->setupUi(this);
    setWindowTitle(tr("选择变量"));
    this->setWindowModality(Qt::ApplicationModal);

    m_treeView = new QTreeView();
    m_treeView->setAlternatingRowColors(true);
    m_treeView->setAnimated(false);
    m_treeView->setAllColumnsShowFocus(true);
    m_treeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_treeView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    connect(m_treeView,&QTreeView::doubleClicked,this,&CMDSelectVariableWidget::doubleClickTree);

    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->addWidget(m_treeView);
    m_mainLayout->setMargin(1);
    m_mainLayout->setStretchFactor(m_treeView,0);
    this->setLayout(m_mainLayout);

    //多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

CMDSelectVariableWidget::~CMDSelectVariableWidget()
{
    delete m_variableTreeModel;
    delete m_treeView;
    delete m_mainLayout;
    delete ui;
}
void CMDSelectVariableWidget::setRTDBManager(IMDRTDBManager*nsRTDBManager)
{
    m_rtdbManager = nsRTDBManager;
    m_variableTreeModel = CMDRTDBManagerFactory::createBrowseVariableTreeModel(m_rtdbManager);
    m_variableTreeModel->setTreeItemIcon(QIcon(":/images/VariableGroup.png"),MDTreeItemType::VariableGroup);
    m_variableTreeModel->setTreeItemIcon(QIcon(":/images/Variable.png"),MDTreeItemType::Variable);
    m_variableTreeModel->setCheckedable(false);
    m_treeView->setModel(m_variableTreeModel);
}

bool CMDSelectVariableWidget::getSelectedVariables(QList<IMDVariable*>&varList)
{
     QList<QModelIndex> checkedItemList=  m_variableTreeModel->getCheckedItems();
     for(int i=0;i<checkedItemList.count();i++)
     {
         IMDTreeItem *item = static_cast<IMDTreeItem*>(checkedItemList[i].internalPointer());

         if(item->itemType() == MDTreeItemType::Variable)
         {
//             IMDVariable *variable = (IMDVariable *)(IMDRTDBVariable *)item;
             IMDVariable *variable = static_cast<IMDVariable*>(checkedItemList[i].internalPointer());
             varList.append(variable);
         }
     }
     return true;
}
void CMDSelectVariableWidget::doubleClickTree(const QModelIndex &index)
{
    QString str;
    IMDTreeItem *item = static_cast<IMDTreeItem*>(index.internalPointer());
    if(item != NULL && item->itemType() == MDTreeItemType::Variable)
    {
#ifdef QT_DEBUG
        IMDVariable *variable = (IMDVariable *)(IMDRTDBVariable *)item;
#else
        IMDVariable *variable = static_cast<IMDVariable*>(static_cast<IMDRTDBVariable*>(item));
#endif
        str = variable->name();
        emit selectVariableName(str);
    }
}
