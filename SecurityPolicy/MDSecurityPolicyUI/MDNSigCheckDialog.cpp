#include "MDNSigCheckDialog.h"
#include "ui_MDNSigCheckDialog.h"
#include "MDStringResource.h"

CMDNSigCheckDialog::CMDNSigCheckDialog(QWidget *parent) :
    CMDSecurityCheckDialog(parent),
    ui(new Ui::CMDNSigCheckDialog)
{
    ui->setupUi(this);
}

CMDNSigCheckDialog::~CMDNSigCheckDialog()
{
    delete ui;
}

void CMDNSigCheckDialog::initial(CMDSecurityPolicyExpert* spExpert, CMDActionSPSession* spSession)
{
    CMDSecurityCheckDialog::initial(spExpert, spSession);

    ui->operatorPasswordEdit->setEchoMode(QLineEdit::Password);

    QString typeText = CMDActionID::typeToString(m_spSession->actionSP().actionID().type());
    QString actionComment = typeText + " " + m_spSession->actionSP().actionID().objName();
    setWindowTitle(STRANS(STRING_NSIG) + " - " + actionComment);
}

void CMDNSigCheckDialog::on_operatorOKButton_clicked()
{
    if(!m_spExpert || !m_spSession)
        return;

    m_spSession->operatorInput().setSigID(ui->operatorAccountEdit->text());
    m_spSession->operatorInput().setSigPassword(ui->operatorPasswordEdit->text());
    m_spSession->operatorInput().setSigComment(ui->operatorCommentEdit->text());
    bool result = m_spExpert->checkSPSession_NSig(m_spSession);
    if(result)
    {
        setResult(QDialog::Accepted);
        accept();
    }
    else
    {
        showErrorMessageBox(ui->operatorAccountEdit->text(), m_spSession->operatorOutput().resultCode());
    }
}
