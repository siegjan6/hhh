#include "MDSecurityPolicyExpertImp.h"
#include "MDSecurityPolicyContacter.h"
#include "MDSecurityPolicyConfiger.h"
#include "MDAccountConfiger.h"
#include "MDAccount.h"
#include "MDPowerGroup.h"
#include "MDActionSPSession.h"
#include "MDLogMessage.h"
#include <QRegularExpression>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#ifdef _WIN32
    #include "MMKV/MMKV.h"
#elif defined(__linux__)
    #include "MMKV.h"
#endif

/////////////////////////////////////////////////////
/// \brief mmkv postfix description
/// [account]                    - 账户连续错误登陆次数 = (int32_t)0
/// [account]_logined            - 第一次登陆成功后 = (bool)true
/// [account]_lock               - 账户运行时违反安全策略后锁定标记 = (int32_t)1
/// [account]_lock_time          - 账户运行时违反安全策略后锁定时间戳 = (int64_t) unix timestamp seconds
/// [account]_psw_modify_time    - 账户成功修改密码的时间戳,作为账户密码过期时间判断原点,新设备第一次运行且未成功进行过修改密码操作，把第一次登录成功的时间戳写入此key = (int64_t) unix timestamp seconds
/// [account]_usedpsw            - 账户成功修改过的密码历史 = std::string
/// [account]_curpsw             - 账户成功登录的当前密码 = std::string
/// [account]_changedpsw         - 账户运行时最近一次成功修改的密码(此配置放在项目路径下[prjpath]/SecurityPolicy/__security__),存在则使用此密码来验证，不存在使用项目配置默认密码 = std::string


CMDSecurityPolicyExpertImp::CMDSecurityPolicyExpertImp()
{
    m_configer = NULL;
    m_contacter = NULL;
    m_pid = "";
    m_currentLoginResult = MDSecurityResultCode::no_result;
}

CMDSecurityPolicyExpertImp::~CMDSecurityPolicyExpertImp()
{

}

/*!
\section 初始化与释放
*/
void CMDSecurityPolicyExpertImp::setParaBeforeLoad(const QString& pid, CMDSecurityPolicyConfiger* configer, CMDSecurityPolicyContacter* contacter)
{
    m_pid = pid;
    m_configer = configer;
    m_contacter = contacter;
}

void CMDSecurityPolicyExpertImp::load()
{
    loadKV();
}

void CMDSecurityPolicyExpertImp::unload()
{
    if(m_mmkv)
        m_mmkv->onExit();
}

/*!
\section 登录
*/
MDSecurityResultCode CMDSecurityPolicyExpertImp::login(const QString& account, const QString& password, int checkGroup/* = 0*/)
{ 
    if(checkGroup >0)
    {
        // 强制验证 账户是否已分配至权限组
        QStringList grps = getGroups();
        if(MDSecurityResultCode::account_has_no_power == checkPower(account,grps))
        {
            return MDSecurityResultCode::account_has_not_assign_to_power;
        }
    }
    MDSecurityResultCode result = checkAccount(account, password, false);
    if(MDSecurityResultCode::success == result)
    {
        m_currentAccount.setSigID(account);
        m_currentAccount.setSigPassword(password);
    }

    QString retMsg;
    log_login(account, result, retMsg);
    m_contacter->onLogin(account, (int)result, retMsg);
    m_currentLoginResult = result;
    return result;
}

void CMDSecurityPolicyExpertImp::loginByAccount(const QString& name)
{
    MDSecurityResultCode result = checkAccount(name, "", true);
    if(MDSecurityResultCode::success == result)
    {
        m_currentAccount.setSigID(name);
        CMDAccountProperty_Config* accountConfig = m_configer->accountConfiger()->find(name);
        if(accountConfig)
        {
            if(accountConfig->isEnable())
            {
                m_currentAccount.setSigPassword(accountConfig->password());
                if(m_mmkv_by_prj){
                    std::string std_runtimeChangedPsw = "";
                    std::string std_account = name.toStdString();
                    m_mmkv_by_prj->getString(std_account+"_changedpsw", std_runtimeChangedPsw);
                    if(!std_runtimeChangedPsw.empty())
                        m_currentAccount.setSigPassword(QString::fromStdString(std_runtimeChangedPsw));
                }
            }
        }

        m_currentLoginResult = MDSecurityResultCode::success;
    }
}

bool CMDSecurityPolicyExpertImp::log_login(const QString& account, MDSecurityResultCode result, QString& retMsg)
{
    if(!m_contacter)
        return false;

    CMDLogMessage msg;
//    if(MDSecurityResultCode::success == result)
//    {
//        msg.setMsg(account + " 登录成功！");
//    }
//    else
//    {
//        msg.setMsg(account + " 登录失败！- 错误码：" + QVariant((int)result).toString());
//    }

    QString errMsg = getAuthErrorMsg(result);

    msg.setMsg(account + " " + errMsg);
    msg.setCurrentUser(account);
    m_contacter->log(&msg);
    retMsg = account + " " + errMsg;
    return true;
}

