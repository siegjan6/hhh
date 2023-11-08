#include "ViewVarTableDialog.h"
#include "ui_ViewVarTableDialog.h"
#include "NewViewVarDialog.h"
#include <QDebug>
#include <QMessageBox>

ViewVarTableDialog::ViewVarTableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewVarTableDialog),
    m_pModel(new QStandardItemModel(0,5)),
    m_pViewVarPopupMenu(NULL),
    m_pViewVar_NewAction(NULL),
    m_pViewVar_DeleteAction(NULL),
    m_pViewVar_ModifyAction(NULL),
    m_DBQueryPath("")
{
    ui->setupUi(this);

    initTableView();

    QWidget::setTabOrder(ui->tableView,ui->pbt_Confirm);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->tableView);
}

ViewVarTableDialog::~ViewVarTableDialog()
{
    delete ui;
    if(m_pModel)
    {
        delete m_pModel;
        m_pModel = NULL;
    }
    if(m_pViewVarPopupMenu)
    {
        delete m_pViewVarPopupMenu;
        m_pViewVarPopupMenu = NULL;
    }
    if(m_pViewVar_NewAction)
    {
        delete m_pViewVar_NewAction;
        m_pViewVar_NewAction = NULL;
    }
    if(m_pViewVar_DeleteAction)
    {
        delete m_pViewVar_DeleteAction;
        m_pViewVar_DeleteAction = NULL;
    }
    if(m_pViewVar_ModifyAction)
    {
        delete m_pViewVar_ModifyAction;
        m_pViewVar_ModifyAction = NULL;
    }
}

void ViewVarTableDialog::initTableView()
{
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    disconnect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_doubleClickedModifyViewVar(QModelIndex)));
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);    //! 整行选中的方式
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);     //! 禁止编辑//! 隐藏计数列
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_showMenu(QPoint)));
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slot_doubleClickedModifyViewVar(QModelIndex)));
    connect(ui->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slot_sortByColumn(int)));
    for(int i = 0 ; i < 5; i ++)
    {
        ui->tableView->sortByColumn(i,Qt::AscendingOrder);
    }
}

bool ViewVarTableDialog::LoadViewVarTable(const QString &configFileDir)
{
    m_viewVarList.clear();
    QList<DBQ::ViewVariable*> list;
    QString errIn = "";
    bool ok = DBQ::ViewVariable_Load(configFileDir,DBQ::FILE_TYPE_JSON,list,errIn);
    if(!ok)
    {
        return false;
    }

    foreach (DBQ::ViewVariable *viewVar, list)
    {
        m_viewVarList.push_back(viewVar);
    }
    return true;
}

void ViewVarTableDialog::creatTableView()
{
    m_pModel->clear();

    QStringList headerList;
    headerList << QObject::tr("变量名") << QObject::tr("变量类型") << QObject::tr("数值类型")<< QObject::tr("值") << QObject::tr("描述");
    m_pModel->setHorizontalHeaderLabels(headerList);
    ui->tableView->setModel(m_pModel);

    for(int i = 0 ; i < m_viewVarList.count(); i++)
    {
        DBQ::ViewVariable * pViewVar = new DBQ::ViewVariable;
        *pViewVar = *m_viewVarList[i];

        QModelIndex index = m_pModel->index(i, 0);
        if(index.isValid())
        {
            QVariant var = m_pModel->data(index, Qt::UserRole);
            DBQ::ViewVariable *pViewVarOld = var.value<DBQ::ViewVariable *>();
            if(pViewVarOld)
                delete pViewVarOld;
        }

        m_pModel->setItem(i, 0, new QStandardItem(pViewVar->name));
        m_pModel->setItem(i, 1, new QStandardItem(getViewVariableTypeString(pViewVar->type)));
        m_pModel->setItem(i, 2, new QStandardItem(View_GetFieldTypeStringFromType(pViewVar->valueType)));

        if(pViewVar->type == DBQ::VIEW_VARIABLE_TYPE_ENUM)
        {
            QStringList list;
            QString currentText = "";
            list.clear();
            int size = pViewVar->evs.count();
            for(int j = 0 ; j < size; j++)
            {
                currentText = pViewVar->evs.at(j).value + "," + pViewVar->evs.at(j).desc;
                list.append(currentText);
            }
            EnumValueDelegate *pComboDelegate = new EnumValueDelegate(i,list,ui->tableView);
            ui->tableView->setItemDelegateForColumn(3,pComboDelegate);
            ui->tableView->openPersistentEditor( m_pModel->index(i, 3));
        }
        else
        {
            m_pModel->setItem(i, 3, new QStandardItem(pViewVar->value));
            m_pModel->item(i,3)->setTextAlignment(Qt::AlignCenter);
        }

        m_pModel->setItem(i, 4, new QStandardItem(pViewVar->description));

        QVariant variant;//! TODO:提到外面
        variant.setValue(pViewVar);
        index = m_pModel->index(i, 0);
        m_pModel->setData(index, variant, Qt::UserRole);
        m_pModel->item(i,0)->setTextAlignment(Qt::AlignCenter);
        m_pModel->item(i,1)->setTextAlignment(Qt::AlignCenter);
        m_pModel->item(i,2)->setTextAlignment(Qt::AlignCenter);
        m_pModel->item(i,4)->setTextAlignment(Qt::AlignCenter);
    }
}

