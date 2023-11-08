#include "DbTableFieldDialog.h"
#include "ui_dbtablefielddialog.h"
#include "MDDataArchivesInterface/IMDDataField.h"
#include "IMDDataGroup.h"
#include "IMDDataItem.h"

CDbTableFieldDialog::CDbTableFieldDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbTableFieldDialog)
{
    ui->setupUi(this);
    m_pField      = NULL; //! 字段
    m_pDataGroup  = NULL; //! 数据组

    initLanguage();
   //connect(ui->ResFieldCombox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(resField_combox_change(QString)));
}

CDbTableFieldDialog::~CDbTableFieldDialog()
{
    if (ui)
    {
        delete ui;
        ui = NULL;
    }
}

void CDbTableFieldDialog::initUI(IMDDataField *pField, IMDDataGroup *pGrp)
{
    if (NULL == pField || NULL == pGrp)
    {
        return;
    }

    IMDDataItem *pItem = NULL;
    int index = 0;
    QString strItem = "";   

    m_pField = pField;
    m_pDataGroup = pGrp;
    for (int i = 0; i < pGrp->itemCount(); ++i)
    {
        pItem = pGrp->getItem(i);
        if (NULL == pItem)
        {
            continue;
        }

        strItem = pItem->name();
        if (strItem == pField->dataItemName())
        {
            index = i;
        }

        //ui->ResFieldCombox->addItem(strItem);
    }

    ui->ObjectTypeEdit->setText(dataTypeToString(pField->dataItemDataType()));
    ui->FieldTypeComboBox->setCurrentIndex((int)pField->dataType());
    //ui->ResFieldCombox->setCurrentIndex(index);
    ui->MasterKeyCheckBox->setChecked(pField->isPrimarykey());    
    ui->DesFieldLlineEdit->setText(pField->name());
    ui->CommentEdit->setText(pField->description());

    if (!pField->name().isEmpty())
    {
        //ui->ResFieldCombox->setDisabled(true);
        ui->FieldTypeComboBox->setDisabled(true);
        ui->ObjectTypeEdit->setReadOnly(true);
        ui->EnableNullCheckBox->setChecked(pField->isEnableNull());
    }
    else
    {        
        //resField_combox_change(ui->ResFieldCombox->currentText());
        ui->EnableNullCheckBox->setChecked(true);
    }
}

void CDbTableFieldDialog::changeLanguage()
{
    initLanguage();
}

QString CDbTableFieldDialog::dataTypeToString(MDDataType type)
{
    QString strType = "";
    switch(type)
    {
    case MDDataType::Bool:
    case MDDataType::SByte:
    case MDDataType::Byte:
    case MDDataType::Int16:
    case MDDataType::UInt16:
    case MDDataType::Int32:
    case MDDataType::UInt32:
    case MDDataType::Int64:
    case MDDataType::UInt64:
        strType = QString("Int");
        break;
    case MDDataType::Float:
        strType = QString("Float");
        break;
    case MDDataType::Double:
        strType = QString("Double");
        break;
    case MDDataType::DateTime:
        strType = QString("DateTime");
        break;
    default:
        strType = QString("String");
        break;
    }

    return strType;
}

void CDbTableFieldDialog::initLanguage()
{

}

void CDbTableFieldDialog::on_OkButton_clicked()
{
    if (m_pField)
    {
        m_pField->setDataItemName(ui->groupFieldLlineEdit->text());
        m_pField->setName(ui->DesFieldLlineEdit->text());
        m_pField->setDataType((MDDataType)ui->FieldTypeComboBox->currentIndex());
        m_pField->setDataItemDataType((MDDataType)ui->FieldTypeComboBox->currentIndex());
        m_pField->setPrimarykey(ui->MasterKeyCheckBox->isChecked());
        m_pField->enableNull(ui->EnableNullCheckBox->isChecked());
        int uSize = 0;
        switch((MDDataType)ui->FieldTypeComboBox->currentIndex())
        {
        case MDDataType::Bool:
        case MDDataType::Byte:
        case MDDataType::SByte:
        case MDDataType::Int16:
        case MDDataType::UInt16:
        case MDDataType::Int32:
        case MDDataType::UInt32:
            uSize = 4;
            break;
        case MDDataType::Float:
        case MDDataType::Int64:
        case MDDataType::UInt64:
        case MDDataType::Double:
        case MDDataType::DateTime:
            uSize = 8;
            break;
        case MDDataType::String:
            uSize = 250;
            break;
        default:
            uSize = 250;
            break;
        }
        m_pField->setSize(uSize);
        m_pField->setDescription(ui->CommentEdit->text().isEmpty() ? ui->DesFieldLlineEdit->text():ui->CommentEdit->text());
    }

    QDialog::accept();
}

void CDbTableFieldDialog::on_CancelButton_clicked()
{
    QDialog::reject();
}

void CDbTableFieldDialog::resField_combox_change(const QString &text)
{
    /*
    QString strName = text;
    strName.replace(".", "_");
    ui->DesFieldLlineEdit->setText(strName);
    if (m_pDataGroup)
    {
        if (m_pDataGroup->getItem(text))
        {
            MDDataType type = m_pDataGroup->getItem(text)->dataType();
            ui->FieldTypeComboBox->setCurrentIndex((int)(type));
            ui->ObjectTypeEdit->setText(dataTypeToString(type));
        }
    }
    */
}