QString CMDSecurityPolicyExpertImp::getAuthErrorMsg(MDSecurityResultCode errcode)
{
    QString errMsg = "";
    switch(errcode) {
        case MDSecurityResultCode::no_result:
            errMsg = "登录失败";
            break;
        case MDSecurityResultCode::success:
            errMsg = "登录成功";
            break;
        case MDSecurityResultCode::account_not_exist:
            errMsg = "帐户不存在";
            break;
        case MDSecurityResultCode::invalid_account:
            errMsg = "账户无效";
            break;
        case MDSecurityResultCode::invalid_password:
            errMsg = "账户密码错误";
            break;
        case MDSecurityResultCode::account_has_no_power:
            errMsg = "帐户不具有权限";
            break;
        case MDSecurityResultCode::account_disabled:
            errMsg = "帐户被禁用";
            break;
        case MDSecurityResultCode::password_expired:
            errMsg = "账户密码已过期,请及时修改密码";
            break;
        case MDSecurityResultCode::account_locked:
            errMsg = "帐户被锁定,需要管理员手动解锁";
            break;
        case MDSecurityResultCode::action_type_not_match:
            errMsg = "验证动作类型与预设类型不匹配";
            break;
        case MDSecurityResultCode::password_not_match_strategy:
            errMsg = "密码不符合安全策略";
            break;
        case MDSecurityResultCode::cannot_use_oldpassword:
            errMsg = "不可使用旧密码";
            break;
        case MDSecurityResultCode::first_login_modify_password:
            errMsg = "第一次登陆必须修改密码";
            break;
        case MDSecurityResultCode::account_has_not_assign_to_power:
            errMsg = "账户未分配至任何权限组";
            break;
    }
    return errMsg;
}

void CMDSecurityPolicyExpertImp::logout()
{
    CMDLogMessage msg;
    msg.setMsg(m_currentAccount.sigID() + " 注销");
    msg.setCurrentUser(m_currentAccount.sigID());
    m_contacter->log(&msg);
    m_contacter->onLogout(m_currentAccount.sigID());
    m_currentAccount.setSigID("");
    m_currentAccount.setSigPassword("");
}

QString CMDSecurityPolicyExpertImp::currentAccount()
{
    return m_currentAccount.sigID();
}

MDSecurityResultCode CMDSecurityPolicyExpertImp::currentLoginResult()
{
    return m_currentLoginResult;
}

/*!
\section 安全验证
*/

bool CMDSecurityPolicyExpertImp::checkSPSession_NSig(CMDActionSPSession* spSession)
{
    if(!m_configer->para()->isEnableSP())
        return true;

    if(isCurrentAccountHasPower(spSession))
    {
        // 具备权限直接通过
        QString resultMsg = spSession->actionSP().actionID().getLogMsg();
        //QString resultMsg = spSession->operatorOutput().resultMsg();
        resultMsg += "[动作保护,无需签名,因为当前账户具备权限]";
        spSession->operatorOutput().setResultMsg(resultMsg);
        spSession->operatorOutput().setResultCode(MDSecurityResultCode::success);
        spSession->operatorInput().setSigID(currentAccount());
        logSPSession(spSession);

        return true;
    }

    bool result = helper_CheckSPSession_Operator(spSession);

    if(spSession->actionSP().isEnableLog())
    {
        log_NSig(spSession, result);
    }

    return result;
}

bool CMDSecurityPolicyExpertImp::isCurrentAccountHasPower(CMDActionSPSession* spSession)
{
    bool result = false;
    if(MDActionProtectMode::MindSCADA_Sig == spSession->actionSP().protectMode())
    {
        CMDActionSPSession spSessionForCurrentAccount(*spSession);
        spSessionForCurrentAccount.setOperatorInput(m_currentAccount);
        result = helper_CheckSPSession_Operator(&spSessionForCurrentAccount);
    }
    return result;
}

bool CMDSecurityPolicyExpertImp::log_NSig(CMDActionSPSession* spSession, bool result)
{
    if(!m_contacter)
        return false;

    CMDLogMessage msg;

    if(result)
    {
        QString resultMsg = spSession->actionSP().actionID().getLogMsg();
        resultMsg += " 保护签名成功！";
        spSession->operatorOutput().setResultMsg(resultMsg);
        return logSPSession(spSession);
        //msg.setMsg("签名成功！");
    }
    else
    {
        MDSecurityResultCode result = spSession->operatorOutput().resultCode();
        QString resultMsg = spSession->actionSP().actionID().getLogMsg();
        QString errcode = getAuthErrorMsg(result);
        resultMsg += QString(" 保护签名失败！[%1]").arg(errcode);
        spSession->operatorOutput().setResultMsg(resultMsg);
        return logSPSession(spSession);
        //msg.setMsg("签名失败！- 错误码：" + QVariant((int)result).toString());
        //msg.setMsg(errcode);
    }
    msg.setSpSession(spSession);
    msg.setCurrentUser(currentAccount());
    m_contacter->log(&msg);
    return true;
}

bool CMDSecurityPolicyExpertImp::logSPSession(CMDActionSPSession* spSession)
{
    if(!m_contacter)
        return false;

    CMDLogMessage msg;
    if(spSession->actionSP().protectMode() == MDActionProtectMode::MindSCADA_Sig
            && spSession->operatorInput().sigID() != "")
        spSession->operatorOutput().setSigTime(msg.dt());
    msg.setMsg(spSession->operatorOutput().resultMsg());
    if(spSession->actionSP().protectMode() == MDActionProtectMode::Electronic_Sig)
    {
        if(spSession->actionSP().sigType() == MDSigType::operator_then_verifier)
        {
            if(spSession->verifierInput().sigID() != "" && spSession->verifierOutput().resultMsg() !="")
                msg.setMsg(spSession->verifierOutput().resultMsg());
        }
    }
    msg.setSpSession(spSession);
    msg.setCurrentUser(currentAccount());
    MDObjectType tp = CMDActionID::actionToObjType(spSession->actionSP().actionID().type());
    CMDObject objtmp("",tp);
    msg.setObject(&objtmp);
    m_contacter->log(&msg);
    return true;
}

