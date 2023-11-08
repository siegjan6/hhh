#include "NewViewVarDialog.h"
#include "ui_NewViewVarDialog.h"
#include "ViewVarEnumValueDialog.h"
#include "SourceTreeDialog.h"
#include "NewFieldDialog.h"

NewViewVarDialog::NewViewVarDialog(int dialogType, QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewViewVarDialog),
    m_windowTitle(title),
    m_ViewVar(new DBQ::ViewVariable),
    m_dialogType(dialogType),
    m_DataArchivesFilePath("")
{
    ui->setupUi(this);
    initUI();
    setConstAndFunctionTabOrder();
}

NewViewVarDialog::~NewViewVarDialog()
{
    delete ui;
}

DBQ::ViewVariable *NewViewVarDialog::getViewVar() const
{
    return m_ViewVar;
}

void NewViewVarDialog::setOriginalViewVarParameter(DBQ::ViewVariable *viewVar)
{
    ui->lineEdit_VarName->setText(viewVar->name);
    ui->lineEdit_Description->setText(viewVar->description);
    ui->comboBox_VarType->setCurrentIndex(viewVar->type-1);
    ui->comboBox_VarValueType->setCurrentIndex(viewVar->valueType-1);
    switch (viewVar->type)
    {
    case DBQ::VIEW_VARIABLE_TYPE_CONST:
        ui->lineEdit_Const_Value->setText(viewVar->value);
        break;
    case DBQ::VIEW_VARIABLE_TYPE_ENUM:
        if(m_dialogType == 1)
        {
            m_modifyEnumValueList.clear();
            for(int i = 0; i < viewVar->evs.count(); i++)
            {
                if(viewVar->evs.at(i).desc.isEmpty())
                {
                    ui->comboBox_enumValue->addItem(viewVar->evs.at(i).value);
                }
                else
                {
                    ui->comboBox_enumValue->addItem(viewVar->evs.at(i).value+","+viewVar->evs.at(i).desc);
                }
                m_modifyEnumValueList.push_back(viewVar->evs.at(i));
            }
        }
        break;
    case DBQ::VIEW_VARIABLE_TYPE_FIELD:
        ui->lineEdit_Field_Value->setText(viewVar->value);
        break;
    default:
        m_ViewVar->value = "";
        break;
    }
}

void NewViewVarDialog::setViewVarParameter()
{
    m_ViewVar->evs.clear();
    m_ViewVar->name = ui->lineEdit_VarName->text();
    m_ViewVar->description = ui->lineEdit_Description->text();
    m_ViewVar->type = getCurrentViewVarType();
    QString typeStr = ui->comboBox_VarValueType->currentText();
    m_ViewVar->valueType = DBQ::View_GetFieldTypeFromString(typeStr);
    switch (getCurrentViewVarType())
    {
    case DBQ::VIEW_VARIABLE_TYPE_CONST:
        m_ViewVar->value = ui->lineEdit_Const_Value->text();
        break;
    case DBQ::VIEW_VARIABLE_TYPE_ENUM:
    {
        if(m_dialogType == 0)
        {
            foreach (DBQ::ViewVariableEnumValue pEnumValue, m_newEnumValueList)
            {
                m_ViewVar->evs.push_back(pEnumValue);
            }
        }
        if(m_dialogType == 1)
        {
            foreach (DBQ::ViewVariableEnumValue pEnumValue, m_modifyEnumValueList)
            {
                m_ViewVar->evs.push_back(pEnumValue);
            }
        }
        ui->comboBox_enumValue->clear();
        int size = m_ViewVar->evs.size();
        for(int i = 0; i < size; i++)
        {
            if(m_ViewVar->evs.at(i).desc.isEmpty())
            {
                ui->comboBox_enumValue->addItem(m_ViewVar->evs.at(i).value);
            }
            else
            {
                ui->comboBox_enumValue->addItem(m_ViewVar->evs.at(i).value+","+m_ViewVar->evs.at(i).desc);
            }
        }
    }
        break;
    case DBQ::VIEW_VARIABLE_TYPE_FIELD:
        m_ViewVar->value = ui->lineEdit_Field_Value->text();
        break;
    default :
        m_ViewVar->value = "";
        break;
    }
}

