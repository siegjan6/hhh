/*!
\author aGRw 2016.10.08
\section 控件配置（菜单，工具箱）
*/
#ifndef MDDRAWOBJECTCONFIG_H
#define MDDRAWOBJECTCONFIG_H

#include <QtCore/qglobal.h>

#if defined(MDHMIFORM_LIBRARY)
#  define MDHMIFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDHMIFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QTreeWidgetItem>
#include <QMenu>

class CMDHMIForm;

class MDHMIFORMSHARED_EXPORT CMDDrawObjectConfig
{
public:
    //创建工具箱
    static void createToolkit(QTreeWidget *parent);
    //创建菜单
    static QMenu* createMenu(CMDHMIForm* parent);
    //创建图标列表
    static QMap<int, QIcon *> *createDrawIcons();
};

#endif // MDDRAWOBJECTCONFIG_H
