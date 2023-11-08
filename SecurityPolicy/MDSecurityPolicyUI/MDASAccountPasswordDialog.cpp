#include "MDASAccountPasswordDialog.h"
#include "ui_MDASAccountPasswordDialog.h"
#include "MDAccountStrategyConfiger.h"
#include <QMessageBox>
#include "MDStringResource.h"

CMDASAccountPasswordDialog::CMDASAccountPasswordDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDASAccountPasswordDialog)
{
    ui->setupUi(this);

    m_configer = NULL;

    QStringList exs;
    exs << "QLineEdit";
    setLanguageExceptions(&exs);
}

CMDASAccountPasswordDialog::~CMDASAccountPasswordDialog()
{
    delete ui;
}

void CMDASAccountPasswordDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    QVariant minAV(m_configer->accountStrategy()->minAccountID());
    QVariant minPV(m_configer->accountStrategy()->minPassword());
    QVariant isEnableDeleteAccount(m_configer->accountStrategy()->isEnableDeleteAccount());
    QVariant isEnableModifyPasswordFirstLogin(m_configer->accountStrategy()->isEnableModifyPasswordFirstLogin());
    QVariant containAlphabet(m_configer->accountStrategy()->containAlphabet());
    QVariant containNumber(m_configer->accountStrategy()->containNumber());
    QVariant containUppercaseLowercase(m_configer->accountStrategy()->containUppercaseLowercase());
    QVariant containExtraAlphabet(m_configer->accountStrategy()->containExtraAlphabet());
    QVariant cantUseOldPsw(m_configer->accountStrategy()->cantUseOldPsw());
    QVariant passwordInvalidContinueInputCount(m_configer->accountStrategy()->passwordInvalidContinueInputCount());
    QVariant passwordValidPeriod(m_configer->accountStrategy()->passwordValidPeriod());
    QVariant passwordValidPeriodUnit(m_configer->accountStrategy()->passwordValidPeriodUnit());
    QVariant autoUnlockPeriod(m_configer->accountStrategy()->autoUnlockPeriod());
    QVariant autoUnlockPeriodUnit(m_configer->accountStrategy()->autoUnlockPeriodUnit());
    QVariant lockScreenPeriod(m_configer->accountStrategy()->lockScreenPeriod());
    QVariant lockScreenPeriodUnit(m_configer->accountStrategy()->lockScreenPeriodUnit());
    QVariant pswCannotContainAccout(m_configer->accountStrategy()->pswCannotContainAccout());

    ui->minAccountLineEdit->setText(minAV.toString());
    ui->minPasswordLineEdit->setText(minPV.toString());
    ui->cb_deleteAccount->setChecked(isEnableDeleteAccount.toBool());
    ui->cb_modifyPasswordFirstLogin->setChecked(isEnableModifyPasswordFirstLogin.toBool());
    ui->cb_Alphabet->setChecked(containAlphabet.toBool());
    ui->cb_Number->setChecked(containNumber.toBool());
    ui->cb_UppercaseLowercase->setChecked(containUppercaseLowercase.toBool());
    ui->cb_Extra_Alphabet->setChecked(containExtraAlphabet.toBool());
    ui->cb_CantUseOldPsw->setChecked(!cantUseOldPsw.toBool());
    ui->spinBox_invalidPswCount->setValue(passwordInvalidContinueInputCount.toInt());
    ui->spinBox_PswValidPeriod->setValue(passwordValidPeriod.toInt());
    ui->comb_PswValidPeriodUnit->setCurrentIndex(passwordValidPeriodUnit.toInt());
    ui->spinBox_AutoUnlockPeriod->setValue(autoUnlockPeriod.toInt());
    ui->comb_AutoUnlockUnit->setCurrentIndex(autoUnlockPeriodUnit.toInt());
    ui->spinBox_LockScreenPeriod->setValue(lockScreenPeriod.toInt());
    ui->comb_LockScreenPeriodUnit->setCurrentIndex(lockScreenPeriodUnit.toInt());
    ui->cb_PswCannotContainAccout->setChecked(pswCannotContainAccout.toBool());

}