void NewViewVarDialog::initUI()
{
    this->setWindowTitle(m_windowTitle);
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEdit_Field_Value->setReadOnly(true);
    ui->comboBox_enumValue->clear();
    connect(ui->comboBox_VarType,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_changeStackWidget(int)));
}

DBQ::VIEW_VARIABLE_TYPE NewViewVarDialog::getCurrentViewVarType()
{
    DBQ::VIEW_VARIABLE_TYPE type;
    switch (ui->comboBox_VarType->currentIndex()+1)
    {
    case 1:
        type = DBQ::VIEW_VARIABLE_TYPE_CONST;
        break;
    case 2:
        type = DBQ::VIEW_VARIABLE_TYPE_ENUM;
        break;
    case 3:
        type = DBQ::VIEW_VARIABLE_TYPE_FIELD;
        break;
    case 4:
        type = DBQ::VIEW_VARIABLE_TYPE_FUNC;
        break;
    default:
        type = DBQ::VIEW_VARIABLE_TYPE_UNKNOWN;
        break;
    }
    return type;
}

void NewViewVarDialog::setConstAndFunctionTabOrder()
{
    QWidget::setTabOrder(ui->lineEdit_VarName,ui->lineEdit_Description);
    QWidget::setTabOrder(ui->lineEdit_Description, ui->comboBox_VarType);
    QWidget::setTabOrder(ui->comboBox_VarType, ui->comboBox_VarValueType);
    QWidget::setTabOrder(ui->comboBox_VarValueType, ui->lineEdit_Const_Value);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->lineEdit_VarName);
}

void NewViewVarDialog::setEnumTabOrder()
{
    QWidget::setTabOrder(ui->lineEdit_VarName,ui->lineEdit_Description);
    QWidget::setTabOrder(ui->lineEdit_Description, ui->comboBox_VarType);
    QWidget::setTabOrder(ui->comboBox_VarType, ui->comboBox_VarValueType);
    QWidget::setTabOrder(ui->comboBox_VarValueType, ui->comboBox_enumValue);
    QWidget::setTabOrder(ui->pbt_Add_EnumVar, ui->pbt_Confirm);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->lineEdit_VarName);
}

void NewViewVarDialog::setFieldTabOrder()
{
    QWidget::setTabOrder(ui->lineEdit_VarName,ui->lineEdit_Description);
    QWidget::setTabOrder(ui->lineEdit_Description, ui->comboBox_VarType);
    QWidget::setTabOrder(ui->comboBox_VarType, ui->comboBox_VarValueType);
    QWidget::setTabOrder(ui->comboBox_VarValueType, ui->lineEdit_Field_Value);
    QWidget::setTabOrder(ui->lineEdit_Field_Value, ui->pbt_Add_DataArchiveField);
    QWidget::setTabOrder(ui->pbt_Add_DataArchiveField, ui->pbt_Add_NewField);
    QWidget::setTabOrder(ui->pbt_Add_NewField, ui->pbt_Confirm);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->lineEdit_VarName);
}

void NewViewVarDialog::slot_changeStackWidget(int index)
{
    switch (index+1)
    {
    case 1:
        ui->stackedWidget->setCurrentIndex(0);
        setConstAndFunctionTabOrder();
        break;
    case 2:
        ui->stackedWidget->setCurrentIndex(1);
        setEnumTabOrder();
        break;
    case 3:
        ui->stackedWidget->setCurrentIndex(2);
        setFieldTabOrder();
        break;
    default:
        ui->stackedWidget->setCurrentIndex(0);
        setConstAndFunctionTabOrder();
        break;
    }
}

