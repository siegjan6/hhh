#include "MainTreeWidget.h"
#include <QMenu>
#include <QPlainTextEdit>
#include <QDebug>
#include <QMessageBox>

#include <assert.h>
#include "TestorGlobalInst.h"
#include <MDDataSource/MDDataSourceFactory.h>
#include <MDDataGroup/MDDataGroupFactory.h>
#include <NewDBsourceDialog.h>
#include <IMDDataSourceManager.h>
#include <IMDDataSource.h>
#include <IMDDataTable.h>
#include <dbdatagrouptreecreator.h>
#include "MDNewDataGroup.h"
#include <MDDataDefine.h>
#include <IMDDataItem.h>
#include <IMDDataGroup.h>
#include <IMDDataGroupManager.h>
#include "testdialog.h"
#include "MDDataGroupTableWidget.h"
#include "TestDataEditor.h"
#include "newfiledialog.h"
#include "testdatagroupmgr.h"
#include "scriptcasegroupmgr.h"
#include "ScriptEditor.h"
#include "DBSourceParamWidget.h"
#include "MDDataSrcTabWidget.h"
#include "DBTableDialog.h"

Q_DECLARE_METATYPE(IMDDataSource *)
Q_DECLARE_METATYPE(IMDDataTable *)
Q_DECLARE_METATYPE(IMDDataGroup *)
Q_DECLARE_METATYPE(QWidget *)

class DBSourceTreeOperator
{
 public:
    /*!
    \section 加载数据 创建数据源树形控件
    */
    void InitTree(QTreeWidgetItem *pRoot, IMDDataArchives * pDataArchives);
};

class CaseNodeCreator
{
public:
    CaseNodeCreator(CaseGroupMgr * pMgr, int nodeType);
    QTreeWidgetItem *  Create(QString testDataFileName, QTreeWidgetItem * parentNode);
    virtual QWidget * CreateWithEditor(QString testDataFileName, QTreeWidgetItem * parentNode,  QTabWidget * pTabWidget) = 0;
 private:
    CaseGroupMgr * m_pMgr;
    int m_NodeType;

};

CaseNodeCreator::CaseNodeCreator(CaseGroupMgr * pMgr, int nodeType)
    :m_pMgr(pMgr),
      m_NodeType(nodeType)
{
}

QTreeWidgetItem * CaseNodeCreator::Create(QString testDataFileName, QTreeWidgetItem * parentNode)
{
    //创建测试文件
    QString emptyStr;
    bool b = m_pMgr->SaveCase(testDataFileName, emptyStr); //测试用例名
    if(!b)
    {
        QMessageBox::information(0,  "失败",  "创建测试用例失败");
        return 0;
    }

    //qDebug()<<fileName;

    //创建树状节点

    QTreeWidgetItem *pItem = new QTreeWidgetItem(QStringList(testDataFileName), m_NodeType);
    pItem->setIcon(0,QIcon(":/img/images/file.png"));
    parentNode->addChild(pItem);

    QVariant var;
    var.setValue (testDataFileName);
    pItem->setData (0, Qt::UserRole, var);

    return pItem;
}

class DataCaseNodeCreator : public CaseNodeCreator
{
public:
    DataCaseNodeCreator();
    virtual QWidget * CreateWithEditor(QString testDataFileName, QTreeWidgetItem * parentNode,  QTabWidget * pTabWidget);
};

DataCaseNodeCreator::DataCaseNodeCreator()
    :CaseNodeCreator(TestDataGroupMgr::GetInst(),  MainTree_Node_Kind_Test_Data_Child)
{
}

QWidget * DataCaseNodeCreator::CreateWithEditor(QString testDataFileName, QTreeWidgetItem * parentNode,  QTabWidget * pTabWidget)
{
    QTreeWidgetItem * pItem = CaseNodeCreator::Create(testDataFileName, parentNode);
    //创建测试用例编辑控件

    TestDataEditor * pEditor = new TestDataEditor();
    pEditor->SetTestDataFileName(testDataFileName);
    pTabWidget->addTab(pEditor, QString("TestData:%1").arg(testDataFileName));

    QVariant var;
    var.setValue(pEditor);
    pItem->setData(1, Qt::UserRole, var);

    return pEditor;
}

class ScriptCaseNodeCreator : public CaseNodeCreator
{
public:
    ScriptCaseNodeCreator();
    virtual QWidget * CreateWithEditor(QString testDataFileName, QTreeWidgetItem * parentNode,  QTabWidget * pTabWidget);
};

