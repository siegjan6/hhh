#include "MDSecurityPolicyExpert.h"
#include "MDSecurityPolicyExpertImp.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

CMDSecurityPolicyExpert::CMDSecurityPolicyExpert()
{
    m_imp = new CMDSecurityPolicyExpertImp;
}

CMDSecurityPolicyExpert::~CMDSecurityPolicyExpert()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化与释放
*/
void CMDSecurityPolicyExpert::setParaBeforeLoad(const QString& pid, CMDSecurityPolicyConfiger* configer, CMDSecurityPolicyContacter* contacter)
{
    if(m_imp)
        m_imp->setParaBeforeLoad(pid, configer, contacter);
}

void CMDSecurityPolicyExpert::load()
{
    if(m_imp)
        m_imp->load();
}
void CMDSecurityPolicyExpert::unload()
{
    if(m_imp)
        m_imp->unload();
}

/*!
\section 登录
*/
MDSecurityResultCode CMDSecurityPolicyExpert::login(const QString& account, const QString& password, int checkGroup/* = 0*/)
{
    return (m_imp ? m_imp->login(account, password, checkGroup) : MDSecurityResultCode::no_result);
}

void CMDSecurityPolicyExpert::loginByAccount(const QString& name)
{
    if(m_imp)
         m_imp->loginByAccount(name);
}

bool CMDSecurityPolicyExpert::login1(const QString& account, const QString& password, int checkGroup/* = 0*/)
{
    return (MDSecurityResultCode::success == login(account, password, checkGroup));
}

void CMDSecurityPolicyExpert::logout()
{
    if(m_imp)
        m_imp->logout();
}

QString CMDSecurityPolicyExpert::currentAccount()
{
    return (m_imp ? m_imp->currentAccount() : "");
}

MDSecurityResultCode CMDSecurityPolicyExpert::currentLoginResult()
{
    return (m_imp ? m_imp->currentLoginResult() : MDSecurityResultCode::no_result);
}

/*!
\section 安全验证
*/
bool CMDSecurityPolicyExpert::isCurrentAccountHasPower(CMDActionSPSession* spSession)
{
    return (m_imp ? m_imp->isCurrentAccountHasPower(spSession) : false);
}

bool CMDSecurityPolicyExpert::checkSPSession_NSig(CMDActionSPSession* spSession)
{
    return (m_imp ? m_imp->checkSPSession_NSig(spSession) : false);
}

bool CMDSecurityPolicyExpert::checkSPSession_ESig_Operator(CMDActionSPSession* spSession)
{
    return (m_imp ? m_imp->checkSPSession_ESig_Operator(spSession) : false);
}

bool CMDSecurityPolicyExpert::checkSPSession_ESig_Verifier(CMDActionSPSession* spSession)
{
    return (m_imp ? m_imp->checkSPSession_ESig_Verifier(spSession) : false);
}

CMDSecurityPolicyConfiger* CMDSecurityPolicyExpert::configer()
{
    return (m_imp ? m_imp->m_configer : NULL);
}

int CMDSecurityPolicyExpert::accountsCount()
{
    return (m_imp ? m_imp->accountsCount() : 0);
}
QStringList CMDSecurityPolicyExpert::getAccounts()
{
    return (m_imp ? m_imp->getAccounts() : QStringList());
}
QStringList CMDSecurityPolicyExpert::getAccountGroups(QString name)
{
    return (m_imp ? m_imp->getAccountGroups(name) : QStringList());
}
QStringList CMDSecurityPolicyExpert::getDisabledAccounts()
{
    return (m_imp ? m_imp->getDisabledAccounts() : QStringList());
}
QStringList CMDSecurityPolicyExpert::getLockedAccounts()
{
    return (m_imp ? m_imp->getLockedAccounts() : QStringList());
}
bool CMDSecurityPolicyExpert::enableAccount(QString name)
{
    return (m_imp ? m_imp->enableAccount(name) : 0);
}
bool CMDSecurityPolicyExpert::disableAccount(QString name)
{
    return (m_imp ? m_imp->disableAccount(name) : 0);
}
int CMDSecurityPolicyExpert::deleteAccount(QString name)
{
    return (m_imp ? m_imp->deleteAccount(name) : 0);
}
bool CMDSecurityPolicyExpert::lockAccount(QString name)
{
    return (m_imp ? m_imp->lockAccount(name) : 0);
}
bool CMDSecurityPolicyExpert::unlockAccount(QString name)
{
    return (m_imp ? m_imp->unlockAccount(name) : 0);
}
bool CMDSecurityPolicyExpert::isAccountEnabled(QString name)
{
    return (m_imp ? m_imp->isAccountEnabled(name) : 0);
}
bool CMDSecurityPolicyExpert::isAccountLocked(QString name)
{
    return (m_imp ? m_imp->isAccountLocked(name) : 0);
}
bool CMDSecurityPolicyExpert::isAccountLogin(QString name)
{
    return (m_imp ? m_imp->isAccountLogin(name) : 0);
}
bool CMDSecurityPolicyExpert::upsertAccount(QString name, QString comment, QString password, bool enable, bool lock)
{
    return (m_imp ? m_imp->upsertAccount(name, comment, password, enable, lock) : 0);
}


