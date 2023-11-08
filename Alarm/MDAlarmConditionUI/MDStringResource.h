#ifndef MDSTRINGRESOURCE_H
#define MDSTRINGRESOURCE_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"

const QString STRING_COND_TEXT = QObject::tr("报警条件");
const int ALARM_COND_TREEITEM_TYPE = QTreeWidgetItem::ItemType::UserType + 2;

const QString STRING_PROP_COMMON = QObject::tr("常规");
const QString STRING_PROP_COND = QObject::tr("条件");
const QString STRING_PROP_SP = QObject::tr("安全策略");
const QString STRING_NAME_IS_EMPTY = QObject::tr("名称为空！");
const QString STRING_CONFIRM_DELETE_COND = QObject::tr("确定删除报警条件");
const QString STRING_CONFIRM_DELETE_MULTI_COND = QObject::tr("确定删除 %1 个报警条件？");
const QString STRING_OK = QObject::tr("确定");
const QString STRING_CANCEL = QObject::tr("取消");
const QString STRING_NAME = QObject::tr("名称");
const QString STRING_COMMENT = QObject::tr("描述");
const QString STRING_LEVEL = QObject::tr("类别");
const QString STRING_SEVERITY = QObject::tr("等级");
const QString STRING_SELECT_COND = QObject::tr("选择报警条件");
const QString STRING_ERROR = QObject::tr("错误");
const QString STRING_ID = QObject::tr("ID");
const QString STRING_ENABLE = QObject::tr("使能");
const QString STRING_ENABLEEXP = QObject::tr("使能条件");
const QString STRING_ACK = QObject::tr("知悉");
const QString STRING_CONFIRM = QObject::tr("确认");

#endif // MDSTRINGRESOURCE_H