ScriptCaseNodeCreator::ScriptCaseNodeCreator()
    :CaseNodeCreator(ScriptCaseGroupMgr::GetInst(), MainTree_Node_Kind_Test_Case_Child)
{
}

QWidget * ScriptCaseNodeCreator::CreateWithEditor(QString caseName, QTreeWidgetItem * parentNode,  QTabWidget * pTabWidget)
{
    QTreeWidgetItem * pItem = CaseNodeCreator::Create(caseName, parentNode);
    //创建测试用例编辑控件

    ScriptEditor * pEditor = new ScriptEditor();
    pEditor->SetCaseName(caseName);
    pTabWidget->addTab(pEditor, QString("TestScript:%1").arg(caseName));

    QVariant var;
    var.setValue(pEditor);
    pItem->setData(1, Qt::UserRole, var);

    return pEditor;
}


MainTreeWidget::MainTreeWidget(QWidget *parent) :
    QTreeWidget(parent),
    m_pTabWidget(0),
    m_pHistoryRootItem(0)
{
   setContextMenuPolicy(Qt::CustomContextMenu);
   setHeaderLabel("Project");
   connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),  SLOT(showContextMenu(const QPoint&)));
   connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), SLOT(slotitemDoubleClicked(QTreeWidgetItem*,int)));

    Reload();
}

void MainTreeWidget::setContainerWidget(QTabWidget * pTabWidget)
{
    m_pTabWidget = pTabWidget;
    //TODO:如果多次connect会怎么样
    connect(this, SIGNAL(setCurrentTabWidget(QWidget*)), m_pTabWidget, SLOT(setCurrentWidget(QWidget*)));
}

void MainTreeWidget::Reload()
{
    Init();
    InitDataSourceGroupMenu();
    InitDataSourceMenu();
    InitHistoryMenu();
    InitTestDataCaseMenu();
    InitTestScriptCaseMenu();
    DBSourceTreeOperator dbSrcTreeOperator;
    dbSrcTreeOperator.InitTree(m_pDataSourceRootItem, TestorGlobalInst::GetDataArchives());
    InitTestDataCaseSubtree();
    InitTestScriptCaseSubtree();
}

void MainTreeWidget::Init()
{
    QStringList itemNameStrList;
    itemNameStrList<<"数据源";
    m_pDataSourceRootItem = new QTreeWidgetItem(this, itemNameStrList,  MainTree_Node_Kind_Data_Source_Root);
    m_pDataSourceRootItem->setIcon(0,QIcon(":/img/images/folder.png"));

    DBDataGroupTreeCreator dbDataGTCreator(TestorGlobalInst::GetDataArchives(), this);
    dbDataGTCreator.build();
    m_pHistoryRootItem = dbDataGTCreator.getHistoryRootItem();

    itemNameStrList.clear();
    itemNameStrList<<"测试数据";
    m_TestDataSubstreeRootItem = new QTreeWidgetItem(this, itemNameStrList,  MainTree_Node_Kind_Test_Data_Root);
    m_TestDataSubstreeRootItem->setIcon(0,QIcon(":/img/images/folder.png"));

    itemNameStrList.clear();
    itemNameStrList<<"测试用例";
    m_TestScriptCaseSubstreeRootItem = new QTreeWidgetItem(this, itemNameStrList,  MainTree_Node_Kind_Test_Case_Root);
    m_TestScriptCaseSubstreeRootItem->setIcon(0,QIcon(":/img/images/folder.png"));
}

void MainTreeWidget::InitDataSourceGroupMenu()
{
    m_pDBSourceGroupPopupMenu = new QMenu(this);

    m_pDBSource_NewAction = new QAction(tr("新建数据源"), m_pDBSourceGroupPopupMenu);
    connect(m_pDBSource_NewAction,SIGNAL(triggered()), this, SLOT(slotitemActiveNewDBSource()));

    // 加数据源菜单
    m_pDBSourceGroupPopupMenu->addAction(m_pDBSource_NewAction);
}

