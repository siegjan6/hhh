#include "RuntimeFilterDialog.h"
#include "ui_RuntimeFilterDialog.h"
#include "ViewDBSourceDialog.h"
#include <MDDBQueryConfigure.h>
#include <IMDDBQueryEngine.h>
#include <MDDatabaseRecordset.h>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QStringList>
#include <QComboBox>
#include <QDebug>
#include <vector>
#include <QSet>
#include <algorithm>

Q_DECLARE_METATYPE(DBQ::ViewVariable *)

//-------------------------------------------------------------------------------------------------------------------
FiledEnumValueDelegate::FiledEnumValueDelegate(QTableView * parameterTableView, QObject *parent)
    : QItemDelegate(parent)
{
    m_ParameterTableView = parameterTableView;
}

QWidget *FiledEnumValueDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QAbstractItemModel * pModel = m_ParameterTableView->model();
    QVariant vart = pModel->data(pModel->index(index.row(), 0), Qt::UserRole);
    DBQ::ViewVariable * pViewVar = vart.value<DBQ::ViewVariable *>();

    if(!pViewVar) //为空的话 采取默认Editor
        return QItemDelegate::createEditor( parent, option, index);

    if(index.column() != 1) //不是要添加comboBox框的列
        return QItemDelegate::createEditor( parent, option, index);

    if(DBQ::VIEW_VARIABLE_TYPE_ENUM == pViewVar->type
       ||DBQ::VIEW_VARIABLE_TYPE_FIELD == pViewVar->type)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->clear();
        //for(int i = 0 ; i < m_itemList.count(); i++)
        //{
            //editor->addItem(m_itemList.at(i));
        //}
        connect(editor, SIGNAL(activated(int)), this, SLOT(changeCurrentItemSlot(int)));
        return editor;
    }

    return QItemDelegate::createEditor( parent, option, index);
}

void FiledEnumValueDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QAbstractItemModel * pModel = m_ParameterTableView->model();
    QVariant vart = pModel->data(pModel->index(index.row(), 0), Qt::UserRole);
    DBQ::ViewVariable * pViewVar = vart.value<DBQ::ViewVariable *>();

    if(!pViewVar )
    {
        QItemDelegate::setEditorData(editor,index);
        return;
    }

    if(index.column() != 1) //不是comboBox框的列
        return QItemDelegate::setEditorData(editor,index);

    if(DBQ::VIEW_VARIABLE_TYPE_ENUM == pViewVar->type
        ||DBQ::VIEW_VARIABLE_TYPE_FIELD == pViewVar->type)
    {
        int state = index.data().toInt();
        QComboBox *stateBox = static_cast<QComboBox *>(editor);
        if(!stateBox)
            return ; //TODO:log

        stateBox->setCurrentIndex(state);
        return ;
    }

    QItemDelegate::setEditorData(editor,index);
}

void FiledEnumValueDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QAbstractItemModel * pModel = m_ParameterTableView->model();
    QVariant vart = pModel->data(pModel->index(index.row(), 0), Qt::UserRole);
    DBQ::ViewVariable * pViewVar = vart.value<DBQ::ViewVariable *>();

    if(!pViewVar)
    {
        QItemDelegate::setModelData(editor,model,index);
        return;
    }

    if(index.column() != 1) //不是comboBox框的列
    {
        QItemDelegate::setModelData(editor,model,index);
        return;
    }

    if(DBQ::VIEW_VARIABLE_TYPE_FIELD == pViewVar->type)
    {
        QComboBox * stateBox = qobject_cast<QComboBox *>(editor);
        if(!stateBox)
            return ; //TODO:log 不可能

        int state = stateBox->currentIndex();
        //int state = stateBox->currentData().toInt();
        qDebug()<<"Delegate:state="<<state;
        model->setData(index,state);
        stateBox->currentText();
        model->setData(index, stateBox->currentText(), Qt::UserRole+1);//保存当前选项的文本 以便在写文件时获得选中项的文本.
        return ;
    }

    //如果是枚举值 则要写入枚举值而不是描述
    if(DBQ::VIEW_VARIABLE_TYPE_ENUM == pViewVar->type)
    {
        QComboBox * stateBox = qobject_cast<QComboBox *>(editor);
        if(!stateBox)
            return ; //TODO:log 不可能

        int state = stateBox->currentIndex();
        model->setData(index,state);
        stateBox->currentText();
        QString enumValueDescription = stateBox->currentText();

        for(int i=0; i<pViewVar->evs.size(); i++)
        {
            if(enumValueDescription.compare(pViewVar->evs[i].desc, Qt::CaseInsensitive) == 0)
            {
                model->setData(index, pViewVar->evs[i].value, Qt::UserRole+1);//保存当前选项的文本 以便在写文件时获得选中项的文本.
                return ;
            }
       }

        //运行到这里 说明evs中没有当前选项 不符合逻辑
        //TODO:log
        return ;
    }
    QItemDelegate::setModelData(editor,model,index);
}

