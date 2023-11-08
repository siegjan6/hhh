#include "MDEventActionDialog.h"
#include "ui_MDEventActionDialog.h"
#include "MDEventProperty_Config.h"
#include "MDEventUIImp.h"
#include "MDEventUI.h"
#include <QListWidget>
#include "MDEventTreeWindow.h"
#include "MDStringResource.h"

CMDEventActionDialog::CMDEventActionDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDEventActionDialog)
{
    ui->setupUi(this);

    QStringList exs;
    exs << "QListWidget";
    setLanguageExceptions(&exs);
}

CMDEventActionDialog::~CMDEventActionDialog()
{
    delete ui;
}

void CMDEventActionDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CMDEventProperty_Config* src = (CMDEventProperty_Config*)sourceProperty;
    ui->actionListWidget->addItems(src->actionNameList());
}

bool CMDEventActionDialog::UI2Property(void *destProperty)
{
    CMDEventProperty_Config* dest = (CMDEventProperty_Config*)destProperty;

    QStringList actionNameList;
    int count = ui->actionListWidget->count();
    for(int i = 0; i < count; i++)
    {
        QListWidgetItem* item = ui->actionListWidget->item(i);
        actionNameList.append(item->text());
    }

    dest->setActionNameList(actionNameList);

    return true;
}

void CMDEventActionDialog::on_addButton_clicked()
{
    if(!objectUI())
        return;
    CMDEventTreeWindow* treeWindow = (CMDEventTreeWindow*)objectUI();

    QStringList sourceList;
    QStringList destList;
    if(!treeWindow->selectActions(sourceList, destList))
        return;

    int count = destList.count();
    for(int i = 0; i < count; i++)
    {
        QString actionName = destList.at(i);
        QList<QListWidgetItem*> itemList = ui->actionListWidget->findItems(actionName, Qt::MatchCaseSensitive);
        if(itemList.isEmpty())
            ui->actionListWidget->addItem(actionName);
    }
}

void CMDEventActionDialog::on_removeButton_clicked()
{
    ui->actionListWidget->takeItem(ui->actionListWidget->currentRow());
}

void CMDEventActionDialog::on_forwardButton_clicked()
{
    int curRow = ui->actionListWidget->currentRow();
    if(curRow <= 0)
        return;

    QListWidgetItem* curItem = ui->actionListWidget->currentItem();
    if(curItem)
    {
        ui->actionListWidget->takeItem(curRow);

        QString text = curItem->text();
        ui->actionListWidget->insertItem(curRow - 1, text);
        ui->actionListWidget->setCurrentRow(curRow - 1);
    }
}

void CMDEventActionDialog::on_backwardButton_clicked()
{
    int count = ui->actionListWidget->count();
    int curRow = ui->actionListWidget->currentRow();
    if(curRow >= (count - 1))
        return;

    QListWidgetItem* curItem = ui->actionListWidget->currentItem();
    if(curItem)
    {
        ui->actionListWidget->takeItem(curRow);

        QString text = curItem->text();
        ui->actionListWidget->insertItem(curRow + 1, text);
        ui->actionListWidget->setCurrentRow(curRow + 1);
    }
}
