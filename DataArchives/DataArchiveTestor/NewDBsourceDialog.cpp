#include "NewDBsourceDialog.h"
#include "ui_newdbsourcedialog.h"
#include "MDDataSourceFactory.h"
#include "IMDDataGroupManager.h"
#include "IMDDataSourceManager.h"
#include "IMDDataTable.h"
#include "IMDDataArchives.h"
#include "IMDDataGroup.h"
#include "IMDDataItem.h"
#include "IMDDataField.h"
#include "IMDDataSource.h"
#include "MDDataDefine.h"
#include "IMDDataArchives.h"

CNewDBsourceDialog::CNewDBsourceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDBsourceDialog)
{
    ui->setupUi(this);

    m_pMDDataSrc      = NULL; //! 数据源
    m_pDataArchives   = NULL; //! 数据归档

    ui->m_pCheckShow->setChecked(true);
    ui->m_pCheck->setChecked(true);
//    connect(ui->m_pCheck, SIGNAL(stateChanged(int)), this, SLOT(onCheckStateChange(int)));
}

CNewDBsourceDialog::~CNewDBsourceDialog()
{
    if (ui)
    {
        delete ui;
        ui = NULL;
    }
}

void CNewDBsourceDialog::initUI(IMDDataSource *pDataSource, IMDDataArchives *pDataArchives)
{
    m_pMDDataSrc = pDataSource;
    m_pDataArchives = pDataArchives;
}

void CNewDBsourceDialog::changeLanguage()
{
}

void CNewDBsourceDialog::DefaultSetting()
{
    if (NULL == m_pMDDataSrc)
    {
        return;
    }

    m_pMDDataSrc->removeAllDataTable();
    // 报警表
    Alm_DefaultSetting();
    // 日志表
    Log_DefaultSetting();
    // 历史表
    His_DefaultSetting();
}

void CNewDBsourceDialog::Alm_DefaultSetting()
{
    if (NULL == m_pMDDataSrc || NULL == m_pDataArchives)
    {
        return;
    }

    IMDDataGroup *pAlmGrp = m_pDataArchives->dataGroupManager()->getAlarmDataGroup();
    if (NULL == pAlmGrp)
    {
        return;
    }

    createTable(pAlmGrp);
}

void CNewDBsourceDialog::Log_DefaultSetting()
{
    if (NULL == m_pMDDataSrc || NULL == m_pDataArchives)
    {
        return;
    }

    IMDDataGroup *pLogGrp = m_pDataArchives->dataGroupManager()->getLogDataGroup();
    if (NULL == pLogGrp)
    {
        return;
    }

    createTable(pLogGrp);
}

void CNewDBsourceDialog::His_DefaultSetting()
{
    if (NULL == m_pMDDataSrc || NULL == m_pDataArchives)
    {
        return;
    }

    IMDDataGroupManager *pDataGrpMana = m_pDataArchives->dataGroupManager();
    if (NULL == pDataGrpMana)
    {
        return;
    }

    IMDDataGroup *pHisGrp = NULL;
    int count = pDataGrpMana->hisDataGroupCount();
    for (int i = 0; i < count; ++i)
    {
        pHisGrp = pDataGrpMana->getHisDataGroup(i);
        if (NULL == pHisGrp)
        {
            continue;
        }

        createTable(pHisGrp);
    }
}

int CNewDBsourceDialog::calCurrTypeSize(MDDataType type)
{
    int uSize = 0;
    switch(type)
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

    return uSize;
}

void CNewDBsourceDialog::createTable(IMDDataGroup *pDataGrp)
{
    if (NULL == m_pMDDataSrc || NULL == m_pDataArchives)
    {
        return;
    }

    if (NULL == pDataGrp)
    {
        return;
    }

    QString strTableName = pDataGrp->name();
    IMDDataTable *pTableAlm = CMDDataSourceFactory::createDataTable();
    pTableAlm->setDataGroupName(strTableName);
    pTableAlm->setName(strTableName);
    pTableAlm->setDescription(pDataGrp->description());

    // 删除原有字段
    pTableAlm->removeAllDataField();
    int count = pDataGrp->itemCount();
//    QString strName = "";
    IMDDataItem *pItem = NULL;
    IMDDataField *pField = NULL;
    for(int i = 0; i < count; i++)
    {
        pItem = pDataGrp->getItem(i);
        if (NULL == pItem)
        {
            continue;
        }

        pField = createField(pItem->description(), pItem->name(), pItem->dataType());
        if (NULL == pField)
        {
            continue;
        }

        pTableAlm->addDataField(pField);
    }

    m_pMDDataSrc->addDataTable(pTableAlm);
}

IMDDataField *CNewDBsourceDialog::createField(const QString &description, const QString &name, MDDataType type)
{
    IMDDataField *pField = NULL;
    pField = CMDDataSourceFactory::createDataField();
    if (NULL == pField)
    {
        return pField;
    }

    int uSize = calCurrTypeSize(type);
    QString strTmpName = name;
    strTmpName.replace(".", "_");
    pField->setName(strTmpName);
    pField->setDataItemName(name);
    pField->setDataType(type);
    pField->setDataItemDataType(type);
    pField->setPrimarykey(false);
    pField->enableNull(true);
    pField->setSize(uSize);
    if (description.isEmpty())
    {
        pField->setDescription(pField->name());
    }
    else
    {
        pField->setDescription(description);
    }

    return pField;
}

void CNewDBsourceDialog::on_OkButton_clicked()
{
    if ((NULL == m_pDataArchives->dataSourceManager()) || (NULL == m_pMDDataSrc))
    {
        return;
    }

    QString strName  = ui->lineEdit->text(); //数据源名称
    if (strName.isEmpty())
    {
        QMessageBox message(this);
        message.addButton(tr("确认"), QMessageBox::YesRole);
        message.setWindowTitle(tr("提示"));
        message.setText(tr("名称不能为空"));
        message.exec();

        ui->lineEdit->setFocus();
        return;
    }

    if(m_pDataArchives->dataSourceManager()->getDataSource(strName))
    {
        QMessageBox message(this);
        message.addButton(tr("确认"), QMessageBox::YesRole);
        message.setWindowTitle(tr("提示"));
        QString strFront = tr("对象");
        QString strBack = tr("已存在！");
        message.setText(strFront + strName + strBack);
        message.exec();

        ui->lineEdit->setFocus();
        return ;
    }

    m_pMDDataSrc->setCreateDefaultTable(ui->m_pCheck->isChecked());
    if(m_pMDDataSrc->createDefaultTable())
    {// 如果是默认设置，自动增加项目中的表
        DefaultSetting();
    }

    m_pMDDataSrc->setShowTable(ui->m_pCheckShow->isChecked());
    m_pMDDataSrc->setName(strName);
//    m_pDataArchives->dataSourceManager()->addDataSource(m_pMDDataSrc);

    QDialog::accept();

}

void CNewDBsourceDialog::on_CancelButton_clicked()
{
    QDialog::reject();
}

void CNewDBsourceDialog::onCheckStateChange(int state)
{
    if (NULL == m_pMDDataSrc)
    {
        return;
    }

    switch (state)
    {
    case Qt::Unchecked:
        m_pMDDataSrc->setCreateDefaultTable(false);
        break;
    case Qt::Checked:
        m_pMDDataSrc->setCreateDefaultTable(true);
        break;
    default:
        break;
    }
}