bool CMDSecurityPolicyExpertImp::checkSPSession_ESig_Operator(CMDActionSPSession* spSession)
{
    if(!m_configer->para()->isEnableSP())
        return true;

    bool result = helper_CheckSPSession_Operator(spSession);
    if(result)
        lastSPSession_Operator = *spSession;
    log_ESig_Operator(spSession, result);
    return result;
}

bool CMDSecurityPolicyExpertImp::helper_CheckSPSession_Operator(CMDActionSPSession* spSession)
{
    if(!spSession->actionSP().isEnable())   // 安全策略未启用，检查结果为 true
        return true;

    CMDSigInput& input = spSession->operatorInput();
    CMDSigOutput& output = spSession->operatorOutput();
    const QStringList& powerGroupList = spSession->actionSP().operatorGroupList();

    return (MDSecurityResultCode::success == checkSig(input, powerGroupList, output));
}

bool CMDSecurityPolicyExpertImp::checkSPSession_ESig_Verifier(CMDActionSPSession* spSession)
{
    if(!m_configer->para()->isEnableSP())
        return true;

    bool result = helper_CheckSPSession_Verifier(spSession);
    //if(result)
    {
        spSession->setOperatorInput(lastSPSession_Operator.operatorInput());
        spSession->setOperatorOutput(lastSPSession_Operator.operatorOutput());
    }
    log_ESig_Verifier(spSession, result);
    return result;
}

bool CMDSecurityPolicyExpertImp::helper_CheckSPSession_Verifier(CMDActionSPSession* spSession)
{
    if(!spSession->actionSP().isEnable())   // 安全策略未启用，检查结果为 true
        return true;

    CMDSigInput& input = spSession->verifierInput();
    CMDSigOutput& output = spSession->verifierOutput();
    const QStringList& powerGroupList = spSession->actionSP().verifierGroupList();

    return (MDSecurityResultCode::success == checkSig(input, powerGroupList, output));
}

bool CMDSecurityPolicyExpertImp::log_ESig_Operator(CMDActionSPSession* spSession, bool result)
{
    if(!m_contacter)
        return false;

    CMDLogMessage msg;
    if(result)
    {
        QString resultMsg = spSession->actionSP().actionID().getLogMsg();
        resultMsg += " [电子签名(操作签名)成功！]";
        spSession->operatorOutput().setResultMsg(resultMsg);
        return logSPSession(spSession);
        //msg.setMsg("电子签名成功！[操作签名]");
    }
    else
    {
        MDSecurityResultCode result = spSession->operatorOutput().resultCode();
        QString resultMsg = spSession->actionSP().actionID().getLogMsg();
        QString errcode = getAuthErrorMsg(result);
        resultMsg += QString(" [电子签名(操作签名)失败！][%1]").arg(errcode);
        spSession->operatorOutput().setResultMsg(resultMsg);
        return logSPSession(spSession);
        //MDSecurityResultCode result = spSession->operatorOutput().resultCode();
        //msg.setMsg("电子签名失败！[操作签名]- 错误码：" + QVariant((int)result).toString());
    }
    msg.setSpSession(spSession);
    msg.setCurrentUser(currentAccount());
    m_contacter->log(&msg);
    return true;
}

bool CMDSecurityPolicyExpertImp::log_ESig_Verifier(CMDActionSPSession* spSession, bool result)
{
    if(!m_contacter)
        return false;

    CMDLogMessage msg;
    if(result)
    {
        QString resultMsg = spSession->actionSP().actionID().getLogMsg();
        resultMsg += " [电子签名(校验签名)成功！]";
        spSession->verifierOutput().setResultMsg(resultMsg);
        return logSPSession(spSession);
        //msg.setMsg("电子签名成功！[校验签名]");
    }
    else
    {
        MDSecurityResultCode result = spSession->verifierOutput().resultCode();
        QString resultMsg = spSession->actionSP().actionID().getLogMsg();
        QString errcode = getAuthErrorMsg(result);
        resultMsg += QString(" [电子签名(校验签名)失败！][%1]").arg(errcode);
        spSession->verifierOutput().setResultMsg(resultMsg);
        return logSPSession(spSession);
        //MDSecurityResultCode result = spSession->verifierOutput().resultCode();
        //msg.setMsg("电子签名失败！[校验签名]- 错误码：" + QVariant((int)result).toString());
    }
    msg.setSpSession(spSession);
    msg.setCurrentUser(currentAccount());
    m_contacter->log(&msg);
    return true;
}


