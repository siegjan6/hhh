#include "ViewFilterFieldDialog.h"
#include "ui_ViewFilterFieldDialog.h"
#include "SourceTreeDialog.h"
#include "ViewFieldsDialog.h"
#include "ViewVarTableDialog.h"
#include "NewFieldDialog.h"
#include <QMessageBox>

ViewFilterFieldDialog::ViewFilterFieldDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewFilterFieldDialog),
    m_Field(new DBQ::ViewField),
    m_Field_Menu(NULL),
    m_DataArchivesField_Action(NULL),
    m_ViewField_Action(NULL),
    m_NewField_Action(NULL),
    m_DBQueryPath(""),
    m_pView(0)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slot_showMenu(QPoint)));
    connect(ui->comboBox_ParamenterType,SIGNAL(currentIndexChanged(int)),this,SLOT(showCurrentViewVar(int)));
    setConstParamenterTabOrder();
}

ViewFilterFieldDialog::~ViewFilterFieldDialog()
{
    delete ui;
}

void ViewFilterFieldDialog::setDBQueryPath(const QString &DBQueryPath)
{
    m_DBQueryPath = DBQueryPath;
    loadViewVarList();
}

void ViewFilterFieldDialog::setView(DBQ::View * pView)
{
    m_pView = pView;
}

DBQ::VIEW_FILTER_COMPARE_TYPE ViewFilterFieldDialog::getCompare()
{
    switch (ui->comboBox_Comparison->currentIndex())
    {
    case 0: return DBQ::VIEW_FILTER_COMPARE_TYPE_GREATER_THAN;           //>
    case 1: return DBQ::VIEW_FILTER_COMPARE_TYPE_EQUAL;                           //=
    case 2: return DBQ::VIEW_FILTER_COMPARE_TYPE_LESS_THAN;                   //<
    case 3: return DBQ::VIEW_FILTER_COMPARE_TYPE_GREATER_EQUAL;         //>=
    case 4: return DBQ::VIEW_FILTER_COMPARE_TYPE_LESS_EQUAL;                 //<=
    case 5: return DBQ::VIEW_FILTER_COMPARE_TYPE_NOT_EQUAL;                  //!=
    case 6: return DBQ::VIEW_FILTER_COMPARE_TYPE_LIKE;                               //like
    case 7: return DBQ::VIEW_FILTER_COMPARE_TYPE_NOT_LIKE;                      //not like
    default: return DBQ::VIEW_FILTER_COMPARE_TYPE_EQUAL;                           //=
    }
}

QString ViewFilterFieldDialog::getViewFieldTypeString(DBQ::VIEW_FIELD_TYPE fieldType)
{
    return DBQ::View_GetFieldTypeStringFromType(fieldType);
}

void ViewFilterFieldDialog::setFieldParamenter()
{
    if(m_Field)
    {
        if(m_Field->tableName.isEmpty())
        {
            QMessageBox::information(NULL,"警告",tr("字段表名为空!"));
            return;
        }
        if(m_Field->fieldName.isEmpty())
        {
            QMessageBox::information(NULL,"警告",tr("字段名称为空!"));
            return;
        }
        ui->lineEdit_tableName->setText(m_Field->tableName);
        ui->lineEdit_FieldName->setText(m_Field->fieldName);
        ui->lineEdit_FieldType->setText(getViewFieldTypeString(m_Field->type));
        ui->lineEdit_FieldDescription->setText(m_Field->description);
    }
}

void ViewFilterFieldDialog::setCompareComboBox(DBQ::VIEW_FILTER_COMPARE_TYPE type)
{
    int index = -1;
    switch (type)
    {
    case DBQ::VIEW_FILTER_COMPARE_TYPE_GREATER_THAN: index=0;break;
    case DBQ::VIEW_FILTER_COMPARE_TYPE_EQUAL: index=1;break;
    case DBQ::VIEW_FILTER_COMPARE_TYPE_LESS_THAN: index=2;break;
    case DBQ::VIEW_FILTER_COMPARE_TYPE_GREATER_EQUAL: index=3;break;
    case DBQ::VIEW_FILTER_COMPARE_TYPE_LESS_EQUAL: index=4;break;
    case DBQ::VIEW_FILTER_COMPARE_TYPE_NOT_EQUAL: index=5;break;
    case DBQ::VIEW_FILTER_COMPARE_TYPE_LIKE: index=6;break;
    case DBQ::VIEW_FILTER_COMPARE_TYPE_NOT_LIKE: index=7;break;
    default:  index=1;break; //TODO:log
    }
    ui->comboBox_Comparison->setCurrentIndex(index);
}

