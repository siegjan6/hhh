#include "MDAccountPasswordDialog.h"
#include "ui_MDAccountPasswordDialog.h"
#include "MDAccountProperty_Config.h"
#include <QMessageBox>
#include "MDSecurityPolicyConfiger.h"
#include "MDStringResource.h"

CMDAccountPasswordDialog::CMDAccountPasswordDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDAccountPasswordDialog)
{
    ui->setupUi(this);
    m_spConfiger = NULL;

    QStringList exs;
    exs << "QLineEdit";
    setLanguageExceptions(&exs);
}

CMDAccountPasswordDialog::~CMDAccountPasswordDialog()
{
    delete ui;
}

void CMDAccountPasswordDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CMDAccountProperty_Config* src = (CMDAccountProperty_Config*)sourceProperty;

    ui->passwordEdit->setText(src->password());
}

bool CMDAccountPasswordDialog::UI2Property(void *destProperty)
{
    CMDAccountProperty_Config* dest = (CMDAccountProperty_Config*)destProperty;

    quint8 minPassword = m_spConfiger->accountStrategyConfiger()->accountStrategy()->minPassword();
    if(ui->passwordEdit->text().length() < minPassword)
    {
        QString msg = STRANS(STRING_PASSWORD_IS_TOO_SHORT) +"[" + QVariant(minPassword).toString() + "]";
        QMessageBox::warning(NULL, STRANS(STRING_ERROR),  msg, QMessageBox::Ok);
        return false;
    }

    dest->setPassword(ui->passwordEdit->text());

    return true;
}