void MainTreeWidget::InitDataSourceMenu()
{
    m_pDBSourcePopupMenu = new QMenu(this);

    m_pDBSource_DeleteAction = new QAction(tr("删除数据源"), m_pDBSourcePopupMenu);
    connect(m_pDBSource_DeleteAction, SIGNAL(triggered()), this, SLOT(slotitemActiveDeleteDBSource()));
    m_pDBSourcePopupMenu->addAction(m_pDBSource_DeleteAction);

    m_pDBSourceTable_newAction = new QAction(tr("新建表"), m_pDBSourcePopupMenu);
    connect(m_pDBSourceTable_newAction, SIGNAL(triggered()), this, SLOT(slotitemActiveNewDBSourceTable()));
    m_pDBSourcePopupMenu->addAction(m_pDBSourceTable_newAction);

    m_pDBSourceTablePopupMenu = new QMenu(this);

    m_pDBSourceTalbe_DeleteAction = new QAction(tr("删除表"), m_pDBSourceTablePopupMenu);
    connect(m_pDBSourceTalbe_DeleteAction, SIGNAL(triggered()), this, SLOT(slotitemActiveDeleteDBSourceTable()));
    m_pDBSourceTablePopupMenu->addAction(m_pDBSourceTalbe_DeleteAction);
}

void MainTreeWidget::InitHistoryMenu()
{
    m_pHisGroupPopupMenu = new QMenu(this);
    m_pHisTablePopupMenu = new QMenu(this);

    m_pHisGroup_NewAction = new QAction(tr("新建"), m_pHisGroupPopupMenu);
    connect(m_pHisGroup_NewAction,SIGNAL(triggered()),this,SLOT(slotitemActiveNewHisTable()));

    m_pHisTable_DeleteAction = new QAction(tr("删除"),  m_pHisTablePopupMenu);
    connect(m_pHisTable_DeleteAction,SIGNAL(triggered()),this,SLOT(slotitemActiveDeleteHisTable()));

    m_pHisGroupPopupMenu->addAction(m_pHisGroup_NewAction);
    m_pHisTablePopupMenu->addAction(m_pHisTable_DeleteAction);
}

void MainTreeWidget::InitTestDataCaseMenu()
{
    m_pTestDataGroupPopupMenu = new QMenu(this);
    m_pTestDataPopupMenu = new QMenu(this);

    m_pTestData_NewAction = new QAction(tr("新建"), m_pTestDataGroupPopupMenu);
    connect(m_pTestData_NewAction, SIGNAL(triggered()),this,SLOT(slotitemActiveNewTestDataNode()));

    m_pTestData_DeleteAction = new QAction(tr("删除"),  m_pTestDataPopupMenu);
    connect(m_pTestData_DeleteAction,SIGNAL(triggered()),this,SLOT(slotitemActiveDeleteTestDataNode()));

    m_pTestDataGroupPopupMenu->addAction(m_pTestData_NewAction);
    m_pTestDataPopupMenu->addAction(m_pTestData_DeleteAction);
}

void MainTreeWidget::InitTestScriptCaseMenu()
{
    m_pTestCaseGroupPopupMenu = new QMenu(this);
    m_pTestCasePopupMenu = new QMenu(this);

    m_pTestCase_NewAction = new QAction(tr("新建"), m_pTestCaseGroupPopupMenu);
    connect(m_pTestCase_NewAction, SIGNAL(triggered()),this,SLOT(slotitemActiveNewTestCaseNode()));

    m_pTestCase_DeleteAction = new QAction(tr("删除"),  m_pTestCasePopupMenu);
    connect(m_pTestCase_DeleteAction ,SIGNAL(triggered()),this,SLOT(slotitemActiveDeleteTestCaseNode()));

    m_pTestCaseGroupPopupMenu->addAction(m_pTestCase_NewAction);
    m_pTestCasePopupMenu->addAction(m_pTestCase_DeleteAction);
}

void MainTreeWidget::InitTestDataCaseSubtree()
{
    QStringList caseNameList;
    TestDataGroupMgr::GetInst()->GetAllCaseName(caseNameList); //测试用例名

    int size = caseNameList.size();
    for(int i=0; i<size; i++)
    {
        DataCaseNodeCreator creator;
        creator.Create(caseNameList[i], m_TestDataSubstreeRootItem);
    }
}

void MainTreeWidget::InitTestScriptCaseSubtree()
{
    QStringList caseNameList;
    ScriptCaseGroupMgr::GetInst()->GetAllCaseName(caseNameList); //测试用例名

    int size = caseNameList.size();
    for(int i=0; i<size; i++)
    {
        ScriptCaseNodeCreator creator;
        creator.Create(caseNameList[i], m_TestScriptCaseSubstreeRootItem);
    }
}

