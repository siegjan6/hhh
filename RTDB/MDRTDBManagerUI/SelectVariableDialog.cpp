#include "SelectVariableDialog.h"
#include "ui_SelectVariableDialog.h"
#include "MDRTDBManagerFactory.h"
#include "IMDTreeItem.h"
#include <QHeaderView>
#include "MDMultiLanguage.h"
CSelectVariableDialog::CSelectVariableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSelectVariableDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("选择变量"));

    ui->treeView->setAlternatingRowColors(true);
    ui->treeView->setAnimated(false);
    ui->treeView->setAllColumnsShowFocus(true);
    ui->treeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->treeView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->treeView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

CSelectVariableDialog::~CSelectVariableDialog()
{
    delete m_variableTreeModel;
    delete ui;
}
void CSelectVariableDialog::setRTDBManager(IMDRTDBManager*nsRTDBManager)
{
    m_rtdbManager = nsRTDBManager;
    m_variableTreeModel = CMDRTDBManagerFactory::createBrowseVariableTreeModel(m_rtdbManager);
    m_variableTreeModel->setTreeItemIcon(QIcon(":/images/VariableGroup.png"),MDTreeItemType::VariableGroup);
    m_variableTreeModel->setTreeItemIcon(QIcon(":/images/Variable.png"),MDTreeItemType::Variable);

    ui->treeView->setModel(m_variableTreeModel);
}

bool CSelectVariableDialog::getSelectedVariables(QList<IMDVariable*>&varList)
{
     QList<QModelIndex> checkedItemList=  m_variableTreeModel->getCheckedItems();
     for(int i=0;i<checkedItemList.count();i++)
     {
         IMDTreeItem *item = static_cast<IMDTreeItem*>(checkedItemList[i].internalPointer());

         if(item->itemType() == MDTreeItemType::Variable)
         {
             IMDVariable *variable = (IMDVariable *)(IMDRTDBVariable *)item;
//             IMDVariable *variable = static_cast<IMDVariable*>(checkedItemList[i].internalPointer());
             varList.append(variable);
         }
     }
     return true;
}

void CSelectVariableDialog::on_OkButton_clicked()
{
    this->setResult(QDialog::Accepted);
    this->accept();
}

void CSelectVariableDialog::on_CancelButton_clicked()
{

    this->setResult(QDialog::Rejected);
    this->reject ();
}