void NewViewVarDialog::on_pbt_Add_EnumVar_clicked()
{
    ViewVarEnumValueDialog dlg;
    dlg.setEnumValueType((ui->comboBox_VarValueType->currentIndex()));
    dlg.setEnumValueList(m_modifyEnumValueList);
    if(dlg.exec() == QDialog::Accepted)
    {
        if(m_dialogType == 0)
        {
            ui->comboBox_enumValue->clear();
            m_newEnumValueList.clear();
            foreach (DBQ::ViewVariableEnumValue pEnumValue, dlg.getEnumValueList())
            {
                m_newEnumValueList.push_back(pEnumValue);
            }
            int size = m_newEnumValueList.size();
            for(int i = 0; i < size; i++)
            {
                if(m_newEnumValueList.at(i).desc.isEmpty())
                {
                    ui->comboBox_enumValue->addItem(m_newEnumValueList.at(i).value);
                }
                else
                {
                    ui->comboBox_enumValue->addItem(m_newEnumValueList.at(i).value+","+m_newEnumValueList.at(i).desc);
                }
            }
        }
        if(m_dialogType == 1)
        {
            ui->comboBox_enumValue->clear();
            m_modifyEnumValueList.clear();
            foreach (DBQ::ViewVariableEnumValue pEnumValue, dlg.getEnumValueList())
            {
                m_modifyEnumValueList.push_back(pEnumValue);
            }
            int size = m_modifyEnumValueList.size();
            for(int i = 0; i < size; i++)
            {
                if(m_modifyEnumValueList.at(i).desc.isEmpty())
                {
                    ui->comboBox_enumValue->addItem(m_modifyEnumValueList.at(i).value);
                }
                else
                {
                    ui->comboBox_enumValue->addItem(m_modifyEnumValueList.at(i).value+","+m_modifyEnumValueList.at(i).desc);
                }
            }
        }
    }
}

void NewViewVarDialog::on_pbt_Add_DataArchiveField_clicked()
{
    SourceTreeDialog STDialog(QString(tr("视图数据源字段")),m_DataArchivesFilePath);
    QStringList dbsourceList;
    if(DBQ::DBSource_GetNames(m_DBQueryPath,dbsourceList))
    {
        STDialog.setDbsourceNameList(dbsourceList);
        STDialog.initDataSourceTree();
        STDialog.setReturnMode(1);
        STDialog.setStackWidgetShowMode(1);
        if(STDialog.exec() == QDialog::Accepted)
        {
            if(STDialog.getViewField())
            {
                QString value = STDialog.getViewField()->tableName+"."+STDialog.getViewField()->fieldName;
                ui->lineEdit_Field_Value->setText(value);
            }
        }
    }
}

void NewViewVarDialog::on_pbt_Add_NewField_clicked()
{
    NewFieldDialog dlg(QString(tr("新建字段")));
    if(dlg.exec() == QDialog::Accepted)
    {
        if(dlg.getField())
        {
            QString value = dlg.getField()->tableName+"."+dlg.getField()->fieldName;
            ui->lineEdit_Field_Value->setText(value);
        }
    }
}

void NewViewVarDialog::on_pbt_Confirm_clicked()
{
    setViewVarParameter();
    this->accept();
}

void NewViewVarDialog::on_pbt_Cancel_clicked()
{
    this->reject();
}
QString NewViewVarDialog::DBQueryPath() const
{
    return m_DBQueryPath;
}

void NewViewVarDialog::setDBQueryPath(const QString &DBQueryPath)
{
    m_DBQueryPath = DBQueryPath;
}

QString NewViewVarDialog::DataArchivesFilePath() const
{
    return m_DataArchivesFilePath;
}

void NewViewVarDialog::setDataArchivesFilePath(const QString &DataArchivesFilePath)
{
    m_DataArchivesFilePath = DataArchivesFilePath;
}

