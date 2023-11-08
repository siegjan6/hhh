#include "MDESigCheckDialog.h"
#include "ui_MDESigCheckDialog.h"
#include "MDStringResource.h"

CMDESigCheckDialog::CMDESigCheckDialog(QWidget *parent) :
    CMDSecurityCheckDialog(parent),
    ui(new Ui::CMDESigCheckDialog)
{
    ui->setupUi(this);

    setWindowTitle(STRANS(STRING_ESIG));
}

CMDESigCheckDialog::~CMDESigCheckDialog()
{
    delete ui;
}

void CMDESigCheckDialog::initial(CMDSecurityPolicyExpert* spExpert, CMDActionSPSession* spSession)
{
    CMDSecurityCheckDialog::initial(spExpert, spSession);

    ui->operatorPasswordEdit->setEchoMode(QLineEdit::Password);
    ui->verifierPasswordEdit->setEchoMode(QLineEdit::Password);

    QString typeText = CMDActionID::typeToString(m_spSession->actionSP().actionID().type());
    QString actionComment = typeText + " " + m_spSession->actionSP().actionID().objName();
    setWindowTitle(STRANS(STRING_ESIG)+ " - " + actionComment);
}

void CMDESigCheckDialog::on_operatorOKButton_clicked()
{
    if(!m_spExpert || !m_spSession)
        return;

    m_spSession->operatorInput().setSigID(ui->operatorAccountEdit->text());
    m_spSession->operatorInput().setSigPassword(ui->operatorPasswordEdit->text());
    m_spSession->operatorInput().setSigComment(ui->operatorCommentEdit->text());
    bool result = m_spExpert->checkSPSession_ESig_Operator(m_spSession);
    if(result)
    {
        if(MDSigType::operator_only == m_spSession->actionSP().sigType())
        {
            setResult(QDialog::Accepted);
            accept();
        }
        else if(MDSigType::operator_then_verifier == m_spSession->actionSP().sigType())
        {
            ui->operatorAccountEdit->setEnabled(false);
            ui->operatorPasswordEdit->setEnabled(false);
            ui->operatorCommentEdit->setEnabled(false);
            ui->operatorOKButton->setEnabled(false);

            ui->verifierAccountEdit->setEnabled(true);
            ui->verifierPasswordEdit->setEnabled(true);
            ui->verifierCommentEdit->setEnabled(true);
            ui->verifierOKButton->setEnabled(true);
        }
    }
    else
    {
        showErrorMessageBox(ui->operatorAccountEdit->text(), m_spSession->operatorOutput().resultCode());
    }
}

void CMDESigCheckDialog::on_verifierOKButton_clicked()
{
    if(!m_spExpert || !m_spSession)
        return;

    m_spSession->verifierInput().setSigID(ui->verifierAccountEdit->text());
    m_spSession->verifierInput().setSigPassword(ui->verifierPasswordEdit->text());
    m_spSession->verifierInput().setSigComment(ui->verifierCommentEdit->text());
    bool result = m_spExpert->checkSPSession_ESig_Verifier(m_spSession);
    if(result)
    {
        setResult(QDialog::Accepted);
        accept();
    }
    else
    {
        showErrorMessageBox(ui->verifierAccountEdit->text(), m_spSession->verifierOutput().resultCode());
    }
}
