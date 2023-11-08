#ifndef MDSTRINGRESOURCE_H
#define MDSTRINGRESOURCE_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"

const QString STRING_ACTION_TEXT = QObject::tr("动作");
const int ACTION_TREEITEM_TYPE = QTreeWidgetItem::ItemType::UserType + 2;

const QString STRING_PROP_GENERAL = QObject::tr("通用");
const QString STRING_PROP_PARA = QObject::tr("参数");
const QString STRING_PROP_SP = QObject::tr("安全策略");
const QString STRING_NAME_IS_EMPTY = QObject::tr("名称为空！");
const QString STRING_CONFIRM_DELETE_ACTION = QObject::tr("确定删除动作");
const QString STRING_CONFIRM_DELETE_MULTI_ACTION = QObject::tr("确定删除 %1 个动作？");
const QString STRING_OK = QObject::tr("确定");
const QString STRING_CANCEL = QObject::tr("取消");
const QString STRING_NAME = QObject::tr("名称");
const QString STRING_COMMENT = QObject::tr("描述");
const QString STRING_OBJECT = QObject::tr("对象");
const QString STRING_TYPE = QObject::tr("类型");
const QString STRING_SELECT_ACTION = QObject::tr("选择动作");
const QString STRING_ERROR = QObject::tr("错误");

#endif // MDSTRINGRESOURCE_H
