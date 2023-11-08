#include "MDAlarmConditionParaDialog.h"
#include "ui_MDAlarmConditionParaDialog.h"
#include "MDAlarmCondition.h"
#include <QMessageBox>
#include "MDAlarmConditionTreeWindow.h"
#include "MDStringResource.h"

CMDAlarmConditionParaDialog::CMDAlarmConditionParaDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDAlarmConditionParaDialog)
{
    ui->setupUi(this);

    QStringList exs;
    exs << "QLineEdit";
    setLanguageExceptions(&exs);
}

CMDAlarmConditionParaDialog::~CMDAlarmConditionParaDialog()
{
    delete ui;
}

void CMDAlarmConditionParaDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CMDAlarmCondition* src = (CMDAlarmCondition*)sourceProperty;

    ui->expressionEdit->setText(src->expression());
    ui->varNameEdit->setText(src->bindVar());
}

bool CMDAlarmConditionParaDialog::UI2Property(void *destProperty)
{
    CMDAlarmCondition* dest = (CMDAlarmCondition*)destProperty;

    dest->setExpression(ui->expressionEdit->text());
    dest->setBindVar(ui->varNameEdit->text());

    return true;
}

void CMDAlarmConditionParaDialog::on_selectObjectButton_clicked()
{
    if(!objectUI())
        return;

    CMDAlarmConditionTreeWindow* treeWindow = (CMDAlarmConditionTreeWindow*)objectUI();

    QString dest;
    if(treeWindow->selectVariable("", dest))
    {
        ui->varNameEdit->setText(dest);
    }
}