void ViewFilterFieldDialog::setRightValueUI(DBQ::ViewFilterNode * pNode)
{
    if(DBQ::VIEW_FILTER_RIGHT_VALUE_TYPE_CONST == pNode->rightValueType)//常量类型
    {
        ui->comboBox_ParamenterType->setCurrentIndex(0);
        ui->lineEdit_ParamenterValue->setText(pNode->rightValue);
    }
    else
    {
        ui->comboBox_ParamenterType->setCurrentIndex(1); //变量类型
        ui->comboBox_ViewVarList->setCurrentText(pNode->rightValue);//TODO:在全局视图变量中查找 验证此变量是否存在.
    }

}

void ViewFilterFieldDialog::setConstParamenterTabOrder()
{
    QWidget::setTabOrder(ui->comboBox_Comparison, ui->comboBox_ParamenterType);
    QWidget::setTabOrder(ui->comboBox_ParamenterType, ui->lineEdit_ParamenterValue);
    QWidget::setTabOrder(ui->lineEdit_ParamenterValue, ui->pbt_Confirm);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->comboBox_Comparison);
}

void ViewFilterFieldDialog::setVarParamenterTabOrder()
{
    QWidget::setTabOrder(ui->comboBox_Comparison, ui->comboBox_ParamenterType);
    QWidget::setTabOrder(ui->comboBox_ParamenterType, ui->comboBox_ViewVarList);
    QWidget::setTabOrder(ui->comboBox_ViewVarList, ui->pbt_Confirm);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->comboBox_Comparison);
}

void ViewFilterFieldDialog::loadViewVarList()
{
    QList<DBQ::ViewVariable*> viewVarList;
    QString errIn = "";
    if(DBQ::ViewVariable_LoadFromDir(m_DBQueryPath,viewVarList,errIn))
    {
        int size = viewVarList.size();
        for(int i = 0; i < size; i++)
        {
            ui->comboBox_ViewVarList->addItem(viewVarList.at(i)->name);
        }
    }
}

void ViewFilterFieldDialog::setFilterItem(DBQ::ViewFilterNode * pFilterNode)
{
    if(!pFilterNode)
        return ;//TODO:log
    if(pFilterNode->type != DBQ::VIEW_FILTER_NODE_TYPE_LEAF)
        return ;//TODO:log

    //pFilterNode
    //获得字段 更新界面的字段部分
    DBQ::ViewField * pField = 0;
    if(!(pField=DBQ::View_GetFieldByTableFieldName(m_pView, pFilterNode->leftValue)))
        return ;//TODO:log
    *m_Field = *pField;
    setFieldParamenter();

    //更新比较条件
    setCompareComboBox(pFilterNode->compare);
    //更新比较值
    setRightValueUI(pFilterNode);
}

DBQ::ViewFilterNode * ViewFilterFieldDialog::getResult()
{
    DBQ::ViewFilterNode * pNode = new DBQ::ViewFilterNode;
    //过滤项左值:字段名称  表名.字段名
    pNode->leftValue = ui->lineEdit_tableName->text() + "." + ui->lineEdit_FieldName->text();
    pNode->leftType = m_Field->type; //左值类型
    pNode->compare = getCompare(); //获取比较关系
    //! 获取过滤项右值


    //! 获取过滤项右值类型
     switch (ui->comboBox_ParamenterType->currentIndex())
     {
     case 0:
     {
         pNode->rightValueType = DBQ::VIEW_FILTER_RIGHT_VALUE_TYPE_CONST;
         pNode->rightValue = ui->lineEdit_ParamenterValue->text();
         break;
     }
     case 1:
     {
         pNode->rightValueType = DBQ::VIEW_FILTER_RIGHT_VALUE_TYPE_VAR;
         pNode->rightValue = ui->comboBox_ViewVarList->currentText();
         break;
     }
     default:
         delete pNode;
         return 0;
     }

     return pNode;
}

void ViewFilterFieldDialog::showCurrentViewVar(int index)
{
    ui->stackedWidget->setCurrentIndex(index);

    switch (index)
    {
    case 0:
        setConstParamenterTabOrder();
        break;
    case 1:
        setVarParamenterTabOrder();
        break;
    default:
        break;
    }
}

