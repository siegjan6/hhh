#include "MDPowerGroupAccountDialog.h"
#include "ui_MDPowerGroupAccountDialog.h"
#include "MDPowerGroup.h"
#include "MDPowerGroupTreeWindow.h"
#include "MDStringResource.h"

CMDPowerGroupAccountDialog::CMDPowerGroupAccountDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDPowerGroupAccountDialog)
{
    ui->setupUi(this);
}

CMDPowerGroupAccountDialog::~CMDPowerGroupAccountDialog()
{
    delete ui;
}

void CMDPowerGroupAccountDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CMDPowerGroup* src = (CMDPowerGroup*)sourceProperty;
    ui->accountListWidget->addItems(src->accountList());
}

bool CMDPowerGroupAccountDialog::UI2Property(void *destProperty)
{
    CMDPowerGroup* dest = (CMDPowerGroup*)destProperty;

    QStringList accountNameList;
    int count = ui->accountListWidget->count();
    for(int i = 0; i < count; i++)
    {
        QListWidgetItem* item = ui->accountListWidget->item(i);
        accountNameList.append(item->text());
    }

    dest->setAccountList(accountNameList);

    return true;
}

void CMDPowerGroupAccountDialog::on_removeButton_clicked()
{
    ui->accountListWidget->takeItem(ui->accountListWidget->currentRow());
}

void CMDPowerGroupAccountDialog::on_addButton_clicked()
{
    if(!objectUI())
        return;
    CMDPowerGroupTreeWindow* treeWindow = (CMDPowerGroupTreeWindow*)objectUI();

    QStringList sourceList;
    QStringList destList;
    if(!treeWindow->selectAccounts(sourceList, destList))
        return;

    int count = destList.count();
    for(int i = 0; i < count; i++)
    {
        QString accountName = destList.at(i);
        QList<QListWidgetItem*> itemList = ui->accountListWidget->findItems(accountName, Qt::MatchCaseSensitive);
        if(itemList.isEmpty())
            ui->accountListWidget->addItem(accountName);
    }
}