bool CMDASAccountPasswordDialog::UI2Property(void *destProperty)
{
    int minAccount = QVariant(ui->minAccountLineEdit->text()).toInt();
    if(minAccount < 1)
    {
        QMessageBox::warning(NULL, STRANS(STRING_ERROR), STRANS(STRING_MIN_ACCOUNT_MUST_BE_GREATER_THAN_1), QMessageBox::Ok);
        return false;
    }

    int minPassword = QVariant(ui->minPasswordLineEdit->text()).toInt();
    bool isEnableDeleteAccount = QVariant(ui->cb_deleteAccount->isChecked()).toBool();
    bool isModifyPasswordFirstLogin = QVariant(ui->cb_modifyPasswordFirstLogin->isChecked()).toBool();
    bool containAlphabet = QVariant(ui->cb_Alphabet->isChecked()).toBool();
    bool containNumber = QVariant(ui->cb_Number->isChecked()).toBool();
    bool containUppercaseLowercase = QVariant(ui->cb_Number->isChecked()).toBool();
    bool containExtraAlphabet = QVariant(ui->cb_Extra_Alphabet->isChecked()).toBool();
    bool cantUseOldPsw =  QVariant(ui->cb_CantUseOldPsw->isChecked()).toBool();
    int passwordInvalidContinueInputCount = QVariant(ui->spinBox_invalidPswCount->value()).toInt();
    int passwordValidPeriod = QVariant(ui->spinBox_PswValidPeriod->value()).toInt();
    int passwordValidPeriodUnit = QVariant(ui->comb_PswValidPeriodUnit->currentIndex()).toInt();
    int autoUnlockPeriod = QVariant(ui->spinBox_AutoUnlockPeriod->value()).toInt();
    int autoUnlockPeriodUnit = QVariant(ui->comb_AutoUnlockUnit->currentIndex()).toInt();
    int lockScreenPeriod = QVariant(ui->spinBox_LockScreenPeriod->value()).toInt();
    int lockScreenPeriodUnit = QVariant(ui->comb_LockScreenPeriodUnit->currentIndex()).toInt();
    bool pswCannotContainAccout =  QVariant(ui->cb_PswCannotContainAccout->isChecked()).toBool();

    m_configer->accountStrategy()->setMinAccountID((quint8)minAccount);
    m_configer->accountStrategy()->setMinPassword((quint8)minPassword);
    m_configer->accountStrategy()->setEnableDeleteAccount(isEnableDeleteAccount);
    m_configer->accountStrategy()->setEnableModifyPasswordFirstLogin(isModifyPasswordFirstLogin);
    m_configer->accountStrategy()->setContainAlphabet(containAlphabet);
    m_configer->accountStrategy()->setContainNumber(containNumber);
    m_configer->accountStrategy()->setContainUppercaseLowercase(containUppercaseLowercase);
    m_configer->accountStrategy()->setContainExtraAlphabet(containExtraAlphabet);
    m_configer->accountStrategy()->setCantUseOldPsw(!cantUseOldPsw);
    m_configer->accountStrategy()->setPasswordInvalidContinueInputCount(passwordInvalidContinueInputCount);
    m_configer->accountStrategy()->setPasswordValidPeriod(passwordValidPeriod);
    m_configer->accountStrategy()->setPasswordValidPeriodUnit(passwordValidPeriodUnit);
    m_configer->accountStrategy()->setAutoUnlockPeriod(autoUnlockPeriod);
    m_configer->accountStrategy()->setAutoUnlockPeriodUnit(autoUnlockPeriodUnit);
    m_configer->accountStrategy()->setLockScreenPeriod(lockScreenPeriod);
    m_configer->accountStrategy()->setLockScreenPeriodUnit(lockScreenPeriodUnit);
    m_configer->accountStrategy()->setPswCannotContainAccout(pswCannotContainAccout);


    return true;
}