MDSecurityResultCode CMDSecurityPolicyExpertImp::checkAccount(const QString& account, const QString& password, bool isNoNeedPassword/* = false*/)
{
    MDSecurityResultCode ret = MDSecurityResultCode::success;
    CMDAccountProperty_Config* accountConfig = m_configer->accountConfiger()->find(account);
    CMDAccountStrategy* accountStrategy = m_configer->accountStrategyConfiger()->accountStrategy();
    std::string std_account = account.toStdString();

    if(!accountConfig)
        return MDSecurityResultCode::account_not_exist;

    if(!accountConfig->isEnable())
        return MDSecurityResultCode::account_disabled;

    int32_t lockAccountAtRuntime = m_mmkv->getInt32(std_account+"_lock");
    if(accountConfig->isLocked()){
        // 设计时锁用户总开关
        return MDSecurityResultCode::account_locked;
    }
    else if(lockAccountAtRuntime > 0) {
        // 运行时根据用户策略自动解锁
        if(m_mmkv){
            int64_t locktimestamp = m_mmkv->getInt64(std_account+"_lock_time");
            if(locktimestamp >0) {
                QDateTime lastlockedtime = QDateTime::fromSecsSinceEpoch(locktimestamp);
                qint64 lockedtimesecs = lastlockedtime.secsTo(QDateTime::currentDateTime());
                int autoUnlockPeriod = accountStrategy->autoUnlockPeriod();
                // 0-min 1-hour 2-day 3-month
                int unit = accountStrategy->autoUnlockPeriodUnit();
                if( autoUnlockPeriod>0){
                    quint64 autoUnlockPeriodSecs = 0;
                    if(unit==0)
                        autoUnlockPeriodSecs = autoUnlockPeriod * 60;
                    else if(unit==1)
                        autoUnlockPeriodSecs = autoUnlockPeriod * 60 * 60;
                    else if(unit==2)
                        autoUnlockPeriodSecs = autoUnlockPeriod * 60 * 60 * 24;
                    else if(unit==3)
                        autoUnlockPeriodSecs = autoUnlockPeriod * 60 * 60 * 24 * 30;
                    if(lockedtimesecs >= autoUnlockPeriodSecs){
                        // 锁定时间>=配置自动解锁时间 则自动解运行时用户锁
                        m_mmkv->set((int32_t)0, std_account);
                        m_mmkv->removeValueForKey(std_account);
                        m_mmkv->set((int32_t)0, std_account+"_lock");
                        m_mmkv->removeValueForKey(std_account+"_lock");
                        m_mmkv->set((int64_t)0, std_account+"_lock_time");
                        m_mmkv->removeValueForKey(std_account+"_lock_time");
                        m_mmkv->sync();
                    }
                }

            }
        }
        return MDSecurityResultCode::account_locked;
    }

    if(account.length() < accountStrategy->minAccountID())
        return MDSecurityResultCode::invalid_account;

    if(!isNoNeedPassword) {
        std::string std_runtimeChangedPsw = "";
        if(m_mmkv_by_prj)
            m_mmkv_by_prj->getString(std_account+"_changedpsw", std_runtimeChangedPsw);

        if(!std_runtimeChangedPsw.empty())
            accountConfig->setPassword(QString::fromStdString(std_runtimeChangedPsw));

        if(0 != accountConfig->password().compare(password))
            ret = MDSecurityResultCode::invalid_password;
    }

//    no_result                      = -1,  //! 失败
//    success                        = 0,   //! 成功
//    account_not_exist              = 1,   //! 帐户不存在
//    invalid_account                = 2,   //! 无效的账户
//    invalid_password               = 3,   //! 密码错误
//    account_has_no_power           = 4,   //! 帐户不具有权限
//    account_disabled               = 5,   //! 帐户被禁用
//    password_expired               = 6,   //! 密码已过期,请及时修改密码
//    account_locked                 = 7,   //! 帐户被锁定,需要管理员手动解锁
//    action_type_not_match          = 8,   //! 验证动作类型与预设类型不匹配
//    password_not_match_strategy    = 9    //! 密码不符合安全策略
//    cannot_use_oldpassword         = 10,  //! 不可使用旧密码
//    first_login_modify_password    = 11   //! 第一次登陆必须修改密码

    else if(!isNoNeedPassword && 0 == accountConfig->password().compare(password)) {
        if(password.length() < accountStrategy->minPassword())
            return MDSecurityResultCode::password_not_match_strategy;
        if(accountStrategy->containAlphabet()) {
            QRegularExpression regex("[a-zA-Z]");
            QRegularExpressionMatch match = regex.match(password);
            if (!match.hasMatch()) {
                qDebug() << "The password does not contain letters.";
                ret = MDSecurityResultCode::password_not_match_strategy;
            }
        }
        if(accountStrategy->containNumber()) {
            QRegularExpression regex("[0-9]");
            QRegularExpressionMatch match = regex.match(password);
            if (!match.hasMatch()) {
                qDebug() << "The password does not contain numbers.";
                ret = MDSecurityResultCode::password_not_match_strategy;
            }
        }
        if(accountStrategy->containUppercaseLowercase()) {
            QRegularExpression regex("[a-z].*[A-Z]|[A-Z].*[a-z]");
            QRegularExpressionMatch match = regex.match(password);
            if (!match.hasMatch()) {
                qDebug() << "The password does not contain uppercase and lowercase.";
                ret = MDSecurityResultCode::password_not_match_strategy;
            }
        }
        if(accountStrategy->containExtraAlphabet()) {
            QRegularExpression regex("[\x20-\x2F\x3A-\x40\x5B-\x60\x7B-\x7E]");
            QRegularExpressionMatch match = regex.match(password);
            if (!match.hasMatch()) {
                qDebug() << "The password does not contain extra alphabet.";
                ret = MDSecurityResultCode::password_not_match_strategy;
            }
        }
        if(accountStrategy->pswCannotContainAccout()) {
            if(password.contains(account)) {
                qDebug() << "The password can not contain account.";
                ret = MDSecurityResultCode::password_not_match_strategy;
            }
        }

    }

    int passwordInvalidContinueInputCount = accountStrategy->passwordInvalidContinueInputCount();
    if(m_mmkv && ret != MDSecurityResultCode::success && passwordInvalidContinueInputCount >0) {
        uint32_t invalid_password_continuous_count = m_mmkv->getUInt32(std_account, 0);
        m_mmkv->set(++invalid_password_continuous_count, std_account);
        if(invalid_password_continuous_count >= passwordInvalidContinueInputCount) {
            // 密码连续错误passwordInvalidContinueInputCount次
            m_mmkv->set((int32_t)1, std_account+"_lock");
            int64_t locktimestamp = QDateTime::currentDateTime().toSecsSinceEpoch();
            m_mmkv->set(locktimestamp, std_account+"_lock_time");
        }
    }

    if(!isNoNeedPassword && m_mmkv && ret == MDSecurityResultCode::success && 0 == accountConfig->password().compare(password) && !accountStrategy->cantUseOldPsw()) {
        // 判断是否是使用过的密码
        std::string _usedpsw = "";
        m_mmkv->getString(std_account+"_usedpsw", _usedpsw);
        std::string std_psw = password.toStdString();
        size_t pos = _usedpsw.find(std_psw+"\n");
        if (pos != std::string::npos) {
            // 找到曾经使用过的密码
            std::string lastusedpsw = "";
            if(m_mmkv->getString(std_account+"_curpsw", lastusedpsw))
            {
                if(m_mmkv_by_prj){
                    std::string std_runtimeChangedPsw = "";
                    m_mmkv_by_prj->getString(std_account+"_changedpsw", std_runtimeChangedPsw);
                    // 不是当前正在使用过的登陆成功过的密码，且不是最近修改成功到密码，需要判断是否是使用过的密码
                    if(!lastusedpsw.empty() && lastusedpsw != std_psw && !std_runtimeChangedPsw.empty() && std_runtimeChangedPsw != std_psw)
                        return MDSecurityResultCode::cannot_use_oldpassword;
                }
            }
        }
    }

    // password valid period expired
    int passwordValidPeriod = accountStrategy->passwordValidPeriod();
    if(m_mmkv && ret == MDSecurityResultCode::success && passwordValidPeriod >0) {
        int64_t psw_modify_timestamp = m_mmkv->getInt64(std_account+"_psw_modify_time", 0);
        // 0-min 1-hour 2-day 3-month
        int unit = accountStrategy->passwordValidPeriodUnit();
        if( psw_modify_timestamp>0){
            QDateTime lastpsw_modify_timestamp = QDateTime::fromSecsSinceEpoch(psw_modify_timestamp);
            qint64 psw_modify_timestamp_elapse = lastpsw_modify_timestamp.secsTo(QDateTime::currentDateTime());
            quint64 passwordValidPeriodSecs = 0;
            if(unit==0)
                passwordValidPeriodSecs = passwordValidPeriod * 60;
            else if(unit==1)
                passwordValidPeriodSecs = passwordValidPeriod * 60 * 60;
            else if(unit==2)
                passwordValidPeriodSecs = passwordValidPeriod * 60 * 60 * 24;
            else if(unit==3)
                passwordValidPeriodSecs = passwordValidPeriod * 60 * 60 * 24 * 30;
            if(psw_modify_timestamp_elapse >= passwordValidPeriodSecs) {
                // 密码超过有效期
                return MDSecurityResultCode::password_expired;
            }
        }
    }

    if(ret != MDSecurityResultCode::success)
        return ret;

    if(m_mmkv){
        // 登陆成功清除错误登陆次数
        m_mmkv->set(0, std_account);
        m_mmkv->removeValueForKey(std_account);
        // 记录账户使用过的密码
        std::string _usedpsw = "";
        bool _bhasusedpsw = m_mmkv->getString(std_account+"_usedpsw", _usedpsw);
        bool firstloginMustmodifypsw = false;
        bool first_login = !m_mmkv->containsKey(std_account+"_logined");
        if(first_login && accountStrategy->isEnableModifyPasswordFirstLogin())
            firstloginMustmodifypsw = true;
        if(firstloginMustmodifypsw) {
            if(m_mmkv_by_prj){
                std::string std_runtimeChangedPsw = "";
                m_mmkv_by_prj->getString(std_account+"_changedpsw", std_runtimeChangedPsw);
                if(std_runtimeChangedPsw.empty())
                    return MDSecurityResultCode::first_login_modify_password;
            }
        }

        std::string std_psw = password.toStdString();
        size_t pos = _usedpsw.find(std_psw+"\n");
        if (pos == std::string::npos) {
            // 未存储则append
            _usedpsw.append(std_psw+"\n");
            m_mmkv->set(_usedpsw, std_account+"_usedpsw");
        }

        // 记录已登录标记
        m_mmkv->set(true, std_account+"_logined");
        // 记录当前能成功登录到密码
        m_mmkv->set(std_psw, std_account+"_curpsw");
        if(first_login) {
            // 第一次登录
            bool pswhasmodified = m_mmkv->containsKey(std_account+"_psw_modify_time");
            if(!pswhasmodified) {
                // 且密码未在此设备上成功修改过，将第一次成功登录的时间戳写入_psw_modify_time，表示账户过期时间判断原点
                int64_t psw_modify_time = QDateTime::currentDateTime().toSecsSinceEpoch();
                m_mmkv->set(psw_modify_time, std_account+"_psw_modify_time");
            }
        }
    }

    return MDSecurityResultCode::success;
}

