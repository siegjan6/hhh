/*!

\author dGFuZ3p3

\section 功能

定义安全策略常量。

*/

#ifndef MDSECURITYPOLICYCONSTANT_H
#define MDSECURITYPOLICYCONSTANT_H

//! 动作保护模式
enum class MDActionProtectMode
{
    null_mode                   = 0,
    MindSCADA_Sig               = 1,    //! MindSCADA签名保护
    Electronic_Sig              = 2     //! 电子签名保护
};

//! 签名类型
enum class MDSigType
{
    null_type                       = 0,
    operator_only                   = 1,  //! 仅需要操作者签名
    operator_then_verifier          = 2   //! 需要操作者签名和校验者签名
};

//! 安全验证结果
enum class MDSecurityResultCode
{
    no_result                           = -1,  //! 失败
    success                             = 0,   //! 成功
    account_not_exist                   = 1,   //! 帐户不存在
    invalid_account                     = 2,   //! 无效的账户
    invalid_password                    = 3,   //! 密码错误
    account_has_no_power                = 4,   //! 帐户不具有权限
    account_disabled                    = 5,   //! 帐户被禁用
    password_expired                    = 6,   //! 密码已过期,请及时修改密码
    account_locked                      = 7,   //! 帐户被锁定,需要管理员手动解锁
    action_type_not_match               = 8,   //! 验证动作类型与预设类型不匹配
    password_not_match_strategy         = 9,   //! 密码不符合安全策略
    cannot_use_oldpassword              = 10,  //! 不可使用旧密码
    first_login_modify_password         = 11,  //! 第一次登陆必须修改密码
    account_has_not_assign_to_power     = 12,  //! 帐户未分配至任何权限组
};

// TODO: 登陆返回码处理
//switch(log_ret) {
//    case -1:
//        errMsg = "失败";
//        break;
//    case 0:
//        errMsg = "成功";
//        break;
//    case 1:
//        errMsg = "帐户不存在";
//        break;
//    case 2:
//        errMsg = "无效的账户";
//        break;
//    case 3:
//        errMsg = "密码错误";
//        break;
//    case 4:
//        errMsg = "帐户不具有权限";
//        break;
//    case 5:
//        errMsg = "帐户被禁用";
//        break;
//    case 6:
//        errMsg = "密码已过期,请及时修改密码";
//        break;
//    case 7:
//        errMsg = "帐户被锁定,需要管理员手动解锁";
//        break;
//}

#endif // MDSECURITYPOLICYCONSTANT_H
