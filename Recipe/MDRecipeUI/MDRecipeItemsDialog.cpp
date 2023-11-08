#include "MDRecipeItemsDialog.h"
#include "ui_MDRecipeItemsDialog.h"
#include "MDRecipeProperty_Config.h"
#include <QMessageBox>
#include <QVariant>
#include "MDStringResource.h"
#include "MDSelectVariableWidget.h"
#include "MDProjectConfiger.h"

CMDRecipeItemsDialog::CMDRecipeItemsDialog(CMDProjectConfiger* pConfiger, QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDRecipeItemsDialog)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnWidth(0, 270);
    ui->tableWidget->setColumnWidth(1, 90);

    m_varWidget = NULL;
    m_pConfiger = pConfiger;
}

CMDRecipeItemsDialog::~CMDRecipeItemsDialog()
{
    delete ui;
}

void CMDRecipeItemsDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CMDRecipeProperty_Config* src = (CMDRecipeProperty_Config*)sourceProperty;

    QList<CMDRecipeItem*> itemList;
    src->items()->enumItems(itemList);
    ui->tableWidget->setRowCount(src->items()->count());
    int iRow = 0;
    foreach (CMDRecipeItem* item, itemList)
    {
        ui->tableWidget->setItem(iRow, 0, new QTableWidgetItem(item->id()));
        ui->tableWidget->setItem(iRow, 1, new QTableWidgetItem(item->value()));
        iRow ++;
    }

    CMDSelectVariableWidget* w = new CMDSelectVariableWidget;
    w->setRTDBManager(m_pConfiger->nsRTDBManager());
    layout()->addWidget(w);
    connect(w, &CMDSelectVariableWidget::selectVariableName,
        this, &CMDRecipeItemsDialog::addVar);
    m_varWidget = w;
    m_varWidget->setVisible(false);
}

void CMDRecipeItemsDialog::addVar(const QString &var)
{
    QList<QTableWidgetItem*> matchedItems = ui->tableWidget->findItems(var, Qt::MatchExactly);
    if(!matchedItems.isEmpty())
    {
        ui->tableWidget->setCurrentItem(matchedItems.first());
        return;
    }

    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount);
    QTableWidgetItem* newItem = new QTableWidgetItem(var);
    ui->tableWidget->setItem(rowCount, 0, newItem);
    ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(""));
    ui->tableWidget->setCurrentItem(newItem);
}

bool CMDRecipeItemsDialog::UI2Property(void *destProperty)
{
    CMDRecipeProperty_Config* dest = (CMDRecipeProperty_Config*)destProperty;
    dest->items()->deleteAll();

    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        QTableWidgetItem* tiID = ui->tableWidget->item(i, 0);
        QTableWidgetItem* tiValue = ui->tableWidget->item(i, 1);
        dest->items()->addItem(tiID->text(), tiValue->text());
    }

    return true;
}

void CMDRecipeItemsDialog::on_addButton_clicked()
{
    m_varWidget->setVisible(!m_varWidget->isVisible());
}

void CMDRecipeItemsDialog::on_removeButton_clicked()
{
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    foreach (QTableWidgetItem* twItem, items)
    {
        ui->tableWidget->setCurrentItem(twItem);
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    }
}
