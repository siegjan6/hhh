/*!

\author dGFuZ3p3

\section 功能

管理对象树的窗口类的参数类的实现类。

*/

#ifndef MDOBJECTTREEWINDOWPARAIMP_H
#define MDOBJECTTREEWINDOWPARAIMP_H

#include <QModelIndex>
#include <QAction>
#include <QMenu>
#include "MDObjBatchOperationExpert.h"

class QTreeView;
class CMDObjectTreeModel;
class CMDObjectTreeItem;
class ITranslateWidgetTool;
class CMDObjectTreeWindow;
class CMDObjectTreeWindowParaImp : public QObject
{
    Q_OBJECT
public:
    CMDObjectTreeWindowParaImp();
    ~CMDObjectTreeWindowParaImp();
    void release();
public:
    void resetModelData();
    CMDObjectTreeItem* toItem(const QModelIndex& index);
    void selectedItems(QList<CMDObjectTreeItem*>& items);
    void selectedTopItems(QList<CMDObjectTreeItem*>& topItems);
public:
    CMDObjectTreeWindow* m_treeWindow;
    CMDObjectTreeModel *m_objectTreeModel ;// NULL;
    void *m_objectConfiger ;// NULL;
    void* m_objectUI ;// NULL;
public:
    QTreeView *m_treeView ;// NULL;
public:
    QList<void*> m_copyList;
    void clearCopyList(); //! 清除对 CMDObjectTreeItem 的引用
    void prepareCopy(QList<CMDObjectTreeItem*>& copyList);
public:
    bool m_addObjOnSelect;

/*!
\section 批量增加
*/
public:
    void showBatchAddDialog(const QJsonArray& ja);
    CMDObjBatchOperationExpert m_batchAdd;
    QJsonObject propToJson(const QString& propID, const QString& propComment,
                                                         const QString& refValue, const QString& formatValue, int start, int step, int base);
public slots:
    void addOneOnBatch(const QJsonObject &json);

// ----------------------------------------------------------------------------
// 菜单
// ----------------------------------------------------------------------------
public:
    QMenu* m_menu ;// NULL;
    QMenu* m_subMenuForAdd ;// NULL;
    QAction* m_actAdd ;// NULL;
    QAction* m_actConfig ;// NULL;
    QAction* m_actDelete ;// NULL;
    QAction* m_actCopy ;// NULL;
    QAction* m_actPaste ;// NULL;
    QAction* m_actRename ;// NULL;
public:
    QAction* m_actBatchAdd ;// NULL;

// ----------------------------------------------------------------------------
// 多语言
// ----------------------------------------------------------------------------
public:
    void initTranslateTool(CMDObjectTreeWindow* treeWindow);
    void changeLanguage();
    ITranslateWidgetTool* m_translateTool;
};

#endif // MDOBJECTTREEWINDOWPARAIMP_H