void ViewFilterFieldDialog::slot_showMenu(const QPoint &pos)
{
    Q_UNUSED(pos);
#if 0
    if(!m_Field_Menu)
    {
        m_Field_Menu = new QMenu(tr("新建"),this);

        if(!m_DataArchivesField_Action)
        {
            m_DataArchivesField_Action = new QAction(tr("从数据归档获取字段"),m_Field_Menu);
            connect(m_DataArchivesField_Action,SIGNAL(triggered()),this,SLOT(slot_DataArchivesField_Action()));
            m_Field_Menu->addAction(m_DataArchivesField_Action);
        }
        if(!m_ViewField_Action)
        {
            m_ViewField_Action = new QAction(tr("从视图获取字段"),m_Field_Menu);
            connect(m_ViewField_Action,SIGNAL(triggered()),this,SLOT(slot_ViewField_Action()));
            m_Field_Menu->addAction(m_ViewField_Action);
        }
        if(!m_NewField_Action)
        {
            m_NewField_Action = new QAction(tr("新建字段"),m_Field_Menu);
            connect(m_NewField_Action,SIGNAL(triggered()),this,SLOT(slot_NewField_Action()));
            m_Field_Menu->addAction(m_NewField_Action);
        }
    }
    m_Field_Menu->exec(mapToGlobal(pos));
#endif
}

void ViewFilterFieldDialog::slot_DataArchivesField_Action()
{
    QString dataSrcFullPath = m_DBQueryPath.left(m_DBQueryPath.length()-7) + "DataArchives/" + QString("dataSource.dat");
    SourceTreeDialog dlg(QString(tr("视图数据源字段")),dataSrcFullPath);
    QStringList dbsourceList;
    if(DBQ::DBSource_GetNames(m_DBQueryPath,dbsourceList))
    {
        dlg.setDbsourceNameList(dbsourceList);
        dlg.initDataSourceTree();
        dlg.setReturnMode(1);
        dlg.setStackWidgetShowMode(1);
        if(dlg.exec() == QDialog::Accepted)
        {
            if(dlg.getViewField())
            {
                *m_Field = *dlg.getViewField();
                setFieldParamenter();
            }
        }
    }
}

void ViewFilterFieldDialog::slot_ViewField_Action()
{
    if(!m_pView)
        return ;//TODO:log

    ViewFieldsDialog dlg;
    dlg.LoadView(m_DBQueryPath, m_pView->viewName);
    QList<DBQ::ViewField *> fields;
    if(dlg.exec() == QDialog::Accepted)
    {
        dlg.GetFields(fields);

        if(fields.size()>0)
        {
            *m_Field = *fields.at(0);
            setFieldParamenter();
        }
    }
}

void ViewFilterFieldDialog::slot_NewField_Action()
{
    NewFieldDialog dlg(QString(tr("新建字段")));
    if(dlg.exec() == QDialog::Accepted)
    {
        if(dlg.getField())
        {
            *m_Field = *dlg.getField();
            setFieldParamenter();
        }
    }
}

void ViewFilterFieldDialog::on_pbt_viewField_clicked()
{
    if(!m_pView)
        return ;//TODO:log

    ViewFieldsDialog dlg;
    dlg.LoadView(m_DBQueryPath, m_pView->viewName);
    QList<DBQ::ViewField *> fields;
    if(dlg.exec() == QDialog::Accepted)
    {
        dlg.GetFields(fields);

        if(fields.size()>0)
        {
            *m_Field = *fields.at(0);
            setFieldParamenter();
        }
    }
}

/*
void ViewFilterFieldDialog::on_pbt_newField_clicked()
{
    NewFieldDialog dlg(QString(tr("新建字段")));
    if(dlg.exec() == QDialog::Accepted)
    {
        if(dlg.getField())
        {
            *m_Field = *dlg.getField();
            setFieldParamenter();
        }
    }
}
*/

void ViewFilterFieldDialog::on_pbt_Confirm_clicked()
{
    if(ui->comboBox_ParamenterType->currentIndex() == 0)
    {
        if(ui->lineEdit_ParamenterValue->text().isEmpty())
        {
            QMessageBox::information(NULL,"警告",tr("常量值为空!"));
            return;
        }
    }
    if(ui->lineEdit_tableName->text().isEmpty())
    {
        QMessageBox::information(NULL,"警告",tr("字段表名为空!"));
        return;
    }
    if(ui->lineEdit_FieldName->text().isEmpty())
    {
        QMessageBox::information(NULL,"警告",tr("字段名称为空!"));
        return;
    }
    this->accept();
}

void ViewFilterFieldDialog::on_pbt_Cancel_clicked()
{
    this->reject();
}