MDSecurityResultCode CMDSecurityPolicyExpertImp::checkPower(const QString& account, const QStringList& powerGroupList)
{
    MDSecurityResultCode result = MDSecurityResultCode::account_has_no_power;
    int count = powerGroupList.count();
    for(int i = 0; i < count; i ++)
    {
        CMDPowerGroup* pGroup = m_configer->powerGroupConfiger()->find(powerGroupList.at(i));
        if(pGroup && (pGroup->accountList().contains(account, Qt::CaseInsensitive) || pGroup->name()=="*"))
        {
            result = MDSecurityResultCode::success;
            break;
        }
    }
    return result;
}

MDSecurityResultCode CMDSecurityPolicyExpertImp::checkSig(const CMDSigInput& input, const QStringList& powerGroupList,
                                                        CMDSigOutput& output)
{
    MDSecurityResultCode accountResult = checkAccount(input.sigID(), input.sigPassword(), input.isNoPasswordNeeded());
    if(accountResult != MDSecurityResultCode::success)
    {
        output.setSuccess(false);
        output.setResultCode(accountResult);
        output.setSigTime(QDateTime::currentDateTime());
        return accountResult;
    }

    MDSecurityResultCode powerResult = checkPower(input.sigID(), powerGroupList);
    output.setSuccess(powerResult == MDSecurityResultCode::success);
    output.setResultCode(powerResult);
    output.setSigTime(QDateTime::currentDateTime());
    return powerResult;
}