void FiledEnumValueDelegate::changeCurrentItemSlot(int index)
{
    Q_UNUSED(index);
    QComboBox * editor = qobject_cast<QComboBox *>(sender());
    emit commitData(editor);
}

void FiledEnumValueDelegate::receiveItemListSlot(QStringList & itemList, int row, int col, int currentIndex)
{
    QAbstractItemModel * pModel = m_ParameterTableView->model();
    QVariant vart = pModel->data(pModel->index(row, 0), Qt::UserRole);
    DBQ::ViewVariable * pViewVar = vart.value<DBQ::ViewVariable *>();
    if(!pViewVar)
        return ; //TODO:log

    //由索引获得editor
    QWidget * edtior = m_ParameterTableView->indexWidget(pModel->index(row, col));
    QComboBox * comboBoxeditor = qobject_cast<QComboBox *>(edtior);
    if(!comboBoxeditor)
        return ; //TODO:log
    comboBoxeditor->clear();
    comboBoxeditor->addItems(itemList);

    comboBoxeditor->setCurrentIndex(currentIndex);
}

RuntimeFilterDialog::RuntimeFilterDialog(IMDDBQueryEngine * pDBQueryEngine, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RuntimeFilterDialog),
    m_bIsChanged(false)
{
    ui->setupUi(this);

    m_pDBQueryEngine = pDBQueryEngine;
    if(m_pDBQueryEngine)
        m_ViewPath = m_pDBQueryEngine->getViewPath();

    //m_ViewPath = "D:/MindSCADA/code/DBQuery/TestCase/case8";
    //<左侧列表>
    QStringList viewNames;
    DBQ::View_GetAllViewNameFromDir(m_ViewPath, viewNames);

    m_pViewNameModel = new QStringListModel(viewNames);
    ui->viewListView->setModel(m_pViewNameModel);
    connect(ui->viewListView, SIGNAL(clicked(QModelIndex)), this, SLOT(listViewclicked(QModelIndex)));
    //</左侧列表>

    //<参数表格>
    m_pParameterModel = new QStandardItemModel(0, 3);
    m_pParameterModel->setHeaderData(0, Qt::Horizontal, QObject::tr("参数")); //保存DBQ::ViewVariable * 指针
    m_pParameterModel->setHeaderData(1, Qt::Horizontal, QObject::tr("值"));

    ui->parameterTableView->setModel(m_pParameterModel);
    ui->parameterTableView->hideColumn(2);//第三列保存查询ID.为了查询数据库字段,而不阻塞主线程,采用异步查询的方式.
    ui->parameterTableView->horizontalHeader()->setStretchLastSection(true);

    m_paramViewDelegate = new FiledEnumValueDelegate(ui->parameterTableView, this);
    ui->parameterTableView->setItemDelegate(m_paramViewDelegate);
    connect(this, SIGNAL(comboBoxItemListSignal(QStringList&,int,int,int)), m_paramViewDelegate, SLOT(receiveItemListSlot(QStringList&,int,int,int)));
    //</参数表格>

    //<数据源表格>
    m_pDBSourcsModel  = new QStandardItemModel(0, 2);
    m_pDBSourcsModel->setHeaderData(0, Qt::Horizontal, QObject::tr("数据源"));
    m_pDBSourcsModel->setHeaderData(1, Qt::Horizontal, QObject::tr("数据库"));

    ui->dataSourceTableView->setModel(m_pDBSourcsModel);
    ui->dataSourceTableView->horizontalHeader()->setStretchLastSection(true);
    ui->dataSourceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);    //! 整行选中的方式
    ui->dataSourceTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);     //! 禁止编辑
    //</数据源表格>
    m_nTimerId = startTimer(1000);//100毫秒触发一次

    if(m_pViewNameModel->rowCount()>0)
        emit ui->viewListView->clicked(m_pViewNameModel->index(0, 0));
}