//参考http://stackoverflow.com/questions/12937812/how-to-create-different-popup-context-menus-for-each-type-of-qtreewidgetitem
void MainTreeWidget::showContextMenu(const QPoint &pos)
{
    QTreeWidgetItem* item = itemAt(pos);
    if(!item)
    {
        return ;
    }

    switch (item->type()) {
        case MainTree_Node_Kind_Data_Source_Root:
            m_pDBSourceGroupPopupMenu->exec(mapToGlobal(pos));
        break;
        case MainTree_Node_Kind_Data_Source:
            m_pDBSourcePopupMenu->exec(mapToGlobal(pos));
        break;
        case MainTree_Node_Kind_Data_Source_Table:
            m_pDBSourceTablePopupMenu->exec(mapToGlobal(pos));
        break;
        case MainTree_Node_Kind_Data_Group_Root:
        break;
        case MainTree_Node_Kind_Data_Group_History_Root:
            m_pHisGroupPopupMenu->exec(mapToGlobal(pos));
        break;
        case MainTree_Node_Kind_Data_Group_History_Table:
            m_pHisTablePopupMenu->exec(mapToGlobal(pos));
        break;
        case MainTree_Node_Kind_Test_Data_Root:
                m_pTestDataGroupPopupMenu->exec(mapToGlobal(pos));
        break;
        case MainTree_Node_Kind_Test_Data_Child:
                m_pTestDataPopupMenu->exec(mapToGlobal(pos));
        break;
         case MainTree_Node_Kind_Test_Case_Root:
            m_pTestCaseGroupPopupMenu->exec(mapToGlobal(pos));
        break;
        case MainTree_Node_Kind_Test_Case_Child:
            m_pTestCasePopupMenu->exec(mapToGlobal(pos));
         break;
          default:
        {
            //TODO:LOG
        }
    }
}

//!创建数据源
void MainTreeWidget::slotitemActiveNewDBSource()
{
    QTreeWidgetItem *pDataSrcRootItem = currentItem();
    assert(pDataSrcRootItem != NULL);

    IMDDataArchives * pDataArchives = TestorGlobalInst::GetDataArchives();
    IMDDataSourceManager *pDataSrcManager = pDataArchives->dataSourceManager();
    if(NULL == pDataSrcManager)
    {
        return;
    }

    IMDDataSource *pDataSrc = CMDDataSourceFactory::createDataSource();
    if(NULL == pDataSrc)
    {
        return;
    }

    CNewDBsourceDialog dlg;
    dlg.initUI(pDataSrc, pDataArchives);
    if(dlg.exec() == QDialog::Accepted)
    {
        if(pDataSrcManager->addDataSource(pDataSrc))
        {
            QTreeWidgetItem * pItem = new QTreeWidgetItem(QStringList(pDataSrc->name()), MainTree_Node_Kind_Data_Source);
            pItem->setIcon(0,QIcon(":/img/images/folder.png"));
            pDataSrcRootItem->addChild(pItem); //创建数据源节点

            QVariant Var;
            Var.setValue (pDataSrc);
            pItem->setData (0, Qt::UserRole, Var);

            pDataSrcManager->save();

            if (pDataSrc->isShowTable())
            {
                IMDDataTable *pTable = NULL;
                int count = pDataSrc->dataTableCount();
                for(int j = 0; j < count; j++) //为数据源中的各个表创建节点
                {
                    pTable = pDataSrc->getDataTable(j);
                    if(NULL == pDataSrc)
                    {
                        continue;
                    }

                    QTreeWidgetItem *pItemTable = new QTreeWidgetItem(QStringList(pTable->name()), MainTree_Node_Kind_Data_Source_Table);
                    pItemTable->setIcon(0,QIcon(":/img/images/file.png"));
                    pItem->addChild(pItemTable);

                    QVariant Var;
                    Var.setValue (pTable);
                    pItemTable->setData (0, Qt::UserRole, Var);
                }
            }

            setCurrentItem(pItem);
        }
        else
        {
            delete pDataSrc;
            pDataSrc = NULL;
        }
    }
    else
    {
        delete pDataSrc;
        pDataSrc = NULL;
    }
}

