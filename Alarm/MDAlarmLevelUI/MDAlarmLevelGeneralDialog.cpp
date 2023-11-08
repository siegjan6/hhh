#include "MDAlarmLevelGeneralDialog.h"
#include "ui_MDAlarmLevelGeneralDialog.h"
#include "MDAlarmLevel.h"
#include <QMessageBox>
#include "MDStringResource.h"

CMDAlarmLevelGeneralDialog::CMDAlarmLevelGeneralDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDAlarmLevelGeneralDialog)
{
    ui->setupUi(this);

    QStringList exs;
    exs << "QLineEdit";
    setLanguageExceptions(&exs);
}

CMDAlarmLevelGeneralDialog::~CMDAlarmLevelGeneralDialog()
{
    delete ui;
}

void CMDAlarmLevelGeneralDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CMDAlarmLevel* src = (CMDAlarmLevel*)sourceProperty;

    ui->nameEdit->setText(src->name());
    ui->descriptionEdit->setText(src->comment());
}

bool CMDAlarmLevelGeneralDialog::UI2Property(void *destProperty)
{
    if(ui->nameEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL, STRANS(STRING_ERROR), STRANS(STRING_NAME_IS_EMPTY), QMessageBox::Ok);
        return false;
    }

    CMDAlarmLevel* dest = (CMDAlarmLevel*)destProperty;

    dest->setName(ui->nameEdit->text());
    dest->setComment(ui->descriptionEdit->text());

    CMDMultiLanguage::instance()->addWord(dest->comment(), false);

    return true;
}
