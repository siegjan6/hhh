#include "MDLoginDialog.h"
#include "ui_MDLoginDialog.h"
#include "MDStringResource.h"

CMDLoginDialog::CMDLoginDialog(QWidget *parent) :
    CMDSecurityCheckDialog(parent),
    ui(new Ui::CMDLoginDialog)
{
    ui->setupUi(this);

    ui->operatorPasswordEdit->setEchoMode(QLineEdit::Password);

    setWindowTitle(STRANS(STRING_LOGIN));
}

CMDLoginDialog::~CMDLoginDialog()
{
    delete ui;
}

void CMDLoginDialog::on_operatorOKButton_clicked()
{
    MDSecurityResultCode loginResult = (m_spExpert ?
                            m_spExpert->login(ui->operatorAccountEdit->text(), ui->operatorPasswordEdit->text()) :
                            MDSecurityResultCode::no_result);

    if(loginResult == MDSecurityResultCode::success)
    {
        setResult(QDialog::Accepted);
        accept();
    }
    else
    {
        showErrorMessageBox(ui->operatorAccountEdit->text(), loginResult);
    }
}