int CMDSecurityPolicyExpertImp::accountsCount()
{
    CMDAccountConfiger* c = m_configer->accountConfiger();
    return (c ? c->count() : 0);
}
QStringList CMDSecurityPolicyExpertImp::getAccounts()
{
    CMDAccountConfiger* c = m_configer->accountConfiger();
    return (c ? c->getAccounts() : QStringList());
}
QStringList CMDSecurityPolicyExpertImp::getAccountGroups(QString name)
{
    QStringList retList;
    //CMDAccountConfiger* c = m_configer->accountConfiger();
    QStringList gs = getGroups();
    foreach(QString g, gs){
        bool exist = isAccountInGroup(name, g);
        if(exist){
            retList.append(g);
        }
    }
    return retList;
}
QStringList CMDSecurityPolicyExpertImp::getDisabledAccounts()
{
    CMDAccountConfiger* c = m_configer->accountConfiger();
    return (c ? c->getDisabledAccounts() : QStringList());
}
QStringList CMDSecurityPolicyExpertImp::getLockedAccounts()
{
    CMDAccountConfiger* c = m_configer->accountConfiger();
    QStringList retList = (c ? c->getLockedAccounts() : QStringList());// 总控锁定
    if(m_mmkv){
        std::vector<std::string> keys = m_mmkv->allKeys();
        foreach(std::string s, keys){
            if(s.rfind("_lock") == s.length()-5){
                std::string std_account = s.substr(0, s.length()-5);
                int32_t lock = m_mmkv->getInt32(std_account);
                if(lock >0)
                    retList.append(QString::fromStdString(std_account)); // 运行时安全策略的锁定
            }
        }
    }
    return retList;
}
bool CMDSecurityPolicyExpertImp::enableAccount(QString name)
{
    CMDAccountConfiger* c = m_configer->accountConfiger();
    return (c ? c->enableAccount(name) : 0);
}
bool CMDSecurityPolicyExpertImp::disableAccount(QString name)
{
    CMDAccountConfiger* c = m_configer->accountConfiger();
    return (c ? c->disableAccount(name) : 0);
}
int CMDSecurityPolicyExpertImp::deleteAccount(QString name)
{
    CMDAccountStrategy* accountStrategy = m_configer->accountStrategyConfiger()->accountStrategy();
    if(accountStrategy){
        if(accountStrategy->isEnableDeleteAccount()) {
            // Account deletion not allowed
            return -1;
        }
    }
    CMDAccountConfiger* c = m_configer->accountConfiger();
    bool deleteAccountRet = (c ? c->delete1(name) : 0);
    if(deleteAccountRet){
        QStringList gs = getGroups();
        foreach(QString g, gs){
            bool exist = isAccountInGroup(name, g);
            if(exist){
                removeAccountFromGroup(name, g);
            }
        }
        return 1;
    }
    return 0;
}
bool CMDSecurityPolicyExpertImp::lockAccount(QString name)
{
    CMDAccountConfiger* c = m_configer->accountConfiger();
    return (c ? c->lockAccount(name) : 0);
}
bool CMDSecurityPolicyExpertImp::unlockAccount(QString name)
{
    CMDAccountConfiger* c = m_configer->accountConfiger();
    if(c && m_mmkv){
        std::string std_account = name.toStdString();
        m_mmkv->set((int32_t)0, std_account);
        m_mmkv->removeValueForKey(std_account);
        m_mmkv->set((int32_t)0, std_account+"_lock");
        m_mmkv->removeValueForKey(std_account+"_lock");
        m_mmkv->set((int64_t)0, std_account+"_lock_time");
        m_mmkv->removeValueForKey(std_account+"_lock_time");
        m_mmkv->sync();
    }
    return (c ? c->unlockAccount(name) : 0);
}
bool CMDSecurityPolicyExpertImp::isAccountEnabled(QString name)
{
    CMDAccountConfiger* c = m_configer->accountConfiger();
    return (c ? c->isAccountEnabled(name) : 0);
}
bool CMDSecurityPolicyExpertImp::isAccountLocked(QString name)
{
    CMDAccountConfiger* c = m_configer->accountConfiger();
    return (c ? c->isAccountLocked(name) : 0);
}
bool CMDSecurityPolicyExpertImp::isAccountLogin(QString name)
{
    if(m_currentAccount.sigID() == name)
        return true;
    return false;
}
bool CMDSecurityPolicyExpertImp::upsertAccount(QString name, QString comment, QString password, bool enable, bool lock)
{
    CMDAccountConfiger* c = m_configer->accountConfiger();
    return (c ? c->upsertAccount(name, comment, password, enable, lock) : 0);
}
QString CMDSecurityPolicyExpertImp::getAccount(QString account)
{
    CMDAccountConfiger* c = m_configer->accountConfiger();
    return (c ? c->getAccount(account) : "");
}
int CMDSecurityPolicyExpertImp::modifyPassword(QString account, QString oldPassword, QString newPassword)
{
    CMDAccountProperty_Config* accountConfig = m_configer->accountConfiger()->find(account);
    if(!accountConfig)
        return (int)MDSecurityResultCode::account_not_exist;

    CMDAccountStrategy* s = m_configer->accountStrategyConfiger()->accountStrategy();
    if(newPassword.length() < s->minPassword())
        return (int)MDSecurityResultCode::password_not_match_strategy;
    if(s->containAlphabet()) {
        QRegularExpression regex("[a-zA-Z]");
        QRegularExpressionMatch match = regex.match(newPassword);
        if (!match.hasMatch()) {
            qDebug() << "The password does not contain letters.";
            return (int)MDSecurityResultCode::password_not_match_strategy;
        }
    }
    if(s->containNumber()) {
        QRegularExpression regex("[0-9]");
        QRegularExpressionMatch match = regex.match(newPassword);
        if (!match.hasMatch()) {
            qDebug() << "The password does not contain numbers.";
            return (int)MDSecurityResultCode::password_not_match_strategy;
        }
    }
    if(s->containUppercaseLowercase()) {
        QRegularExpression regex("[a-z].*[A-Z]|[A-Z].*[a-z]");
        QRegularExpressionMatch match = regex.match(newPassword);
        if (!match.hasMatch()) {
            qDebug() << "The password does not contain uppercase and lowercase.";
            return (int)MDSecurityResultCode::password_not_match_strategy;
        }
    }
    if(s->containExtraAlphabet()) {
        QRegularExpression regex("[\x20-\x2F\x3A-\x40\x5B-\x60\x7B-\x7E]");
        QRegularExpressionMatch match = regex.match(newPassword);
        if (!match.hasMatch()) {
            qDebug() << "The password does not contain extra alphabet.";
            return (int)MDSecurityResultCode::password_not_match_strategy;
        }
    }
    if(s->pswCannotContainAccout()) {
        if(newPassword.contains(account)) {
            qDebug() << "The password can not contain account.";
            return (int)MDSecurityResultCode::password_not_match_strategy;
        }
    }

    CMDAccountConfiger* c = m_configer->accountConfiger();
    std::string std_runtimeChangedPsw = "";
    std::string _usedpsw = "";
    std::string std_account = account.toStdString();
    if(m_mmkv_by_prj)
        m_mmkv_by_prj->getString(std_account+"_changedpsw", std_runtimeChangedPsw);

    if(!s->cantUseOldPsw()) {
        // 判断是否是使用过的密码
        if(m_mmkv)
            m_mmkv->getString(std_account+"_usedpsw", _usedpsw);
        std::string std_psw = newPassword.toStdString();
        size_t pos = _usedpsw.find(std_psw+"\n");
        if (pos != std::string::npos) {
            // 找到曾经使用过的密码
            //std::string lastusedpsw = "";
            //m_mmkv->getString(std_account+"_curpsw", lastusedpsw);
            // 不是当前正在使用过的登陆成功过的密码，且不是最近修改成功到密码，需要判断是否是使用过的密码
            //if(!std_runtimeChangedPsw.empty() && std_runtimeChangedPsw != std_psw)
            return (int)MDSecurityResultCode::cannot_use_oldpassword;

        }
        else {
            // 未存储则append
            _usedpsw.append(std_psw+"\n");
        }
    }

    if(accountConfig && !std_runtimeChangedPsw.empty())
        accountConfig->setPassword(QString::fromStdString(std_runtimeChangedPsw));

    int ret = (c ? c->modifyPassword(account, oldPassword, newPassword) : -1);
    if(ret == 0 && m_mmkv){
        std::string std_account = account.toStdString();
        int64_t psw_modify_time = QDateTime::currentDateTime().toSecsSinceEpoch();
        m_mmkv->set(psw_modify_time, std_account+"_psw_modify_time");
        if(!_usedpsw.empty())
            m_mmkv->set(_usedpsw, std_account+"_usedpsw");
        if(m_mmkv_by_prj) {
            std::string std_psw = newPassword.toStdString();
            m_mmkv_by_prj->set(std_psw, std_account+"_changedpsw");
        }
    }
    if(ret == 0)
        return (int)MDSecurityResultCode::success;
    return (int)MDSecurityResultCode::no_result;
}
bool CMDSecurityPolicyExpertImp::resetAccount(QString name)
{
    //CMDAccountConfiger* c = m_configer->accountConfiger();
    std::string std_account = name.toStdString();
    if(m_mmkv){
        m_mmkv->set((int32_t)0, std_account);
        m_mmkv->removeValueForKey(std_account);
        m_mmkv->set((int32_t)0, std_account+"_lock");
        m_mmkv->removeValueForKey(std_account+"_lock");
        m_mmkv->set((int64_t)0, std_account+"_lock_time");
        m_mmkv->removeValueForKey(std_account+"_lock_time");
        m_mmkv->set((int64_t)0, std_account+"_psw_modify_time");
        m_mmkv->removeValueForKey(std_account+"_psw_modify_time");
        m_mmkv->removeValueForKey(std_account+"_usedpsw");
        m_mmkv->removeValueForKey(std_account+"_logined");
        m_mmkv->removeValueForKey(std_account+"_curpsw");

        m_mmkv->sync();
    }
    if(m_mmkv_by_prj){
        m_mmkv->removeValueForKey(std_account+"_changedpsw");
        m_mmkv_by_prj->sync();
    }
    return true;
}
bool CMDSecurityPolicyExpertImp::resetAllAccount()
{
    if(m_mmkv){
        m_mmkv->clearAll();
        m_mmkv->sync();
    }
    if(m_mmkv_by_prj){
        m_mmkv_by_prj->clearAll();
        m_mmkv_by_prj->sync();
    }
    return true;
}