QList<DBQ::ViewVariable *> ViewVarTableDialog::getViewVarListSelects() const
{
    return m_viewVarListSelects;
}

QList<DBQ::ViewVariable *> ViewVarTableDialog::getViewVarList() const
{
    return m_viewVarList;
}

QString ViewVarTableDialog::getViewVariableTypeString(DBQ::VIEW_VARIABLE_TYPE valueType)
{
    switch (valueType)
    {
    case DBQ::VIEW_VARIABLE_TYPE_UNKNOWN: return "未知";
    case DBQ::VIEW_VARIABLE_TYPE_CONST: return "常量";
    case DBQ::VIEW_VARIABLE_TYPE_ENUM: return "枚举";
    case DBQ::VIEW_VARIABLE_TYPE_FIELD: return "字段";
    case DBQ::VIEW_VARIABLE_TYPE_FUNC: return "函数";
    }

    return "未知";
}

bool ViewVarTableDialog::deleteViewVariable(int index)
{
    bool success = false;
    if (0 <= index && index < m_viewVarList.count())
    {
        m_viewVarList.removeAt(index);
        success = true;
    }
    return success;
}

void ViewVarTableDialog::slot_doubleClickedModifyViewVar(QModelIndex index)
{
    QModelIndex indexSelected = index;
    QVariant variant;

    indexSelected = m_pModel->index(index.row(), 0);
    variant = m_pModel->data(indexSelected, Qt::UserRole);

    DBQ::ViewVariable *pViewVar = variant.value<DBQ::ViewVariable *>();
    if(pViewVar)
    {
        NewViewVarDialog dlg(1,tr("修改视图变量"),this);
        dlg.setDataArchivesFilePath(m_DBQueryPath.left(m_DBQueryPath.length()-7) + "DataArchives/" + QString("dataSource.dat"));
        dlg.setDBQueryPath(m_DBQueryPath);
        dlg.setOriginalViewVarParameter(pViewVar);

        if(dlg.exec() == QDialog::Accepted)
        {
            if(dlg.getViewVar())
            {
                if(getViewVariable(pViewVar->name))
                {
                    deleteViewVariable(indexSelected.row());
                    m_viewVarList.push_back(dlg.getViewVar());
                    creatTableView();
                    ui->tableView->update();
                }
            }
        }
    }
}

void ViewVarTableDialog::slot_sortByColumn(int column)
{
    if (column >= 5 || column < 0)
        return;
    ui->tableView->sortByColumn(column,Qt::AscendingOrder);
}

QModelIndexList ViewVarTableDialog::deleteRepeatList(QModelIndexList indexList)
{
    QModelIndex index,newIndex;
    QModelIndexList newIndexList;
    foreach(index,indexList)
    {
        if (newIndex.row() != index.row())
        {
            newIndex = index;
            newIndexList.append(newIndex);
        }
    }
    return newIndexList;
}

