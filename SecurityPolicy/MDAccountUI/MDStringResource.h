#ifndef MDSTRINGRESOURCE_H
#define MDSTRINGRESOURCE_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"

const QString STRING_ACCOUNT_TEXT = QObject::tr("帐户");
const int ACCOUNT_TREEITEM_TYPE = QTreeWidgetItem::ItemType::UserType + 1;

const QString STRING_PROP_COMMON = QObject::tr("常规");
const QString STRING_PROP_PASSWORD = QObject::tr("密码");
const QString STRING_PROP_SP = QObject::tr("安全策略");
const QString STRING_NAME_IS_EMPTY = QObject::tr("名称为空！");
const QString STRING_NAME_IS_TOO_SHORT = QObject::tr("名称太短，小于规定的最小长度！");
const QString STRING_PASSWORD_IS_TOO_SHORT = QObject::tr("密码太短，小于规定的最小长度！");
const QString STRING_CONFIRM_DELETE_ACCOUNT = QObject::tr("确定删除帐户");
const QString STRING_CONFIRM_DELETE_MULTI_ACCOUNT = QObject::tr("确定删除 %1 个帐户？");
const QString STRING_OK = QObject::tr("确定");
const QString STRING_CANCEL = QObject::tr("取消");
const QString STRING_NAME = QObject::tr("名称");
const QString STRING_COMMENT = QObject::tr("描述");
const QString STRING_ENABLE = QObject::tr("启用");
const QString STRING_SELECT_ACCOUNT = QObject::tr("选择帐户");
const QString STRING_ERROR = QObject::tr("错误");

#endif // MDSTRINGRESOURCE_H