RuntimeFilterDialog::~RuntimeFilterDialog()
{
    if ( m_nTimerId != 0 )
        killTimer(m_nTimerId); //不kill会怎么样?

    delete ui;
    //delete m_pViewNameModel; //TODO:用delete么
}

bool RuntimeFilterDialog::isChanged()
{
    return m_bIsChanged;
}

void RuntimeFilterDialog::timerEvent(QTimerEvent * )
{
    if(!m_pDBQueryEngine)
        return ; //TODO:log

    QString queryIDInTable;
    int row = m_pParameterModel->rowCount();
    int col = 0;
    int recordRow = 0;
    for(int i=0; i<row; i++)
    {
        queryIDInTable = m_pParameterModel->data(m_pParameterModel->index(i, 2)).toString(); //从参数表格中获得查询ID
        if(queryIDInTable.isEmpty())
            continue;

        qDebug()<<"RuntimeFilterDialog::timerEvent:query id:"<<queryIDInTable;

        CMDDatabaseRecordset * pRecordSet = m_pDBQueryEngine->findWithCache(queryIDInTable);
        if(!pRecordSet)
        {
            qDebug()<<"RuntimeFilterDialog::timerEvent:pRecordSet is null";
            continue;
        }

        QStandardItem * pItem = m_pParameterModel->item(i, 2);//查询完毕之后清空id 这样就不会重复更新了
        if(!pItem)
            return ; //TODO:log
        pItem->setData("", Qt::DisplayRole);
        //m_pParameterModel->setItem(i, 2, new QStandardItem(""));//查询完毕之后清空id 这样就不会重复更新了
       //ui->parameterTableView->hideColumn(2);//得加上此行 不然第三行会显示出来

        QVariant vart = m_pParameterModel->data(m_pParameterModel->index(i, 0), Qt::UserRole);
        DBQ::ViewVariable * pViewVar = vart.value<DBQ::ViewVariable *>();

        if(!pViewVar)
        {
            qDebug()<<"RuntimeFilterDialog::timerEvent:pViewVar is null";
            m_pDBQueryEngine->deleteByIDWithCache(queryIDInTable);//???
            continue ; //TODO: log
        }

        col = pRecordSet->getFiledIndex(pViewVar->name);
        if(col<0)
        {
            qDebug()<<"RuntimeFilterDialog::timerEvent:cann't find the index of "<<pViewVar->value;
            m_pDBQueryEngine->deleteByIDWithCache(queryIDInTable);
            continue;
        }

        recordRow = pRecordSet->rowCount();
        QStringList itemList;
        QString strRecord;
        int index = -1;
        for(int r = 0; r<recordRow; r++)
        {
            strRecord = pRecordSet->value(r, col).toString();
            if(pViewVar->value.compare(strRecord, Qt::CaseInsensitive) == 0)
                index = r;

            itemList.append(strRecord);
        }

        emit comboBoxItemListSignal(itemList, i, 1, index);//temp
    }

}