int CMDSecurityPolicyExpert::groupsCount()
{
    return (m_imp ? m_imp->groupsCount() : 0);
}
QString CMDSecurityPolicyExpert::getGroup(QString name)
{
    return (m_imp ? m_imp->getGroup(name) : "");
}
QStringList CMDSecurityPolicyExpert::getGroups()
{
    return (m_imp ? m_imp->getGroups() : QStringList());
}
QStringList CMDSecurityPolicyExpert::getAccountsInGroup(QString group)
{
    return (m_imp ? m_imp->getAccountsInGroup(group) : QStringList());
}
QStringList CMDSecurityPolicyExpert::getDisabledGroups()
{
    return (m_imp ? m_imp->getDisabledGroups() : QStringList());
}
bool CMDSecurityPolicyExpert::enableGroup(QString name)
{
    return (m_imp ? m_imp->enableGroup(name) : 0);
}
bool CMDSecurityPolicyExpert::disableGroup(QString name)
{
    return (m_imp ? m_imp->disableGroup(name) : 0);
}
bool CMDSecurityPolicyExpert::deleteGroup(QString name)
{
    return (m_imp ? m_imp->deleteGroup(name) : 0);
}
// modifyGroupByUI()
bool CMDSecurityPolicyExpert::addAccountToGroup(QString account,QString group)
{
    return (m_imp ? m_imp->addAccountToGroup(account, group) : 0);
}
bool CMDSecurityPolicyExpert::removeAccountFromGroup(QString account,QString group)
{
    return (m_imp ? m_imp->removeAccountFromGroup(account, group) : 0);
}
bool CMDSecurityPolicyExpert::isAccountInGroup(QString account,QString group)
{
    return (m_imp ? m_imp->isAccountInGroup(account, group) : 0);
}
bool CMDSecurityPolicyExpert::upsertGroup(QString name, QString comment, bool enable)
{
    return (m_imp ? m_imp->upsertGroup(name, comment, enable) : 0);
}
QString CMDSecurityPolicyExpert::getAccount(QString account)
{
    return (m_imp ? m_imp->getAccount(account) : "");
}
int CMDSecurityPolicyExpert::modifyPassword(QString account, QString oldPassword, QString newPassword)
{
    return (m_imp ? m_imp->modifyPassword(account, oldPassword, newPassword) : -1);
}
bool CMDSecurityPolicyExpert::resetAccount(QString name)
{
    return (m_imp ? m_imp->resetAccount(name) : 0);
}
bool CMDSecurityPolicyExpert::resetAllAccount()
{
    return (m_imp ? m_imp->resetAllAccount() : 0);
}


QString CMDSecurityPolicyExpert::getAccountOption()
{
    return (m_imp ? m_imp->getAccountOption() : "");
}
bool CMDSecurityPolicyExpert::setAccountOption(QString option)
{
    return (m_imp ? m_imp->setAccountOption(option) : 0);
}

bool CMDSecurityPolicyExpert::logSPSession(CMDActionSPSession* spSession)
{
    return (m_imp ? m_imp->logSPSession(spSession) : 0);
}

bool CMDSecurityPolicyExpert::commit()
{
    return (m_imp ? m_imp->commit() : 0);
}