int CMDSecurityPolicyExpertImp::groupsCount()
{
    CMDPowerGroupConfiger* c = m_configer->powerGroupConfiger();
    return (c ? c->count() : 0);
}
QString CMDSecurityPolicyExpertImp::getGroup(QString name)
{
    CMDPowerGroupConfiger* c = m_configer->powerGroupConfiger();
    return (c ? c->getGroup(name) : "");
}
QStringList CMDSecurityPolicyExpertImp::getGroups()
{
    CMDPowerGroupConfiger* c = m_configer->powerGroupConfiger();
    return (c ? c->getGroups() : QStringList());
}
QStringList CMDSecurityPolicyExpertImp::getAccountsInGroup(QString group)
{
    CMDPowerGroupConfiger* c = m_configer->powerGroupConfiger();
    return (c ? c->getAccountsInGroup(group) : QStringList());
}
QStringList CMDSecurityPolicyExpertImp::getDisabledGroups()
{
    CMDPowerGroupConfiger* c = m_configer->powerGroupConfiger();
    return (c ? c->getDisabledGroups() : QStringList());
}
bool CMDSecurityPolicyExpertImp::enableGroup(QString name)
{
    CMDPowerGroupConfiger* c = m_configer->powerGroupConfiger();
    return (c ? c->enableGroup(name) : 0);
}
bool CMDSecurityPolicyExpertImp::disableGroup(QString name)
{
    CMDPowerGroupConfiger* c = m_configer->powerGroupConfiger();
    return (c ? c->disableGroup(name) : 0);
}
bool CMDSecurityPolicyExpertImp::deleteGroup(QString name)
{
    CMDPowerGroupConfiger* c = m_configer->powerGroupConfiger();
    return (c ? c->deleteGroup(name) : 0);
}
// modifyGroupByUI()
bool CMDSecurityPolicyExpertImp::addAccountToGroup(QString account,QString group)
{
    CMDPowerGroupConfiger* c = m_configer->powerGroupConfiger();
    return (c ? c->addAccountToGroup(account, group) : 0);
}
bool CMDSecurityPolicyExpertImp::removeAccountFromGroup(QString account,QString group)
{
    CMDPowerGroupConfiger* c = m_configer->powerGroupConfiger();
    return (c ? c->removeAccountFromGroup(account, group) : 0);
}
bool CMDSecurityPolicyExpertImp::isAccountInGroup(QString account,QString group)
{
    CMDPowerGroupConfiger* c = m_configer->powerGroupConfiger();
    return (c ? c->isAccountInGroup(account, group) : 0);
}
bool CMDSecurityPolicyExpertImp::upsertGroup(QString name, QString comment, bool enable)
{
    CMDPowerGroupConfiger* c = m_configer->powerGroupConfiger();
    return (c ? c->upsertGroup(name, comment, enable) : 0);
}


