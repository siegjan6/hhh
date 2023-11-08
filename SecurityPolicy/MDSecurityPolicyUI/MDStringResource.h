#ifndef MDSTRINGRESOURCE_H
#define MDSTRINGRESOURCE_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"

const QString STRING_AS_TEXT = QObject::tr("帐户策略");
const int AS_TREEITEM_TYPE = QTreeWidgetItem::ItemType::UserType + 3;

const QString STRING_ACCOUNT_AND_POWER_GROUP = QObject::tr("帐户和权限组");
const QString STRING_ACCOUNT = QObject::tr("帐户");
const QString STRING_POWER_GROUP = QObject::tr("权限组");
const QString STRING_ACCOUNT_AND_PASSWORD = QObject::tr("帐户&密码");
const QString STRING_MIN_ACCOUNT_MUST_BE_GREATER_THAN_1 = QObject::tr("最小帐户长度必须大于 1！");
const QString STRING_ESIG = QObject::tr("电子签名");
const QString STRING_NSIG = QObject::tr("MindSCADA 签名");
const QString STRING_LOGIN = QObject::tr("登录");

const QString STRING_ERROR = QObject::tr("错误");
const QString STRING_ERROR_CODE = QObject::tr("错误码");
const QString STRING_FAIL = QObject::tr("失败");
const QString STRING_LOGIN_FAIL = QObject::tr("登录失败");
const QString STRING_ACCOUNT_NOT_EXIST = QObject::tr("帐户不存在！");
const QString STRING_INVALID_ACCOUNT = QObject::tr("无效的账户！");
const QString STRING_WRONG_PASSWORD = QObject::tr("密码错误！");
const QString STRING_ACCOUNT_HAS_NO_POWER = QObject::tr("帐户没有权限！");
const QString STRING_ACCOUNT_DISABLED = QObject::tr("帐户被禁用！");

const QString STRING_ACCOUNT_PASSWORD_EXPIRED = QObject::tr("密码已过期,请及时修改密码");
const QString STRING_ACCOUNT_LOCKED = QObject::tr("帐户被锁定,需要管理员手动解锁");
const QString STRING_ACTION_TYPE_NOT_MATCH = QObject::tr("验证动作类型与预设类型不匹配");
const QString STRING_ACCOUNT_PASSWORD_NOT_MATCH_STRATEGY = QObject::tr("密码不符合安全策略");
const QString STRING_ACCOUNT_CANNOT_USE_OLDPASSWORD = QObject::tr("不可使用旧密码");
const QString STRING_ACCOUNT_FIRST_LOGIN_MODIFY_PASSWORD = QObject::tr("第一次登陆必须修改密码");

const QString STRING_NAME_IS_EMPTY = QObject::tr("名称为空！");
const QString STRING_OK = QObject::tr("确定");
const QString STRING_CANCEL = QObject::tr("取消");
const QString STRING_NAME = QObject::tr("名称");

#endif // MDSTRINGRESOURCE_H
