#include "MDAlarmConditionGeneralDialog.h"
#include "ui_MDAlarmConditionGeneralDialog.h"
#include "MDAlarmCondition.h"
#include <QMessageBox>
#include "MDAlarmConditionTreeWindow.h"
#include "MDStringResource.h"

CMDAlarmConditionGeneralDialog::CMDAlarmConditionGeneralDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDAlarmConditionGeneralDialog)
{
    ui->setupUi(this);

    QStringList exs;
    exs << "QLineEdit";
    setLanguageExceptions(&exs);
}

CMDAlarmConditionGeneralDialog::~CMDAlarmConditionGeneralDialog()
{
    delete ui;
}

void CMDAlarmConditionGeneralDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CMDAlarmCondition* src = (CMDAlarmCondition*)sourceProperty;

    ui->nameEdit->setText(src->name());
    ui->descriptionEdit->setText(src->comment());
    ui->levelEdit->setText(src->levelName());
    ui->idEdit->setText(src->customId());
    ui->serveritySpinBox->setValue(src->serverity());
    ui->checkBox_delayed->setChecked(src->delayEnabled());
    ui->delayConditionEdit->setText(src->delayConditionExp());
    ui->delayedEdit->setText(src->delayTimeExp());
    ui->checkBox_latched->setChecked(src->latched());
    ui->checkBox_acknowledgeRequired->setChecked(src->acknowledgeRequired());
    ui->checkBox_enabled->setChecked(src->enabled());
    ui->enableAlarmEdit->setText(src->enableAlarmExp());

    if(!src->acknowledgeRequired()) {
        ui->checkBox_latched->setChecked(false);
        ui->checkBox_latched->setVisible(false);
    }
}

bool CMDAlarmConditionGeneralDialog::UI2Property(void *destProperty)
{
    if(ui->nameEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL, STRANS(STRING_ERROR), STRANS(STRING_NAME_IS_EMPTY), QMessageBox::Ok);
        return false;
    }

    CMDAlarmCondition* dest = (CMDAlarmCondition*)destProperty;

    dest->setName(ui->nameEdit->text());
    dest->setComment(ui->descriptionEdit->text());
    dest->setLevelName(ui->levelEdit->text());
    dest->setCustomId(ui->idEdit->text());
    dest->setServerity(ui->serveritySpinBox->value());
    dest->setDelayEnabled(ui->checkBox_delayed->isChecked());
    dest->setDelayConditionExp(ui->delayConditionEdit->text());
    dest->setDelayTimeExp(ui->delayedEdit->text());
    dest->setLatched(ui->checkBox_latched->isChecked());
    dest->setAcknowledgeRequired(ui->checkBox_acknowledgeRequired->isChecked());
    dest->setEnabled(ui->checkBox_enabled->isChecked());
    dest->setEnableAlarmExp(ui->enableAlarmEdit->text());

    CMDMultiLanguage::instance()->addWord(dest->comment(), false);

    return true;
}

void CMDAlarmConditionGeneralDialog::on_selectLevelButton_clicked()
{
    if(!objectUI())
        return;
    CMDAlarmConditionTreeWindow* treeWindow = (CMDAlarmConditionTreeWindow*)objectUI();

    QString dest;
    if(treeWindow->selectAlarmLevel(ui->levelEdit->text(), dest))
    {
        ui->levelEdit->setText(dest);
    }
}

void CMDAlarmConditionGeneralDialog::on_checkBox_acknowledgeRequired_toggled(bool checked)
{
    ui->checkBox_latched->setVisible(checked);
    if(!checked) {
        ui->checkBox_latched->setChecked(false);
    }
}