void MainTreeWidget::slotitemActiveDeleteDBSource()
{
    IMDDataSourceManager *pDataSrcManager = TestorGlobalInst::GetDataArchives()->dataSourceManager();
    if(NULL == pDataSrcManager)
    {
        return;
    }

    QTreeWidgetItem *pDataSrcItem = currentItem();
    QString strName = pDataSrcItem->text(0);
    IMDDataSource *pDataSrc = pDataSrcManager->getDataSource(strName);
    if(NULL == pDataSrc)
    {
        return;
    }

    QMessageBox message;
    message.setWindowTitle(tr("提示"));
    message.addButton(tr("确定"), QMessageBox::YesRole);
    message.addButton(tr("取消"), QMessageBox::NoRole);
    QString strFront = tr("确定删除 ");
    QString strBack = tr(" 吗？");
    message.setText(strFront + "\'" + strName + "\'" + strBack);

    if (QMessageBox::AcceptRole == message.exec())
    {
        pDataSrcManager->removeDataSource(pDataSrc);
        pDataSrcManager->save();

        m_pDataSourceRootItem->removeChild(pDataSrcItem);
        delete pDataSrcItem;
    }
}

/*!
  \section 点击新建历史组菜单项 触发此槽函数
 */
void MainTreeWidget::slotitemActiveNewHisTable()
{
    QTreeWidgetItem * pHisGroupRootItem = currentItem();
    assert(pHisGroupRootItem != NULL);

    IMDDataArchives * pDataArchives = TestorGlobalInst::GetDataArchives();

    IMDDataGroupManager *pDataGrpMana = pDataArchives->dataGroupManager();
    if (NULL == pDataGrpMana)
    {
        return;
    }

    IMDDataGroup *pHisDataGrp = CMDDataGroupFactory::createDataGroup();
    CMDNewDataGroup dataGrpDlg;//!新建数据组对话框
    dataGrpDlg.initData(pHisDataGrp, pDataGrpMana);
    if (QDialog::Accepted != dataGrpDlg.exec())
    {
        delete pHisDataGrp;
        pHisDataGrp = NULL;
        return;
    }

    //把默认记录时间字段增加到第一行的位置
    IMDDataItem *item = CMDDataGroupFactory::createDataItem();
    item->setDataType(MDDataType::DateTime);
    item->setName(LOG_RECORDTIME);
    item->setDdescription("记录时间");
    pHisDataGrp->addItem(item);

    item = CMDDataGroupFactory::createDataItem();
    item->setDataType(MDDataType::Int32);
    item->setName(LOG_RECORDTIME_MS);
    item->setDdescription("记录时间毫秒");
    pHisDataGrp->addItem(item);

    if(pHisGroupRootItem) //TODO:if(!m_HisGroupRoot) return;
    {
        QTreeWidgetItem *pItem = new QTreeWidgetItem(QStringList(pHisDataGrp->name()), MainTree_Node_Kind_Data_Group_History_Table);
        pItem->setIcon(0,QIcon(":/img/images/file.png"));
        pHisGroupRootItem->addChild(pItem);

        QVariant Var;
        Var.setValue (pHisDataGrp);
        pItem->setData (0, Qt::UserRole, Var);

        if (!pDataGrpMana->addHisDataGroup(pHisDataGrp))
        {
            delete pHisDataGrp;
            pHisDataGrp = NULL;
            //!TODO:Log
            return;
        }

        pDataGrpMana->save();

        setCurrentItem(pItem);
    }
}

/*!
  \section 点击删除历史组菜单项 触发此槽函数
 */
void MainTreeWidget::slotitemActiveDeleteHisTable()
{
    IMDDataArchives * pDataArchives = TestorGlobalInst::GetDataArchives();

    IMDDataGroupManager *pDataGrpMana = pDataArchives->dataGroupManager();
    if (NULL == pDataGrpMana)
    {
        return;
    }

    QTreeWidgetItem *pItem = currentItem();
    IMDDataGroup *pHistoryGroup = pItem->data(0,Qt::UserRole).value< IMDDataGroup* >();
    if(NULL == pHistoryGroup)
    {
        return;
    }

    QMessageBox message;
    message.setWindowTitle(tr("提示"));
    message.addButton(tr("确定"), QMessageBox::YesRole);
    message.addButton(tr("取消"), QMessageBox::NoRole);
    QString strFront = tr("确定删除 ");
    QString strName = "\'" + pHistoryGroup->name() + "\'";
    QString strBack = tr("吗？");
    message.setText(strFront + strName + strBack);

    if (QMessageBox::AcceptRole == message.exec())
    {
        pDataGrpMana->removeHisDataGroup(pHistoryGroup);
        pDataGrpMana->save();


        QWidget * pWidget = pItem->data(1,Qt::UserRole).value<QWidget* >();
        m_pTabWidget->removeTab(m_pTabWidget->indexOf(pWidget));//TODO:会不会自动释放pWidget
        m_pHistoryRootItem->removeChild(pItem);//TODO:会不会同时delete
        //delete pDataSrcItem;//?
    }

}

