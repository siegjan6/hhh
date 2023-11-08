#ifndef MDSTRINGRESOURCE_H
#define MDSTRINGRESOURCE_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"

const QString STRING_EVENT_TEXT = QObject::tr("事件");
const int EVENT_TREEITEM_TYPE = QTreeWidgetItem::ItemType::UserType + 1;

const QString STRING_PROP_GENERAL = QObject::tr("通用");
const QString STRING_PROP_ACTION = QObject::tr("动作");
const QString STRING_PROP_COND = QObject::tr("条件");
const QString STRING_NAME_IS_EMPTY = QObject::tr("名称为空！");
const QString STRING_EVENT_EXPRESSION_IS_EMPTY = QObject::tr("事件表达式为空！");
const QString STRING_CONFIRM_DELETE_EVENT = QObject::tr("确定删除事件");
const QString STRING_CONFIRM_DELETE_MULTI_EVENT = QObject::tr("确定删除 %1 个事件？");
const QString STRING_OK = QObject::tr("确定");
const QString STRING_CANCEL = QObject::tr("取消");
const QString STRING_NAME = QObject::tr("名称");
const QString STRING_COMMENT = QObject::tr("描述");
const QString STRING_OBJECT = QObject::tr("对象");
const QString STRING_TYPE = QObject::tr("类型");
const QString STRING_SELECT_EVENT = QObject::tr("选择事件");
const QString STRING_ERROR = QObject::tr("错误");

#endif // MDSTRINGRESOURCE_H
