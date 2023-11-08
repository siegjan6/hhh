#ifndef MDSTRINGRESOURCE_H
#define MDSTRINGRESOURCE_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"

const QString STRING_LEVEL_TEXT = QObject::tr("报警等级");
const int ALARM_LEVEL_TREEITEM_TYPE = QTreeWidgetItem::ItemType::UserType + 1;

const QString STRING_PROP_COMMON = QObject::tr("常规");
const QString STRING_PROP_COLOR = QObject::tr("颜色");
const QString STRING_SELECT_COLOR = QObject::tr("选择颜色");
const QString STRING_NAME_IS_EMPTY = QObject::tr("名称为空！");
const QString STRING_CONFIRM_DELETE = QObject::tr("确定删除报警等级");
const QString STRING_CONFIRM_DELETE_MULTI = QObject::tr("确定删除 %1 个报警等级？");
const QString STRING_OK = QObject::tr("确定");
const QString STRING_CANCEL = QObject::tr("取消");
const QString STRING_NAME = QObject::tr("名称");
const QString STRING_COMMENT = QObject::tr("描述");
const QString STRING_SELECT_LEVEL = QObject::tr("选择报警等级");
const QString STRING_ERROR = QObject::tr("错误");

#endif // MDSTRINGRESOURCE_H