//!节点双击信号槽
void MainTreeWidget::slotitemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QTreeWidgetItem * pCurrentItem = currentItem();
    assert(pCurrentItem != NULL);

    switch(pCurrentItem->type())
    {
    case MainTree_Node_Kind_Data_Group_History_Table:
    case MainTree_Node_Kind_Data_Group_Alarm:
    case MainTree_Node_Kind_Data_Group_Log:
        {
            IMDDataGroup *pHistoryGroup = pCurrentItem->data(0,Qt::UserRole).value< IMDDataGroup* >();
            if(NULL == pHistoryGroup)
            {
                return;
            }
            QWidget * pTabWidget = pCurrentItem->data(1,Qt::UserRole).value<QWidget* >();

            if(!pTabWidget)
            {
                CMDDataGroupTableWidget * pDataGrpDlg = new CMDDataGroupTableWidget(this);
                pDataGrpDlg->setDataGrpProperty(TestorGlobalInst::GetDataArchives(), pHistoryGroup);
                m_pTabWidget->addTab(pDataGrpDlg, pHistoryGroup->name());
                pTabWidget = pDataGrpDlg;

                QVariant var;
                var.setValue(pTabWidget);
                pCurrentItem->setData(1, Qt::UserRole, var);
            }

            emit setCurrentTabWidget(pTabWidget);
        }
        break;
     case MainTree_Node_Kind_Test_Data_Child:
        {
            QString testDatafileName = pCurrentItem->data(0,Qt::UserRole).toString();

            QWidget * pTabWidget = pCurrentItem->data(1,Qt::UserRole).value<QWidget* >();
            if(!pTabWidget)
            {
                TestDataEditor * pEditor = new TestDataEditor();
                pEditor->SetTestDataFileName(testDatafileName);
                m_pTabWidget->addTab(pEditor, QString("TestData:%1").arg(testDatafileName));
                pTabWidget = pEditor;

                QVariant var;
                var.setValue(pTabWidget);
                pCurrentItem->setData(1, Qt::UserRole, var);
            }

            emit setCurrentTabWidget(pTabWidget);
        }
        break;
     case MainTree_Node_Kind_Test_Case_Child:
        {
            QString caseName = pCurrentItem->data(0,Qt::UserRole).toString();

            QWidget * pTabWidget = pCurrentItem->data(1,Qt::UserRole).value<QWidget* >();
            if(!pTabWidget)
            {
                ScriptEditor * pEditor = new ScriptEditor();
                pEditor->SetCaseName(caseName);
                m_pTabWidget->addTab(pEditor, QString("TestScript:%1").arg(caseName));
                pTabWidget = pEditor;

                QVariant var;
                var.setValue(pTabWidget);
                pCurrentItem->setData(1, Qt::UserRole, var);
            }

            emit setCurrentTabWidget(pTabWidget);
        }
        break;
     case MainTree_Node_Kind_Data_Source: //显示数据源配置界面
        {

            IMDDataSource *pDBSource = pCurrentItem->data(0,Qt::UserRole).value< IMDDataSource* >();
            if(!pDBSource)
                return ;//TODO:log

            QWidget * pTabWidget = pCurrentItem->data(1,Qt::UserRole).value<QWidget* >();
            if(!pTabWidget)
            {
                CDBSourceParamWidget * pDataSourceParamWidget = new CDBSourceParamWidget();
                pDataSourceParamWidget->setDataSourceManager(TestorGlobalInst::GetDataArchives()->dataSourceManager());
                pDataSourceParamWidget->initUI(pDBSource);
                m_pTabWidget->addTab(pDataSourceParamWidget, QString("DataSrc:%1").arg(pDBSource->name()));
                pTabWidget = pDataSourceParamWidget;

                QVariant var;
                var.setValue(pTabWidget);
                pCurrentItem->setData(1, Qt::UserRole, var);
            }

            emit setCurrentTabWidget(pTabWidget);
        }
        break;
    case MainTree_Node_Kind_Data_Source_Table:
        {
            IMDDataTable * pTable = pCurrentItem->data(0,Qt::UserRole).value< IMDDataTable* >();
            if(!pTable)
                return ; //TODO:log

            IMDDataSource *pDBSource = pCurrentItem->parent()->data(0,Qt::UserRole).value< IMDDataSource* >();
            if(!pDBSource)
                return ;//TODO:log

            QWidget * pTabWidget = pCurrentItem->data(1,Qt::UserRole).value<QWidget* >();
            if(!pTabWidget)
            {
                CMDDataSrcTabWidget * pDataSourceTableWidget = new CMDDataSrcTabWidget();
                pDataSourceTableWidget->setDataArchives(TestorGlobalInst::GetDataArchives());
                pDataSourceTableWidget->initData(pTable, pTable->dataGroupName(),  pDBSource->name());
                m_pTabWidget->addTab(pDataSourceTableWidget, QString("DSTable:%1").arg(pTable->dataGroupName()));
                pTabWidget = pDataSourceTableWidget;

                QVariant var;
                var.setValue(pTabWidget);
                pCurrentItem->setData(1, Qt::UserRole, var);
            }

            emit setCurrentTabWidget(pTabWidget);
        }
        break;
    default://TODO:补全
        {
        }
    }//end switch

}

