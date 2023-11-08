/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，视图过滤条件树结构对话框，
    用来实现视图过滤条件节点或过滤项的增加，删除和修改等操作】。

*/

#ifndef VIEWFILTERTREEDIALOG_H
#define VIEWFILTERTREEDIALOG_H

#include <QDialog>
#include <MDDBQueryConfigure.h>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMenu>

Q_DECLARE_METATYPE(DBQ::ViewFilterNode*)

namespace Ui {
class ViewFilterTreeDialog;
}

class ViewFilterTreeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewFilterTreeDialog(QWidget *parent = 0);
    ~ViewFilterTreeDialog();

public:
    //! 获取/设置过滤条件指针
    DBQ::ViewFilterNode * copyFilterTree();
    void setFilter(DBQ::ViewFilterNode *filter);
    //! 创建过滤条件树结构
    void initFilterTree();
    //! 设置数据查询模块路径
    void setDBQueryPath(const QString &DBQueryPath);
    //! 设置视图
    void setView(DBQ::View * pView);
    //!如果存在字段视图则生成对应的视图文件 创建了文件 则返回true
    bool generateFieldViewFile();
private:
    //! 添加And/Or节点
    void addAndOrNode(DBQ::VIEW_FILTER_NODE_TYPE nodeType);
    //! 显示添加过滤项添加界面 交互生成过滤项
    DBQ::ViewFilterNode* generateFilterItemNode();
    //!创建qt tree item,并与pNode绑定.之后添加到parentItem中.
    bool createAndInseterQTreeItemToTree(QTreeWidgetItem * parentItem, DBQ::ViewFilterNode *pNode);
    //! 由FilterTree重建QtTree
    void recreateQTree(DBQ::ViewFilterNode *pFilter);
    //! 过滤条件比较关系字符转换函数
    //const char *Filter_GetCompareString(DBQ::VIEW_FILTER_COMPARE_TYPE type);
    //! 添加and/or节点
    void newAndOrFilterNode(DBQ::VIEW_FILTER_NODE_TYPE nodeType,DBQ::ViewFilterNode *currentFilter,QTreeWidgetItem* currentItem);
    //! 过滤项右值为变量时时字符转换
    //QString filterRightValueshowString(QString rightValue);
    bool getParentAndCurrentNode(DBQ::ViewFilterNode *& parentNode, DBQ::ViewFilterNode *& currentNode);
    bool generateFieldView(DBQ::View * pView, DBQ::ViewVariable * pVar);
    //选中currentNode对应的树的节点
    void selectTreeNode(DBQ::ViewFilterNode * currentNode);
private slots:
    //! 右键菜单槽函数
    void slot_showMenu(const QPoint &pos);
    void slot_addAndNodeAction();
    void slot_addOrNodeAction();
    void slot_addFilterNodeAction();
    void slot_modifyNodeAction();
    void slot_deleteNodeAction();

    //! 确认/取消操作
    void on_pbt_Confirm_clicked();
    void on_pbt_Cancel_clicked();
    void on_pbt_Up_clicked();
    void on_pbt_Down_clicked();
    void on_pbt_Left_clicked();
    void on_pbt_Right_clicked();
private:
    Ui::ViewFilterTreeDialog *ui;
    DBQ::ViewFilterNode *m_filter;      //! 视图过滤项指针
    QTreeWidgetItem* m_pRoot;           //! 根节点指针
    QMenu* m_Filter_Menu;               //! 右键菜单
    QAction* m_addAndNode_Action;       //! 添加And节点
    QAction* m_addOrNode_Action;        //! 添加Or节点
    QAction* m_addFilterNode_Action;    //! 添加过滤项节点
    QAction* m_ModifyNode_Action;       //! 修改节点
    QAction* m_DeleteNode_Action;       //! 删除节点
    QString m_DBQueryPath;              //! 数据查询模块项目路径
    DBQ::View * m_pView;                 //! 视图
    bool m_hasRootNode;                 //! 是否存在根节点
    QList<DBQ::View *> m_FieldViewList;
};

#endif // VIEWFILTERTREEDIALOG_H
