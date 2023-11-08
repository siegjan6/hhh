#include "MDPowerGroupGeneralDialog.h"
#include "ui_MDPowerGroupGeneralDialog.h"
#include "MDPowerGroup.h"
#include <QMessageBox>
#include "MDStringResource.h"

CMDPowerGroupGeneralDialog::CMDPowerGroupGeneralDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDPowerGroupGeneralDialog)
{
    ui->setupUi(this);

    QStringList exs;
    exs << "QLineEdit";
    setLanguageExceptions(&exs);
}

CMDPowerGroupGeneralDialog::~CMDPowerGroupGeneralDialog()
{
    delete ui;
}

void CMDPowerGroupGeneralDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CMDPowerGroup* src = (CMDPowerGroup*)sourceProperty;

    ui->nameEdit->setText(src->name());
    ui->descriptionEdit->setText(src->comment());
    ui->spinBoxLevel->setValue(src->level());
    ui->enableCheckBox->setCheckState(src->isEnable() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    ui->visibleCheckBox->setCheckState(src->isVisible() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
}

bool CMDPowerGroupGeneralDialog::UI2Property(void *destProperty)
{
    if(ui->nameEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL, STRANS(STRING_ERROR), STRANS(STRING_NAME_IS_EMPTY), QMessageBox::Ok);
        return false;
    }

    CMDPowerGroup* dest = (CMDPowerGroup*)destProperty;

    dest->setName(ui->nameEdit->text());
    dest->setComment(ui->descriptionEdit->text());
    dest->setLevel(ui->spinBoxLevel->value());
    dest->setEnable(ui->enableCheckBox->checkState() == Qt::CheckState::Checked);
    dest->setVisible(ui->visibleCheckBox->checkState() == Qt::CheckState::Checked);

    CMDMultiLanguage::instance()->addWord(dest->comment(), false);

    return true;
}