int CMDSecurityPolicyExpert::accountSignature(QString actionInfo, QString sigConf)
{

    // 1. 运行时当前实时签名信息
//    {
//        "account": "u1",
//        "password": "", //optional 动作保护时可不填密码
//        "actionType": 0, // 动作类型,系统预定义(如 打开窗口，复位报警 ...) 需要与设计时保存的保持一致，否则报动作类型不匹配的错误 参考 enum MDActionType
//        "customAction": "XXX", // 动作999 对应的描述
//        "customObjectName": "XXX", // 对象999 对应的对象名
//        "srcState": "XXX", // 当改变变量值时 的原始值
//        "destState": "XXX", // 当改变变量值时 的目标值
//        "actionComment": "YYY", // 本次签名备注
//        "customComment": "ZZZ", // 用户自定义信息 若此项不为空则日志记录按照自定义信息为准，而不使用系统标准记录模式
//obsolete      "actionSrc": "XXX", // 便于最后形成 日志信息(如 某某某 yyyy-MM-dd HH:mm:ss 打开窗口 XXX)， 按照系统标准记录方式记录， 此项非空按照此项设置替换动作对象名并记录至数据库，对应字段为Name
//        "protectMode": 0 // 0-动作保护 1-电子签名(操作人) 2-电子签名(校验人)
//    }
    // 2. 设计时签名配置
    // if 动作保护
    // else 电子签名
//    {
//        "@EL": true, // 启用日志记录
//        "@OKI": {
//            "@Comment": "",
//            "@N": "w", // 动作名 , 当类型是用户自定义类型999时后台以此作为动作名记录，其余为预定义动作情况，无需填此值  eg:执行动作: @N (@Comment) [签名成功]
//            "@OT": 9, // 对象类型 MDObjectType 999为自定义对象
//            "@T": 113 // 动作类型 MDActionType 999为自定义动作
//        },
//        "@PM": 2, // 保护模式 0-无 1-动作保护 2-电子签名
//        "@ST": 2, // 签名类型 0-无 1-operator_only 2-operator_then_verifier
//        "WorkerGroupArray": [  // 操作组
//            "*", // *组代表系统所有组
//            "Manufacture",
//            "Engineers"
//        ],
//        "InspectorGroupArray": [   // 校验组
//            "Administrators",
//            "Engineers"
//        ]
//    }

    CMDActionSP asp;
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(sigConf.toUtf8(), &error);
    if(error.error == QJsonParseError::NoError){
        if(doc.isObject()){
            QJsonObject json = doc.object();
            asp.deserialize(json);
            if(asp.isEnable()) {
                QJsonDocument doc_act = QJsonDocument::fromJson(actionInfo.toUtf8(), &error);
                if(error.error == QJsonParseError::NoError){
                    if(doc_act.isObject()){
                        QJsonObject json_act = doc_act.object();
                        QString account = "";
                        if(json_act.contains("account"))
                            account = json_act.value("account").toString();
                        QString password = "";
                        if(json_act.contains("password"))
                            password = json_act.value("password").toString();
                        int actionType = 0;
                        if(json_act.contains("actionType"))
                            actionType = json_act.value("actionType").toInt();
                        if((int)asp.actionID().type() != actionType)
                            return (int)MDSecurityResultCode::action_type_not_match;
                        QString customAction = "";
                        if(json_act.contains("customAction"))
                            customAction = json_act.value("customAction").toString();
                        asp.actionID().setCustomAction(customAction);
                        QString customObjectName = "";
                        if(json_act.contains("customObjectName"))
                            customObjectName = json_act.value("customObjectName").toString();
                        asp.actionID().setCustomObjectName(customObjectName);
                        QString srcState = "";
                        if(json_act.contains("srcState"))
                            srcState = json_act.value("srcState").toString();
                        asp.actionID().setSrcState(srcState);
                        QString destState = "";
                        if(json_act.contains("destState"))
                            destState = json_act.value("destState").toString();
                        asp.actionID().setDestState(destState);
                        QString actionComment = "";
                        if(json_act.contains("actionComment"))
                            actionComment = json_act.value("actionComment").toString();
                        QString customComment = "";
                        if(json_act.contains("customComment"))
                            customComment = json_act.value("customComment").toString();
                        asp.actionID().setCustomComment(customComment);
                        int protectMode = 0;
                        if(json_act.contains("protectMode"))
                            protectMode = json_act.value("protectMode").toInt();

                        CMDActionSPSession sps(asp);
                        CMDSigInput si;
                        si.setSigID(account);
                        si.setSigPassword(password);
                        si.setSigComment(actionComment);

                        if(protectMode == 0 && asp.protectMode() == MDActionProtectMode::MindSCADA_Sig){
                            // 动作保护
                            si.setNoPasswordNeeded(true);
                            foreach(QString g, asp.operatorGroupList())
                            {
                                int accountInGroups_cnt = 0;
                                if(isAccountInGroup(currentAccount(), g))
                                {
                                    accountInGroups_cnt ++;
                                }
                                // not in any configured operatorGroupList
                                if(accountInGroups_cnt == 0)
                                    si.setNoPasswordNeeded(false);
                            }

                            sps.setOperatorInput(si);
                            CMDSigOutput so;
                            sps.setOperatorOutput(so);
                            bool b = checkSPSession_NSig(&sps);
                            //if(b){
                                so = sps.operatorOutput();
                                return (int)so.resultCode();
                            //}

                        }
                        else if(asp.protectMode() == MDActionProtectMode::Electronic_Sig){
                            // 电子签名
                            si.setNoPasswordNeeded(false);

                            if(protectMode == 1 && (asp.sigType() == MDSigType::operator_only || asp.sigType() == MDSigType::operator_then_verifier)) {
                                // 操作人
                                sps.setOperatorInput(si);
                                CMDSigOutput so;
                                sps.setOperatorOutput(so);
                                bool b = checkSPSession_ESig_Operator(&sps);
                                //if(b){
                                    so = sps.operatorOutput();
                                    return (int)so.resultCode();
                                //}
                            }
                            else if(protectMode == 2 && asp.sigType() == MDSigType::operator_then_verifier){
                                // 校验人
                                sps.setVerifierInput(si);
                                CMDSigOutput so;
                                sps.setVerifierOutput(so);
                                bool b = checkSPSession_ESig_Verifier(&sps);
                                //if(b){
                                    so = sps.verifierOutput();
                                    return (int)so.resultCode();
                                //}
                            }
                        }
                    }
                }
            }
        }
    }

    return -1;
}


