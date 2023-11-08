#include "DbTableFieldDialog.h"
#include "ui_dbtablefielddialog.h"
#include "MDDataArchivesInterface/IMDDataField.h"
#include "IMDDataGroup.h"
#include "IMDDataItem.h"
#include "MDMultiLanguage.h"

CDbTableFieldDialog::CDbTableFieldDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DbTableFieldDialog)
{
    ui->setupUi(this);
    m_pField      = NULL; //! 字段
    m_pDataGroup  = NULL; //! 数据组
    m_addOrModifyItem = 0;
    m_pMulLnaguageTool = NULL; //! 多语言翻译工具

    initLanguage();
    connect(ui->ResFieldCombox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(resField_combox_change(QString)));
}

CDbTableFieldDialog::~CDbTableFieldDialog()
{
    if (ui)
    {
        delete ui;
        ui = NULL;
    }

    if (m_pMulLnaguageTool)
    {
        delete m_pMulLnaguageTool;
        m_pMulLnaguageTool = NULL;
    }
}

void CDbTableFieldDialog::setEditFlag(int addOrModifyItem/* = 0*/)
{
    m_addOrModifyItem = addOrModifyItem;
}

void CDbTableFieldDialog::initUI(IMDDataField *pField, IMDDataGroup *pGrp)
{
    if (NULL == pField || NULL == pGrp)
    {
        return;
    }

    if(m_addOrModifyItem == 0)
        ui->SyncButton->setVisible(false);

    IMDDataItem *pItem = NULL;
    int index = 0;
    QString strItem = "";   

    m_pField = pField;
    m_pDataGroup = pGrp;
    ui->ResFieldCombox->addItem("");
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
            index = i+1;
        }

        ui->ResFieldCombox->addItem(strItem);
    }

    ui->ObjectTypeEdit->setText(dataTypeToString(pField->dataItemDataType()));
    ui->FieldTypeComboBox->setCurrentIndex((int)pField->dataType());
    ui->ResFieldCombox->setCurrentIndex(index);
    ui->MasterKeyCheckBox->setChecked(pField->isPrimarykey());    
    ui->DesFieldLlineEdit->setText(pField->name());
    ui->CommentEdit->setText(pField->description());
    ui->cb_rtCurve->setChecked(pField->isRtCurveEnable());
    ui->cb_hisCurve->setChecked(pField->isHisCurveEnable());
    ui->cb_hisTable->setChecked(pField->isHisTableEnable());
    ui->cb_hisFilter->setChecked(pField->isHisFilterEnable());
    ui->spinBox_order->setValue(pField->order());
    ui->spinBox_lwidth->setValue(pField->lineWidth());
    ui->ColorEdit->setText(pField->color());
    ui->extEdit->setText(pField->ext());

    if (!pField->name().isEmpty())
    {
        //ui->ResFieldCombox->setDisabled(true);
        ui->FieldTypeComboBox->setDisabled(true);
        ui->ObjectTypeEdit->setReadOnly(true);
        ui->EnableNullCheckBox->setChecked(pField->isEnableNull());
    }
    else
    {
        resField_combox_change(ui->ResFieldCombox->currentText());
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
    if (NULL == m_pMulLnaguageTool)
    {
        m_pMulLnaguageTool = CMDMultiLanguage::createWidgetTool();
        m_pMulLnaguageTool->init(this);
    }
    m_pMulLnaguageTool->changeLanguage();
}

void CDbTableFieldDialog::on_OkButton_clicked()
{
    if (m_pField)
    {
        m_pField->setDataItemName(ui->ResFieldCombox->currentText());
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

        m_pField->enableRtCurve(ui->cb_rtCurve->isChecked());
        m_pField->enableHisCurve(ui->cb_hisCurve->isChecked());
        m_pField->enableHisTable(ui->cb_hisTable->isChecked());
        m_pField->enableHisFilter(ui->cb_hisFilter->isChecked());
        m_pField->setOrder(ui->spinBox_order->value());
        m_pField->setlineWidth(ui->spinBox_lwidth->value());
        m_pField->setColor(ui->ColorEdit->text());
        m_pField->setExt(ui->extEdit->toPlainText());
    }

    QDialog::accept();
}

void CDbTableFieldDialog::on_CancelButton_clicked()
{
    QDialog::reject();
}

void CDbTableFieldDialog::resField_combox_change(const QString &text)
{
    QString strName = text;
    strName.replace(".", "_");
    if(m_addOrModifyItem == 0) {
        ui->DesFieldLlineEdit->setText(strName);
    }
    if (m_pDataGroup)
    {
        if (m_pDataGroup->getItem(text))
        {
            MDDataType type = m_pDataGroup->getItem(text)->dataType();
            ui->FieldTypeComboBox->setCurrentIndex((int)(type));
            ui->ObjectTypeEdit->setText(dataTypeToString(type));
            if(m_addOrModifyItem == 0) {
                ui->CommentEdit->setText(m_pDataGroup->getItem(text)->description());
            }
        }
    }
}

void CDbTableFieldDialog::on_SyncButton_clicked()
{
    QString strName = ui->ResFieldCombox->currentText();
    if (m_pDataGroup)
    {
        if (m_pDataGroup->getItem(strName))
        {
            MDDataType type = m_pDataGroup->getItem(strName)->dataType();
            ui->FieldTypeComboBox->setCurrentIndex((int)(type));
            ui->ObjectTypeEdit->setText(dataTypeToString(type));
            ui->CommentEdit->setText(m_pDataGroup->getItem(strName)->description());

        }
    }
}
