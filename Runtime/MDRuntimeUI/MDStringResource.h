#ifndef MDSTRINGRESOURCE_H
#define MDSTRINGRESOURCE_H

#include <QObject>
#include <QtWidgets/qtreewidget.h>
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"

const int RUNTIMEUI_TREEITEM_TYPE = QTreeWidgetItem::ItemType::UserType + 1;

const QString STRING_NOT_ENABLED = QObject::tr("<未启用>");
const QString STRING_RUN_PARA = QObject::tr("运行参数");
const QString STRING_PROP_PROJECT = QObject::tr("项目");
const QString STRING_PROP_HMI = QObject::tr("图形窗体");
const QString STRING_PROP_ALARM = QObject::tr("报警");
const QString STRING_CONFIG = QObject::tr("配置");
const QString STRING_PROP_OPCUA = QObject::tr("OPCUA");

#endif // MDSTRINGRESOURCE_H
