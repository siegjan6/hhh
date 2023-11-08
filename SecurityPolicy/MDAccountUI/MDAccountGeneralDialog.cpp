#include "MDAccountGeneralDialog.h"
#include "ui_MDAccountGeneralDialog.h"
#include "MDAccountProperty_Config.h"
#include <QMessageBox>
#include "MDSecurityPolicyConfiger.h"
#include "MDStringResource.h"

CMDAccountGeneralDialog::CMDAccountGeneralDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDAccountGeneralDialog)
{
    ui->setupUi(this);
    m_spConfiger = NULL;

    QStringList exs;
    exs << "QLineEdit";
    setLanguageExceptions(&exs);
}

CMDAccountGeneralDialog::~CMDAccountGeneralDialog()
{
    delete ui;
}

void CMDAccountGeneralDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CMDAccountProperty_Config* src = (CMDAccountProperty_Config*)sourceProperty;

    ui->nameEdit->setText(src->name());
    ui->descriptionEdit->setText(src->comment());
    ui->enableCheckBox->setChecked(src->isEnable());
    ui->lockCheckBox->setChecked(src->isLocked());
}

bool CMDAccountGeneralDialog::UI2Property(void *destProperty)
{
    if(ui->nameEdit->text().isEmpty())
    {
        QMessageBox::warning(NULL, STRANS(STRING_ERROR), STRANS(STRING_NAME_IS_EMPTY), QMessageBox::Ok);
        return false;
    }

    quint8 minAccount = m_spConfiger->accountStrategyConfiger()->accountStrategy()->minAccountID();
    if(ui->nameEdit->text().length() < minAccount)
    {
        QString msg = STRANS(STRING_NAME_IS_TOO_SHORT) +"[" + QVariant(minAccount).toString() + "]";
        QMessageBox::warning(NULL, STRANS(STRING_ERROR),  msg, QMessageBox::Ok);
        return false;
    }

    CMDAccountProperty_Config* dest = (CMDAccountProperty_Config*)destProperty;

    dest->setName(ui->nameEdit->text());
    dest->setComment(ui->descriptionEdit->text());
    dest->setEnable(ui->enableCheckBox->isChecked());
    dest->setLocked(ui->lockCheckBox->isChecked());

    CMDMultiLanguage::instance()->addWord(dest->comment(), false);

    return true;
}