bool CMDSecurityPolicyExpert::userValidation(const QString &name, const QString &password)
{
    qDebug() << "userValidation=" << (int)m_imp->userValidation(name, password);
    return (m_imp ? m_imp->userValidation(name, password) == MDSecurityResultCode::success : false);
}

//void CMDESigCheckDialog::on_operatorOKButton_clicked()
//{
//    if(!m_spExpert || !m_spSession)
//        return;

//    m_spSession->operatorInput().setSigID(ui->operatorAccountEdit->text());
//    m_spSession->operatorInput().setSigPassword(ui->operatorPasswordEdit->text());
//    m_spSession->operatorInput().setSigComment(ui->operatorCommentEdit->text());
//    bool result = m_spExpert->checkSPSession_ESig_Operator(m_spSession);
//    if(result)
//    {
//        if(MDSigType::operator_only == m_spSession->actionSP().sigType())
//        {
//            setResult(QDialog::Accepted);
//            accept();
//        }
//        else if(MDSigType::operator_then_verifier == m_spSession->actionSP().sigType())
//        {
//            ui->operatorAccountEdit->setEnabled(false);
//            ui->operatorPasswordEdit->setEnabled(false);
//            ui->operatorCommentEdit->setEnabled(false);
//            ui->operatorOKButton->setEnabled(false);

//            ui->verifierAccountEdit->setEnabled(true);
//            ui->verifierPasswordEdit->setEnabled(true);
//            ui->verifierCommentEdit->setEnabled(true);
//            ui->verifierOKButton->setEnabled(true);
//        }
//    }
//    else
//    {
//        showErrorMessageBox(ui->operatorAccountEdit->text(), m_spSession->operatorOutput().resultCode());
//    }
//}

//void CMDESigCheckDialog::on_verifierOKButton_clicked()
//{
//    if(!m_spExpert || !m_spSession)
//        return;

//    m_spSession->verifierInput().setSigID(ui->verifierAccountEdit->text());
//    m_spSession->verifierInput().setSigPassword(ui->verifierPasswordEdit->text());
//    m_spSession->verifierInput().setSigComment(ui->verifierCommentEdit->text());
//    bool result = m_spExpert->checkSPSession_ESig_Verifier(m_spSession);
//    if(result)
//    {
//        setResult(QDialog::Accepted);
//        accept();
//    }
//    else
//    {
//        showErrorMessageBox(ui->verifierAccountEdit->text(), m_spSession->verifierOutput().resultCode());
//    }
//}
