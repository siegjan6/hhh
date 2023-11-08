#ifndef MAINTREEWIDGET_H
#define MAINTREEWIDGET_H

#include <QTreeWidget>
#include <QMenu>
#include <QAction>
#include <QTabWidget>
//#include "ContainerWidget.h"

enum MainTree_Node_Kind
{
    MainTree_Node_Kind_Data_Source_Root = QTreeWidgetItem::UserType + 1,
    MainTree_Node_Kind_Data_Source = QTreeWidgetItem::UserType + 2,
    MainTree_Node_Kind_Data_Source_Table = QTreeWidgetItem::UserType + 3,
    MainTree_Node_Kind_Data_Group_Root = QTreeWidgetItem::UserType + 4,
    MainTree_Node_Kind_Data_Group_Alarm = QTreeWidgetItem::UserType + 5,
    MainTree_Node_Kind_Data_Group_Log = QTreeWidgetItem::UserType + 6,
    MainTree_Node_Kind_Data_Group_History_Root = QTreeWidgetItem::UserType + 7,
    MainTree_Node_Kind_Data_Group_History_Table = QTreeWidgetItem::UserType + 8,
    MainTree_Node_Kind_Test_Data_Root = QTreeWidgetItem::UserType + 9,
    MainTree_Node_Kind_Test_Data_Child= QTreeWidgetItem::UserType + 10,
    MainTree_Node_Kind_Test_Case_Root = QTreeWidgetItem::UserType + 11,
    MainTree_Node_Kind_Test_Case_Child = QTreeWidgetItem::UserType + 12
};

class MainTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit MainTreeWidget(QWidget *parent = 0);
    void setContainerWidget(QTabWidget * pTabWidget);
    void Reload();
private:
    void Init();
    void InitDataSourceGroupMenu();
    void InitDataSourceMenu();
    void InitHistoryMenu();
    void InitTestDataCaseMenu();
    void InitTestScriptCaseMenu();
    void InitTestDataCaseSubtree();
    void InitTestScriptCaseSubtree();
    //void NewTestNataNode(QString testDataFileName, QTreeWidgetItem * parentNode, bool hasCreateEditor);
signals:
    void setCurrentTabWidget(QWidget *widget);
public slots:
    void showContextMenu(const QPoint &pos);
    void slotitemActiveNewDBSource();
    void slotitemActiveDeleteDBSource();
    void slotitemActiveNewHisTable();
    void slotitemActiveDeleteHisTable();
    //!节点双击信号槽
    void slotitemDoubleClicked(QTreeWidgetItem *item, int column);
    void slotitemActiveNewTestDataNode();
    void slotitemActiveDeleteTestDataNode();
    void slotitemActiveNewTestCaseNode();
    void slotitemActiveDeleteTestCaseNode();
    void slotitemActiveNewDBSourceTable();
    void slotitemActiveDeleteDBSourceTable();
 private:
    QMenu * m_pDBSourceGroupPopupMenu;  //! 数据源组菜单
    QMenu * m_pDBSourcePopupMenu;
    QAction * m_pDBSource_NewAction;
    QAction * m_pDBSource_DeleteAction;
    QAction * m_pDBSourceTable_newAction;
    QMenu * m_pDBSourceTablePopupMenu;
    QAction * m_pDBSourceTalbe_DeleteAction;
    QMenu * m_pHisGroupPopupMenu ;  //! 历史组菜单
    QMenu * m_pHisTablePopupMenu ;  //! 历史组菜单
    QAction * m_pHisGroup_NewAction;
    QAction * m_pHisTable_DeleteAction;
    QMenu * m_pTestDataGroupPopupMenu ;  //! 测试数据菜单
    QMenu * m_pTestDataPopupMenu ;  //! 测试数据菜单
    QAction * m_pTestData_NewAction;
    QAction * m_pTestData_DeleteAction;
    QMenu * m_pTestCaseGroupPopupMenu ;  //! 测试用例菜单
    QMenu * m_pTestCasePopupMenu;
    QAction * m_pTestCase_NewAction;
    QAction * m_pTestCase_DeleteAction;
    QTabWidget * m_pTabWidget;
    //节点
    QTreeWidgetItem * m_pDataSourceRootItem ;//!数据源子树根节点
    QTreeWidgetItem * m_pHistoryRootItem; //!历史组子树根节点
    QTreeWidgetItem * m_TestDataSubstreeRootItem;
    QTreeWidgetItem * m_TestScriptCaseSubstreeRootItem;
};

#endif // MAINTREEWIDGET_H
