#ifndef MDSTRINGRESOURCE_H
#define MDSTRINGRESOURCE_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"

const QString STRING_POWER_GROUP_TEXT = QObject::tr("权限组");
const int POWER_GROUP_TREEITEM_TYPE = QTreeWidgetItem::ItemType::UserType + 2;

const QString STRING_PROP_COMMON = QObject::tr("常规");
const QString STRING_PROP_ACCOUNT = QObject::tr("帐户");
const QString STRING_PROP_SP = QObject::tr("安全策略");
const QString STRING_NAME_IS_EMPTY = QObject::tr("名称为空！");
const QString STRING_CONFIRM_DELETE_POWER_GROUP = QObject::tr("确定删除权限组");
const QString STRING_CONFIRM_DELETE_MULTI_POWER_GROUP = QObject::tr("确定删除 %1 个权限组？");
const QString STRING_OK = QObject::tr("确定");
const QString STRING_CANCEL = QObject::tr("取消");
const QString STRING_NAME = QObject::tr("名称");
const QString STRING_COMMENT = QObject::tr("描述");
const QString STRING_SELECT_POWER_GROUP = QObject::tr("选择权限组");
const QString STRING_ERROR = QObject::tr("错误");

#endif // MDSTRINGRESOURCE_H
