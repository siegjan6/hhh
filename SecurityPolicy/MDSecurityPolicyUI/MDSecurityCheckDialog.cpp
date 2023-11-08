#include "MDSecurityCheckDialog.h"
#include "ui_MDSecurityCheckDialog.h"
#include "MDStringResource.h"

CMDSecurityCheckDialog::CMDSecurityCheckDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDSecurityCheckDialog)
{
    //ui->setupUi(this);

    m_spExpert = NULL;
    m_spSession = NULL;

    m_twTool = CMDMultiLanguage::instance()->createWidgetTool();
    QStringList exs;
    exs << "QLineEdit";
    m_twTool->setExceptionTypes(exs);
}

CMDSecurityCheckDialog::~CMDSecurityCheckDialog()
{
    delete ui;

    if(m_twTool)
        delete m_twTool;
}

void CMDSecurityCheckDialog::initial(CMDSecurityPolicyExpert* spExpert, CMDActionSPSession* spSession)
{
    m_spExpert = spExpert;
    m_spSession = spSession;

    m_twTool->init(this);
    m_twTool->changeLanguage();
}

void CMDSecurityCheckDialog::showErrorMessageBox(const QString& account, MDSecurityResultCode code)
{
    switch (code)
    {
    case MDSecurityResultCode::no_result:
            QMessageBox::warning(this, STRANS(STRING_FAIL), STRANS(STRING_LOGIN_FAIL) + "[" + account + "]");
        break;
    case MDSecurityResultCode::account_not_exist:
            QMessageBox::warning(this, STRANS(STRING_FAIL), STRANS(STRING_ACCOUNT_NOT_EXIST) + "[" + account + "]");
        break;
    case MDSecurityResultCode::invalid_account:
            QMessageBox::warning(this, STRANS(STRING_FAIL), STRANS(STRING_INVALID_ACCOUNT) + "[" + account + "]");
        break;
    case MDSecurityResultCode::invalid_password:
            QMessageBox::warning(this, STRANS(STRING_FAIL), STRANS(STRING_WRONG_PASSWORD) + "[" + account + "]");
        break;
    case MDSecurityResultCode::account_has_no_power:
            QMessageBox::warning(this, STRANS(STRING_FAIL), STRANS(STRING_ACCOUNT_HAS_NO_POWER) + "[" + account + "]");
        break;
    case MDSecurityResultCode::account_disabled:
            QMessageBox::warning(this, STRANS(STRING_FAIL), STRANS(STRING_ACCOUNT_DISABLED) + "[" + account + "]");
        break;
    case MDSecurityResultCode::password_expired:
            QMessageBox::warning(this, STRANS(STRING_FAIL), STRANS(STRING_ACCOUNT_PASSWORD_EXPIRED) + "[" + account + "]");
        break;
    case MDSecurityResultCode::account_locked:
            QMessageBox::warning(this, STRANS(STRING_FAIL), STRANS(STRING_ACCOUNT_LOCKED) + "[" + account + "]");
        break;
    case MDSecurityResultCode::action_type_not_match:
            QMessageBox::warning(this, STRANS(STRING_FAIL), STRANS(STRING_ACTION_TYPE_NOT_MATCH) + "[" + account + "]");
        break;
    case MDSecurityResultCode::password_not_match_strategy:
            QMessageBox::warning(this, STRANS(STRING_FAIL), STRANS(STRING_ACCOUNT_PASSWORD_NOT_MATCH_STRATEGY) + "[" + account + "]");
        break;
    case MDSecurityResultCode::cannot_use_oldpassword:
            QMessageBox::warning(this, STRANS(STRING_FAIL), STRANS(STRING_ACCOUNT_CANNOT_USE_OLDPASSWORD) + "[" + account + "]");
        break;
    case MDSecurityResultCode::first_login_modify_password:
            QMessageBox::warning(this, STRANS(STRING_FAIL), STRANS(STRING_ACCOUNT_FIRST_LOGIN_MODIFY_PASSWORD) + "[" + account + "]");
        break;
    default:
        {
            QString msg = STRANS(STRING_ERROR_CODE) + " = " + QVariant((int)code).toString();
            QMessageBox::warning(this, STRANS(STRING_FAIL), msg);
        }
    }
}

void CMDSecurityCheckDialog::on_cancelButton_clicked()
{
    setResult(QDialog::Rejected);
    reject();
}
