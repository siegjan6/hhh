#include "MDSelectObjectDialog.h"
#include "ui_MDSelectObjectDialog.h"
#include "MDObjectTreeItem.h"
#include "MDObjectTreeWindow.h"
#include "MDObjectTreeModel.h"

CMDSelectObjectDialog::CMDSelectObjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDSelectObjectDialog)
{
    ui->setupUi(this);

    m_treeModel = new CMDObjectTreeModel;
    m_objectTreeWindow = NULL;
}

CMDSelectObjectDialog::~CMDSelectObjectDialog()
{
    delete ui;

    if(m_treeModel)
        delete m_treeModel;
}

void CMDSelectObjectDialog::init(const QString& title, CMDObjectTreeWindow *treeWindow, const QStringList& sourceList, bool addObj)
{
    m_objectTreeWindow = treeWindow;

    if(m_treeModel)
    {
        m_treeModel->setCheckedable(true);
        m_treeModel->initial(treeWindow, sourceList);
        ui->treeView->setModel(m_treeModel);
    }
    treeWindow->initTreeColumnWidth(true, ui->treeView);

    setWindowTitle(title);

    ui->AddObjButton->setVisible(addObj);
}

bool CMDSelectObjectDialog::selectedObjects_Name(QStringList& objectNameList, qint16 objectType)
{
    if(!m_treeModel || !m_objectTreeWindow)
        return false;

    QList<QModelIndex> checkedItemList =  m_treeModel->getCheckedItems();
    for(int i=0;i<checkedItemList.count();i++)
    {
        CMDObjectTreeItem *item = static_cast<CMDObjectTreeItem*>(checkedItemList[i].internalPointer());

        if(item->type() == objectType)
            objectNameList.append(m_objectTreeWindow->objectName(item->ptr()));
    }
    return true;
}

bool CMDSelectObjectDialog::selectedObjects_Ptr(QList<void*>& objectList, qint16 objectType)
{
    if(!m_treeModel || !m_objectTreeWindow)
        return false;

    QList<QModelIndex> checkedItemList =  m_treeModel->getCheckedItems();
    for(int i=0;i<checkedItemList.count();i++)
    {
        CMDObjectTreeItem *item = static_cast<CMDObjectTreeItem*>(checkedItemList[i].internalPointer());

        if(item->type() == objectType)
            objectList.append(item->ptr());
    }
    return true;
}

void CMDSelectObjectDialog::on_OKButton_clicked()
{
    this->setResult(QDialog::Accepted);
    this->accept();
}

void CMDSelectObjectDialog::on_CancelButton_clicked()
{
    this->setResult(QDialog::Rejected);
    this->reject ();
}

void CMDSelectObjectDialog::on_AddObjButton_clicked()
{
    m_objectTreeWindow->showSubMenuForAdd();

    if(m_treeModel)
    {
        m_treeModel->resetModelData();
    }
}