void RuntimeFilterDialog::updateParameterTable(const QModelIndex & index)
{
    QString viewName = index.data().toString();
    //qDebug()<<"clicked:"<<viewName;
    int t1 = m_pParameterModel->rowCount();
    for(int i=t1-1; i>-1; i--)
    {
        m_pParameterModel->removeRow(i); //TODO:释放ViewVariable *内存 关闭对话框时也要释放
    }


    QList<DBQ::ViewVariable *> viewVars;
    DBQ::ViewVariable_GetVarFromViewWhenRuntime(m_ViewPath, viewName, viewVars);
    for(int i=0; i<viewVars.size(); i++)
    {
        DBQ::ViewVariable * pVar = viewVars[i];

        //如果描述信息不会空 则参数名为描述信息.如果为空 则参数名为name
        m_pParameterModel->setItem(i, 0, new QStandardItem(viewVars[i]->description.isEmpty()?viewVars[i]->name:viewVars[i]->description));
        //绑定一个ViewVariable指针，用于更新运行时视图变量
        QVariant variant;//! TODO:提到外面
        variant.setValue(pVar);
        QModelIndex index = m_pParameterModel->index(i, 0);
        m_pParameterModel->setData(index, variant, Qt::UserRole);

        if(DBQ::VIEW_VARIABLE_TYPE_CONST == pVar->type)
        {
            m_pParameterModel->setItem(i, 1, new QStandardItem(viewVars[i]->value));
        }else if(DBQ::VIEW_VARIABLE_TYPE_ENUM == pVar->type) //枚举
        {
            QStringList itemList;
            int indexEnum = -1;
            for(int j = 0; j<pVar->evs.size(); j++)
            {
                itemList.append(pVar->evs[j].desc);
                if(pVar->evs[j].value.compare(pVar->value, Qt::CaseInsensitive) == 0)
                    indexEnum = j;
            }
            //openPersistentEditor:始终显示编辑框 一定要在emit之前 不然代理类中获得不了此edtior.
            ui->parameterTableView->openPersistentEditor( m_pParameterModel->index(i, 1));
            emit comboBoxItemListSignal(itemList, i, 1, indexEnum);
        }else if(DBQ::VIEW_VARIABLE_TYPE_FIELD == pVar->type) //数据库中的一个字段 要查数据库
        {
            ui->parameterTableView->openPersistentEditor( m_pParameterModel->index(i, 1));

            if(!m_pDBQueryEngine)
                return ; //TODO:log

            QString queryID;
            if(m_pDBQueryEngine->queryViewWithCache(viewName+"."+pVar->name, queryID))
            {
                //将queryID设置到隐藏列中
                 m_pParameterModel->setItem(i, 2, new QStandardItem(queryID));//隐藏列 不显示
            }
            else
            {
                //TODO:log
            }
        }//end if
    }//end for
}

void RuntimeFilterDialog::updateDataSourceTable(const QModelIndex & index)
{
    QString viewName = index.data().toString();
    updateDataSourceTable(viewName);
}

void RuntimeFilterDialog::updateDataSourceTable(const QString & viewName)
{
    int t1 = m_pDBSourcsModel->rowCount();
    for(int i=t1-1; i>-1; i--)
    {
        m_pDBSourcsModel->removeRow(i); //TODO:释放ViewVariable *内存 关闭对话框时也要释放
    }

    //加载view 获得view中配置的数据源
    DBQ::View * pView = 0;//TODO:释放内存
    QString errStr;
    if(!DBQ::View_LoadFromDir(m_ViewPath, viewName, &pView, errStr))
    {
        qDebug()<<errStr; //TODO:log
        return ;
    }

    //如果没有配置数据源 则设置成当前数据源
    if(pView->dbSources.size() == 0)
    {
        m_pDBSourcsModel->setItem(0, 0, new QStandardItem("当前"));
        m_pDBSourcsModel->setItem(0, 1, new QStandardItem("当前"));
        return ;
    }

    int rowCount = 0;
    for(int i=0; i<pView->dbSources.size(); i++)
    {
        for(int j=0; j<pView->dbSources[i].dbNames.size(); j++)
        {
            m_pDBSourcsModel->setItem(rowCount, 0, new QStandardItem( pView->dbSources[i].dbSrcName));
            m_pDBSourcsModel->setItem(rowCount, 1, new QStandardItem(pView->dbSources[i].dbNames[j]));
            rowCount++;
        }
    }

}

void RuntimeFilterDialog::listViewclicked(const QModelIndex & index)
{
    updateParameterTable(index);
    updateDataSourceTable(index);
}