DBQ::ViewVariable *ViewVarTableDialog::getViewVariable(const QString &name)
{
    DBQ::ViewVariable *pViewVar = NULL;
    int size = m_viewVarList.size();
    for (int i = 0; i < size; ++i)
    {
        pViewVar = m_viewVarList.at(i);
        if (pViewVar && (pViewVar->name == name))
        {
            return pViewVar;
        }
    }

    return NULL;
}

void ViewVarTableDialog::slot_showMenu(const QPoint &pos)
{
    if(!m_pViewVarPopupMenu)
    {
        m_pViewVarPopupMenu = new QMenu(tr("新建"),ui->tableView);

        if(!m_pViewVar_NewAction)
        {
            m_pViewVar_NewAction = new QAction(tr("添加变量"),ui->tableView);
            connect(m_pViewVar_NewAction,SIGNAL(triggered()),this,SLOT(slot_NewViewVariable()));
            m_pViewVarPopupMenu->addAction(m_pViewVar_NewAction);
        }
        if(!m_pViewVar_DeleteAction)
        {
            m_pViewVar_DeleteAction = new QAction(tr("删除变量"),ui->tableView);
            connect(m_pViewVar_DeleteAction,SIGNAL(triggered()),this,SLOT(slot_DeleteViewVariable()));
            m_pViewVarPopupMenu->addAction(m_pViewVar_DeleteAction);
        }
        if(!m_pViewVar_ModifyAction)
        {
            m_pViewVar_ModifyAction = new QAction(tr("修改变量"),ui->tableView);
            connect(m_pViewVar_ModifyAction,SIGNAL(triggered()),this,SLOT(slot_ModifyViewVariable()));
            m_pViewVarPopupMenu->addAction(m_pViewVar_ModifyAction);
        }
    }
    m_pViewVarPopupMenu->exec(ui->tableView->mapToGlobal(pos));
}

void ViewVarTableDialog::slot_NewViewVariable()
{
    NewViewVarDialog dlg(0,tr("新建视图变量"),this);
    dlg.setDataArchivesFilePath(m_DBQueryPath.left(m_DBQueryPath.length()-7) + "DataArchives/" + QString("dataSource.dat"));
    dlg.setDBQueryPath(m_DBQueryPath);
    if(dlg.exec() == QDialog::Accepted)
    {
        QString viewVarName = dlg.getViewVar()->name;
        if(viewVarName.isEmpty())
        {
            QMessageBox::information(NULL,"警告",tr("当前视图变量名称为空!"));
            return;
        }
        DBQ::ViewVariable *pviewVar = dlg.getViewVar();
        if(dlg.getViewVar() &&  NULL == getViewVariable(viewVarName))
        {
            m_viewVarList.append(pviewVar);
            creatTableView();
            ui->tableView->update();
        }
        else
        {
            QMessageBox::information(NULL,"警告",tr("%1当前变量名称已经存在!").arg(viewVarName));
            return;
        }
    }
}

void ViewVarTableDialog::slot_DeleteViewVariable()
{
    QModelIndexList indexList1 = ui->tableView->selectionModel()->selectedIndexes();
    if(indexList1.isEmpty())
        return;
    QString strMsg = QString(tr("确定删除当前选中的视图变量吗？"));
    QMessageBox message(QMessageBox::Information,"提示", strMsg, QMessageBox::Yes | QMessageBox::No, NULL);

    if (QMessageBox::Yes == message.exec())
    {
        while (!indexList1.isEmpty())
        {
            QModelIndexList indexList = deleteRepeatList(indexList1);
            QModelIndex index = indexList.first();
            m_pModel->removeRow(index.row());
            if(deleteViewVariable(index.row()))
            {
                qDebug()<<"成功删除当前选中行!"<<endl;
            }
            indexList1 = ui->tableView->selectionModel()->selectedIndexes();
        }
    }
}