QString CMDSecurityPolicyExpertImp::getAccountOption()
{
    CMDAccountStrategyConfiger* c = m_configer->accountStrategyConfiger();
    return (c ? c->getAccountOption() : "");
}
bool CMDSecurityPolicyExpertImp::setAccountOption(QString option)
{
    CMDAccountStrategyConfiger* c = m_configer->accountStrategyConfiger();
    return (c ? c->setAccountOption(option) : 0);
}


bool CMDSecurityPolicyExpertImp::commit()
{
    CMDAccountConfiger* ac = m_configer->accountConfiger();
    bool ac_saved = (ac ? ac->saveToFile() : 0);

    CMDPowerGroupConfiger* gc = m_configer->powerGroupConfiger();
    bool gc_saved = (gc ? gc->saveToFile() : 0);

    CMDAccountStrategyConfiger* sc = m_configer->accountStrategyConfiger();
    bool sc_saved = (sc ? sc->saveToFile() : 0);

    return ac_saved && gc_saved && sc_saved;
}

bool CMDSecurityPolicyExpertImp::loadKV()
{
    // kv cache
    QStringList dataPaths = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    if(dataPaths.length() == 0)
        return false;
    QString dataKVPath = dataPaths[0] + QDir::separator() + "KV";
#ifdef __linux__
    dataKVPath = "/home/pi/.local/share/Mindteco/MindSCADASvc/KV";
#endif
    std::string std_id = "__mdauth__";
    std::string cryptkey = "##bm$crypt%user&auth**";
    QFileInfo fileInfo(m_configer->configFileName());
    QString securityDirPath = fileInfo.dir().path();
#ifdef _WIN32
    std::wstring std_rootDir = dataKVPath.toStdWString();
    MMKV::initializeMMKV(std_rootDir);
    std::wstring std_securityDirPath = securityDirPath.toStdWString();
#elif defined(__linux__)
    std::string std_rootDir = dataKVPath.toStdString();
    MMKV::initializeMMKV(std_rootDir, MMKVLogInfo);
    std::string std_securityDirPath = securityDirPath.toStdString();
#endif
    m_mmkv = MMKV::mmkvWithID(std_id, MMKVMode::MMKV_MULTI_PROCESS, &cryptkey, &std_rootDir);
    m_mmkv_by_prj = MMKV::mmkvWithID("__security__", MMKVMode::MMKV_MULTI_PROCESS, &cryptkey, &std_securityDirPath);
    bool kvLoaded = m_mmkv != nullptr && m_mmkv_by_prj != nullptr;
    return kvLoaded;
}

MDSecurityResultCode CMDSecurityPolicyExpertImp::userValidation(const QString &name, const QString &password)
{
    return checkAccount(name, password);
}
