/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块界面框架类，
    用来实现编辑时项目树结构中数据查询模块相关节点】。

*/

#ifndef DBQUERYUI_H
#define DBQUERYUI_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <QMenu>
#include <QAction>
#include <QFileInfo>

#include "MDProjectConfiger.h"
#include "IStudioInterface.h"
#include "ITranslateWidgetTool.h"
#include "MDDBQueryConfigure.h"
#include "NewViewDialog.h"

Q_DECLARE_METATYPE(DBQ::View*)

class QTreeWidget;
class QTreeWidgetItem;

class DBQueryUI: public QObject
{
    Q_OBJECT
public:
    DBQueryUI();
    virtual ~DBQueryUI();

/*!
\section 项目管理，报表树节点相关接口函数
*/
public:
    //! 数据视图根节点
    QTreeWidgetItem *m_rootInStudio ;
    //! 项目管理指针
    IStudioInterface* m_studioInterface;
    //! 加载数据视图管理树节点
    void load();
    //! 卸载数据视图管理树节点
    void unLoad();

private:
    //! 获取树管理器指针
    QTreeWidget *treeWidget();
    //! 获取树根节点指针
    QTreeWidgetItem *treeRoot();
    //! 当前选中的树节点
    QTreeWidgetItem* selectNode();
    //! 是否是数据查询视图节点
    bool isDBQueryUINode(QTreeWidgetItem* node);
    //! 是否是数据查询数据源节点
    bool isDBSourceNode(QTreeWidgetItem* node);
    //! 是否是数据查询视图节点
    bool isViewNode(QTreeWidgetItem* node);
    //! 是否是数据查询视图节点
    bool isViewVarNode(QTreeWidgetItem* node);
    //! 是否是数据查询视图节点
    bool isViewListNode(QTreeWidgetItem* node);
    //! 是否是数据查询视图子节点
    bool isViewListChildNode(QTreeWidgetItem *node);
    //! 判断hItem节点是否为htrDestParent子节点?(包括自己本身)
    bool isItemInChildren(QTreeWidgetItem *hItem,QTreeWidgetItem *htrDestParent);
    //! 清除节点
    void clearNode(QTreeWidgetItem* node);
    //! 构建数据查询视图根节点
    void buildDBQueryUIDirTree(QTreeWidgetItem *pTreeRoot);
    //! 在DBQuery项目子目录中创建全局数据源 global_dbsources.json 文件
    bool creatDBSourceFile();
    //! 在DBQuery项目子目录中创建全局视图变量 global_view_vars.json 文件
    bool creatViewVarFile();
    //! 构建数据查询数据源节点
    void buildDBSourceTree();
    //! 加载数据查询数据源链表
    void LoadDBSourceList(QList<DBQ::DBSource *> dbsourcesList);
    //! 清除数据查询视图中数据源子节点
    void clearDBSourceChildItems();
    //! 构建数据查询视图节点
    void buildViewTree();
    //! 加载数据查询视图链表
    void LoadViewList(QTreeWidgetItem *pRoot);
    //! 从数据查询文件目录创建数据查询视图子节点
    void creatDBQueryViewFilesItem(QStringList list);
    //! 根据视图名称查找视图文件链表是否有同名的视图
    bool isExistSameViewName(QString name);
    //! 根据视图名称查找并返回当前索引
    bool removeCurrentViewName(QString name);

signals:
    void rebuildViewTreeSignal();
/*!
\section 信号与槽函数，槽函数主要是响应右键菜单相关功能
*/
private slots:
    void onMouseRClick();               //! 鼠标右击事件
    void onMouseDbClick();              //! 鼠标双击事件
    void slotitemActiveNewView();       //! 新建视图
    void slotitemActiveRenameView();    //! 重命名视图
    void slotitemActiveDeleteView();    //! 删除视图
    void rebuildViewTreeSlot();         //! 重新构建数据查询视图节点
private:          
    QTreeWidgetItem *m_pRoot;           //! 数据视图管理根节点
    QTreeWidgetItem *m_DBSourceRoot;    //! 数据查询模块数据源节点
    QTreeWidgetItem *m_ViewRoot;        //! 数据查询模块视图节点
    QTreeWidgetItem *m_ViewVar;         //! 数据视图变量节点
    QTreeWidgetItem *m_ViewList;        //! 数据视图文件列表节点
    QMenu *m_pDBView_PopupMenu;         //! 右键菜单
    QAction *m_pDBView_NewAction;       //! 新建视图
    QAction *m_pDBView_Rename;          //! 重命名视图
    QAction *m_pDBView_DeleteAction;    //! 删除视图
    QString sRootName;                  //! 数据查询模块根节点名称
    QString sDBSourceName;              //! 数据源节点名称
    QString sViewName;                  //! 视图节点名称
    QString sViewVarName;               //! 视图变量节点名称
    QString sViewListName;              //! 视图变量文件列表节点名称
    QString m_DBQueryPath;              //! 数据查询模块配置文件路径
    QString m_DBSourceFilePath;         //! 数据查询模块全局数据源文件绝对路径名称
    QString m_ViewVarFilePath;          //! 数据查询模块全局视图变量文件绝对路径名称
    QStringList m_viewNameList;         //! 视图文件名称链表

public:
    void changeLanguage();              //! 多语言
private:
    ITranslateWidgetTool* m_transTool;
};

#endif // DBQUERYUI_H