void ViewVarTableDialog::slot_ModifyViewVariable()
{
    QVariant variant;
    QItemSelectionModel *selections = ui->tableView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    QModelIndex indexSelected;
    foreach (QModelIndex index, selected)
    {
        //qDebug()<<index;
        //设置了按行选中的模式 所以会返回所有选中行的cell
        if(index.column() != 0)
            continue;

        indexSelected = m_pModel->index(index.row(), 0);

        variant = m_pModel->data(indexSelected, Qt::UserRole);
        DBQ::ViewVariable *pViewVar = variant.value<DBQ::ViewVariable *>();
        if(pViewVar)
        {
            NewViewVarDialog dlg(1,tr("修改视图变量"),this);
            dlg.setDataArchivesFilePath(m_DBQueryPath.left(m_DBQueryPath.length()-7) + "DataArchives/" + QString("dataSource.dat"));
            dlg.setDBQueryPath(m_DBQueryPath);
            dlg.setOriginalViewVarParameter(pViewVar);

            if(dlg.exec() == QDialog::Accepted)
            {
                if(dlg.getViewVar())
                {
                    if(getViewVariable(pViewVar->name))
                    {
                        deleteViewVariable(indexSelected.row());
                        m_viewVarList.push_back(dlg.getViewVar());
                        creatTableView();
                    }
                }
            }
        }
    }
}

void ViewVarTableDialog::on_pbt_Confirm_clicked()
{
    foreach(DBQ::ViewVariable * pViewVariable, m_viewVarListSelects)
    {
        delete pViewVariable;
    }
    m_viewVarListSelects.clear();

    QItemSelectionModel *selections = ui->tableView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    QVariant variant;
    QModelIndex indexSelected;
    foreach (QModelIndex index, selected)
    {
        //! 设置了按行选中的模式 所以会返回所有选中行的cell
        if(index.column() != 0)
            continue;

        indexSelected = m_pModel->index(index.row(), 0);
        variant = m_pModel->data(indexSelected, Qt::UserRole);
        DBQ::ViewVariable * pViewVar = variant.value<DBQ::ViewVariable *>();
        Q_ASSERT(pViewVar);
        DBQ::ViewVariable * pViewVarCp = new DBQ::ViewVariable;
        *pViewVarCp = *pViewVar;
        m_viewVarListSelects.push_back(pViewVarCp);
    }
    for(int i = 0; i < m_viewVarListSelects.count(); i++)
    {
        qDebug()<<m_viewVarListSelects.at(i)->name<<"Selected viewVarName"<<endl;
    }

    QString errIn;
    QString viewVarFullPath = m_DBQueryPath + QString("/global_view_vars.json");
    bool ok = DBQ::ViewVariable_Write(viewVarFullPath,DBQ::FILE_TYPE_JSON,m_viewVarList,errIn);
    if(ok)
    {
        qDebug()<<"成功保存视图变量链表!"<<endl;
    }
    this->accept();
}

void ViewVarTableDialog::on_pbt_Cancel_clicked()
{
    this->reject();
}
QString ViewVarTableDialog::DBQueryPath() const
{
    return m_DBQueryPath;
}

void ViewVarTableDialog::setDBQueryPath(const QString &DBQueryPath)
{
    m_DBQueryPath = DBQueryPath;
}




///////////////////////////////////////////////////////////////////////////////////////

EnumValueDelegate::EnumValueDelegate(int row, QStringList menuList, QObject *parent): QItemDelegate(parent)
{
    int size = menuList.size();
    for(int i = 0; i < size; i++)
    {
        m_itemList.push_back(menuList[i]);
    }
    m_row = row;
}

QWidget *EnumValueDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.row() != m_row)
    {
        return QItemDelegate::createEditor( parent, option, index);
    }
    QComboBox *editor = new QComboBox(parent);
    editor->clear();
    int size = m_itemList.size();
    for(int i = 0 ; i < size; i++)
    {
        editor->addItem(m_itemList.at(i));
    }
    return editor;
}

void EnumValueDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.row() != m_row)
    {
        QItemDelegate::setEditorData(editor,index);
        editor->setDisabled(true);
        return;
    }
    int state = index.data().toInt();
    QComboBox *stateBox = static_cast<QComboBox *>(editor);
    stateBox->setCurrentIndex(state);
}

void EnumValueDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.row() != m_row)
    {
        QItemDelegate::setModelData(editor,model,index);
        return;
    }
    QComboBox *stateBox = static_cast<QComboBox *>(editor);
    int state = stateBox->currentData().toInt();
    model->setData(index,state);
}

void EnumValueDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.row() != m_row)
    {
        QItemDelegate::updateEditorGeometry(editor,option,index);
        return;
    }
    editor->setGeometry(option.rect);
}