void RuntimeFilterDialog::on_paraSaveButton_clicked()
{
    //从左侧ListView获得当前视图名称
    //m_pViewNameModel->
    QModelIndex index = ui->viewListView->currentIndex();
    QString viewName = m_pViewNameModel->data(index, Qt::EditRole).toString();

    if(viewName.isEmpty())
    {
        accept();//关闭对话框
        return ;
    }

    m_bIsChanged = true;

    //遍历所有参数行 将其值写入运行时视图变量中
    QList<DBQ::RuntimeViewVariable *> rtVarsNew;
    int count = m_pParameterModel->rowCount();
    for(int i=0; i<count; i++)
    {
        //Qt::UserRole
        QVariant vart = m_pParameterModel->data(m_pParameterModel->index(i, 0), Qt::UserRole);
        DBQ::ViewVariable * pViewVar = vart.value<DBQ::ViewVariable *>();
        if(!pViewVar)
            continue ; //

        DBQ::RuntimeViewVariable * pRunViewVar = new DBQ::RuntimeViewVariable;
        pRunViewVar->key = pViewVar->name;

        if(DBQ::VIEW_VARIABLE_TYPE_CONST == pViewVar->type
            ||DBQ::VIEW_VARIABLE_TYPE_FUNC == pViewVar->type)
        {
            pRunViewVar ->value = m_pParameterModel->data(m_pParameterModel->index(i, 1)).toString();
        }else if(DBQ::VIEW_VARIABLE_TYPE_ENUM == pViewVar->type
                    ||DBQ::VIEW_VARIABLE_TYPE_FIELD == pViewVar->type)
        {
            pRunViewVar ->value = m_pParameterModel->data(m_pParameterModel->index(i, 1), Qt::UserRole+1).toString();
        }
        else
        {
            //TODO:log
            delete pRunViewVar;
            continue;
        }

        rtVarsNew.append(pRunViewVar);
    }

                //VIEW_VARIABLE_TYPE_ENUM,                   //枚举
    QString errStr;
    DBQ::RuntimeViewVariable_UpdateInDir(m_ViewPath, viewName, rtVarsNew, errStr);
    qDeleteAll(rtVarsNew);

    accept();//关闭对话框
}

//添加数据源
void RuntimeFilterDialog::on_dsAddButton_clicked()
{
    QModelIndex index = ui->viewListView->currentIndex();
    QString viewName = m_pViewNameModel->data(index, Qt::EditRole).toString();
    if(viewName.isEmpty())
        return ;

    ViewDBSourceDialog dlg;
    dlg.setDBQueryPath(m_ViewPath);
    dlg.initViewDBSource();
    if(dlg.exec() != QDialog::Accepted)
        return ;
     QList<DBQ::ViewDBSource> dbsList = dlg.ViewDBsourceList();
     if(dbsList.isEmpty())
         return ;

     m_bIsChanged = true;

     for(int i=0; i<dbsList.size(); i++)
     {
         for(int j=0; j<dbsList[i].dbNames.size(); j++)
            DBQ::View_AddDB(m_ViewPath, viewName, dbsList[i].dbSrcName, dbsList[i].dbNames[j]);
     }

     updateDataSourceTable(viewName);//如果视图中没有了数据源 要显示当前数据源
}

//删除数据源
void RuntimeFilterDialog::on_dsDelButton_clicked()
{
    //从左侧ListView获得当前视图名称
    QModelIndex index = ui->viewListView->currentIndex();
    QString viewName = m_pViewNameModel->data(index, Qt::EditRole).toString();

    if(viewName.isEmpty())
        return ;

    QItemSelectionModel *selections = ui->dataSourceTableView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    QModelIndex indexDS;
    QString dsName;
    QModelIndex indexDB;
    QString dbName;

    std::vector<int> rowsSelected;
    foreach (QModelIndex index, selected)
    {
        //qDebug()<<index;
        //设置了按行选中的模式 所以会返回所有选中行的cell
        if(index.column() == 1) //已经取出了(row, 1)的值 不用再此获得了
            continue;
        rowsSelected.push_back(index.row());

        indexDS = m_pDBSourcsModel->index(index.row(), 0); //数据源名
        dsName = m_pDBSourcsModel->data(indexDS).toString();
        indexDB = m_pDBSourcsModel->index(index.row(), 1); //数据库名
        dbName = m_pDBSourcsModel->data(indexDB).toString();

        DBQ::View_DeleteDB(m_ViewPath, viewName, dsName, dbName);
        m_bIsChanged = true;
    }

    //删除选中行
    std::sort(rowsSelected.begin(), rowsSelected.end(), std::greater<int>()); //降序排列
    for(int i=0; i<(int)rowsSelected.size(); i++) //(int)去掉警告
        m_pDBSourcsModel->removeRow(rowsSelected[i]);

    updateDataSourceTable(viewName);//如果视图中没有了数据源 要显示当前数据源
}

void RuntimeFilterDialog::on_paraCancelButton_clicked()
{
    reject();//关闭对话框
}