//!新建测试数据节点
void MainTreeWidget::slotitemActiveNewTestDataNode()
{
    NewFileDialog dlg;
    if(dlg.exec() !=  QDialog::Accepted)
        return ;

    //测试数据文件名
    QString fileName = dlg.GetFileName();
    if(fileName.isEmpty())
        return ;

    DataCaseNodeCreator creator;
    QWidget * pWidget = creator.CreateWithEditor(fileName, currentItem(), m_pTabWidget);

    emit setCurrentTabWidget(pWidget);
}

void MainTreeWidget::slotitemActiveDeleteTestDataNode()
{
    QTreeWidgetItem * pItem = currentItem();
     //删除测试数据文件
    QString caseName = pItem->data(0, Qt::UserRole).toString();
    TestDataGroupMgr::GetInst()->DeleteCase(caseName);

    QWidget * pWidget = pItem->data(1, Qt::UserRole).value<QWidget *>();
    m_TestDataSubstreeRootItem->removeChild(pItem);
    m_pTabWidget->removeTab(m_pTabWidget->indexOf(pWidget));//TODO:会不会自动释放pWidget
}

void MainTreeWidget::slotitemActiveNewTestCaseNode()
{
    NewFileDialog dlg;
    if(dlg.exec() !=  QDialog::Accepted)
        return ;

    //测试数据文件名
    QString fileName = dlg.GetFileName();
    if(fileName.isEmpty())
        return ;

    ScriptCaseNodeCreator creator;
    QWidget * pWidget = creator.CreateWithEditor(fileName, currentItem(), m_pTabWidget);

    emit setCurrentTabWidget(pWidget);
}

void MainTreeWidget::slotitemActiveDeleteTestCaseNode()
{
    QTreeWidgetItem * pItem = currentItem();
     //删除测试数据文件
    QString caseName = pItem->data(0, Qt::UserRole).toString();
    ScriptCaseGroupMgr::GetInst()->DeleteCase(caseName);

    QWidget * pWidget = pItem->data(1, Qt::UserRole).value<QWidget *>();
    m_TestScriptCaseSubstreeRootItem->removeChild(pItem);
    m_pTabWidget->removeTab(m_pTabWidget->indexOf(pWidget));//TODO:会不会自动释放pWidget
}

void MainTreeWidget::slotitemActiveNewDBSourceTable()
{
    QTreeWidgetItem *pItem = currentItem();
    if(NULL == pItem)
    {
        return; //TODO:Log
    }


    IMDDataSource *pDBSource = pItem->data(0,Qt::UserRole).value< IMDDataSource* >();
    if( NULL == pDBSource)
    {
        return; //TODO:log
    }

    IMDDataTable * pDBTable = CMDDataSourceFactory::createDataTable();
    CDBTableDialog dlg;
    dlg.initData(pDBSource, pDBTable, TestorGlobalInst::GetDataArchives()->dataGroupManager());
    if(dlg.exec() == QDialog::Accepted)
    {
        if(pDBSource->addDataTable(pDBTable))
        {
            QTreeWidgetItem *pItemTb = new QTreeWidgetItem(QStringList(pDBTable->name()), MainTree_Node_Kind_Data_Source_Table);
            pItemTb->setIcon(0,QIcon(":/img/images/file.png"));
            pItem->addChild(pItemTb);

            QVariant Var;
            Var.setValue (pDBTable);
            pItemTb->setData (0, Qt::UserRole, Var);

            TestorGlobalInst::GetDataArchives()->dataSourceManager()->save();
        }
        else
        {
            delete pDBTable;
            pDBTable = NULL;
        }
    }
    else
    {
        delete pDBTable;
        pDBTable = NULL;
    }
}

void MainTreeWidget::slotitemActiveDeleteDBSourceTable()
{
    IMDDataSourceManager *pDataSrcManager = TestorGlobalInst::GetDataArchives()->dataSourceManager();

    QTreeWidgetItem * pItem = currentItem();
    IMDDataTable *pDBTable = pItem->data(0,Qt::UserRole).value< IMDDataTable* >();
    if(!pDBTable)
    {
        //TODO:log
        return ;
    }

    // 如果选择的是表，取父节点
    QTreeWidgetItem *pParnetItem = pItem->parent();
    if(NULL == pParnetItem)
    {
        return;  //TODO:log
    }

    IMDDataSource *pDBSource = pParnetItem->data(0,Qt::UserRole).value< IMDDataSource* >();
    if(NULL == pDBSource)
    {
        return; //TODO:log
    }

    QMessageBox message;
    message.setWindowTitle(tr("提示"));
    message.addButton(tr("确定"), QMessageBox::YesRole);
    message.addButton(tr("取消"), QMessageBox::NoRole);
    QString strFront = tr("确定删除 ");
    QString strName = "\'" + pDBTable->name() + "\'";
    QString strBack = tr("吗？");
    message.setText(strFront + strName + strBack);

    if (QMessageBox::AcceptRole == message.exec())
    {
        pDBSource->removeDataTable(pDBTable);
        pDataSrcManager->save();

        QWidget * pWidget = pItem->data(1,Qt::UserRole).value<QWidget* >();
        if(pWidget)
        {
            m_pTabWidget->removeTab(m_pTabWidget->indexOf(pWidget));
        }
        //else TODO:log

        pParnetItem->removeChild(pItem);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DBSourceTreeOperator::InitTree(QTreeWidgetItem *pRoot, IMDDataArchives * pDataArchives)
{
    if(NULL == pRoot)
    {
        return;
    }

    if(NULL == pDataArchives)
    {
        return;
    }

    //!TODO:应该加锁.防止在使用这些数据时,被其他线程修改.若被其他线程修改,可能导致崩溃.
    IMDDataSourceManager *pDataSrcManager = pDataArchives->dataSourceManager();
    if(NULL == pDataSrcManager)
    {
        return;
    }

    QString strName = "";
    int count = pDataSrcManager->dataSourceCount();
    for(int i = 0; i < count; i++)
    {
        IMDDataSource *pDataSource = pDataSrcManager->getDataSource(i);
        if(NULL == pDataSource)
        {
            continue;
        }

        strName = pDataSource->name();
        QTreeWidgetItem *pItem = new QTreeWidgetItem(QStringList(strName),  MainTree_Node_Kind_Data_Source);
        pItem->setIcon(0,QIcon(":/img/images/folder.png"));
        pRoot->addChild(pItem);

        //<?>
        QVariant Var;
        Var.setValue (pDataSource);
        pItem->setData (0, Qt::UserRole, Var);
        //</?>

        if (!pDataSource->isShowTable()) //!是否显示数据库中的表
        {
            continue;
        }

        int tabCount = pDataSource->dataTableCount();
        for(int j = 0; j < tabCount; j++)
        {
            IMDDataTable *pTable = pDataSource->getDataTable(j);
            if(NULL == pTable)
            {
                continue;
            }

            QTreeWidgetItem *pItemTable = new QTreeWidgetItem(QStringList(pTable->name()), MainTree_Node_Kind_Data_Source_Table);
            pItemTable->setIcon(0,QIcon(":/img/images/file.png"));
            pItem->addChild(pItemTable);

            QVariant Var;
            Var.setValue (pTable);//?
            pItemTable->setData (0, Qt::UserRole, Var);

        }
    }
}


