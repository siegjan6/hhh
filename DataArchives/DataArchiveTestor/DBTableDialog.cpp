#include "DBTableDialog.h"
#include "IMDDataGroupManager.h"
#include "IMDDataArchives.h"
#include "IMDDataGroup.h"
#include "IMDDataTable.h"
#include "IMDDataSource.h"
#include "MDDataSourceFactory.h"
#include "IMDDataField.h"
#include "IMDDataItem.h"

#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

CDBTableDialog::CDBTableDialog(QWidget *parent) :
    QDialog(parent)
{
    initUI();
    initLanguage();
    resize(240, 180);
}

CDBTableDialog::~CDBTableDialog()
{
    if (m_pHFirstLayout)
    {
        m_pHFirstLayout->removeWidget(m_pLbGrpName);
        m_pHFirstLayout->removeWidget(m_pCombDataGrp);
    }

    if (m_pHSecondLayout)
    {
        m_pHSecondLayout->removeWidget(m_pLbTabName);
        m_pHSecondLayout->removeWidget(m_pLineTabName);
    }

    if (m_pHThirdLayout)
    {
        m_pHThirdLayout->removeWidget(m_pLbDescri);
        m_pHThirdLayout->removeWidget(m_pLineDescri);
    }

    if (m_pHFourthLayout)
    {
        m_pHFourthLayout->removeWidget(m_pBtnOk);
        m_pHFourthLayout->removeWidget(m_pBtnCancel);
    }

    if (m_pVMainLayout)
    {
        QLayoutItem *child = NULL;
        while(child = m_pVMainLayout->takeAt(0))
        {
            delete child;
            child = NULL;
        }
        delete m_pVMainLayout;
        m_pVMainLayout = NULL;
    }

    if (m_pBtnOk)
    {
        delete m_pBtnOk;
        m_pBtnOk = NULL;
    }

    if (m_pBtnCancel)
    {
        delete m_pBtnCancel;
        m_pBtnCancel = NULL;
    }

    if (m_pLbGrpName)
    {
        delete m_pLbGrpName;
        m_pLbGrpName = NULL;
    }

    if (m_pLbTabName)
    {
        delete m_pLbTabName;
        m_pLbTabName = NULL;
    }

    if (m_pLbDescri)
    {
        delete m_pLbDescri;
        m_pLbDescri = NULL;
    }

    if (m_pCombDataGrp)
    {
        delete m_pCombDataGrp;
        m_pCombDataGrp = NULL;
    }

    if (m_pLineTabName)
    {
        delete m_pLineTabName;
        m_pLineTabName = NULL;
    }

    if (m_pLineDescri)
    {
        delete m_pLineDescri;
        m_pLineDescri = NULL;
    }

}

int CDBTableDialog::calculateLabelWidth(QLabel *pLb)
{
    QFontMetrics met(pLb->font());
    return met.width(pLb->text());
}

void CDBTableDialog::initUI()
{
    m_pBtnOk = new QPushButton(tr("确定"), this);;
    m_pBtnOk->setAttribute(Qt::WA_DeleteOnClose);
    connect(m_pBtnOk, SIGNAL(clicked()), this, SLOT(on_OkButton_clicked()));

    m_pBtnCancel = new QPushButton(tr("取消"), this);;
    m_pBtnCancel->setAttribute(Qt::WA_DeleteOnClose);
    connect(m_pBtnCancel, SIGNAL(clicked()), this, SLOT(on_CancelButton_clicked()));

    m_pLbGrpName = new QLabel(tr("数据组名"), this);
    m_pLbGrpName->setAttribute(Qt::WA_DeleteOnClose);

    m_pLbTabName = new QLabel(tr("表名"), this);
    m_pLbTabName->setAttribute(Qt::WA_DeleteOnClose);

    m_pLbDescri = new QLabel(tr("描述"), this);
    m_pLbDescri->setAttribute(Qt::WA_DeleteOnClose);

    m_pCombDataGrp = new QComboBox(this);
    m_pCombDataGrp->setAttribute(Qt::WA_DeleteOnClose);
    m_pCombDataGrp->setFixedWidth(TABLE_FIXED_W);

    m_pLineTabName = new QLineEdit(this);
    m_pLineTabName->setAttribute(Qt::WA_DeleteOnClose);
    m_pLineTabName->setFixedWidth(TABLE_FIXED_W);

    m_pLineDescri = new QLineEdit(this);
    m_pLineDescri->setAttribute(Qt::WA_DeleteOnClose);
    m_pLineDescri->setFixedWidth(TABLE_FIXED_W);

    m_pHFirstLayout = new QHBoxLayout;
    m_pHSecondLayout = new QHBoxLayout;
    m_pHThirdLayout = new QHBoxLayout;
    m_pHFourthLayout = new QHBoxLayout;
    m_pVMainLayout = new QVBoxLayout;

    int maxWidth = calculateLabelWidth(m_pLbGrpName);
    m_pHFirstLayout->addWidget(m_pLbGrpName);
    m_pHFirstLayout->addSpacing(maxWidth - calculateLabelWidth(m_pLbGrpName) + TABLE_LAYOUT_SPACE);
    m_pHFirstLayout->addWidget(m_pCombDataGrp);

    m_pHSecondLayout->addWidget(m_pLbTabName);
    m_pHSecondLayout->addSpacing(maxWidth - calculateLabelWidth(m_pLbTabName) + TABLE_LAYOUT_SPACE);
    m_pHSecondLayout->addWidget(m_pLineTabName);

    m_pHThirdLayout->addWidget(m_pLbDescri);
    m_pHThirdLayout->addSpacing(maxWidth - calculateLabelWidth(m_pLbDescri) + TABLE_LAYOUT_SPACE);
    m_pHThirdLayout->addWidget(m_pLineDescri);

    m_pHFourthLayout->addStretch(0);
    m_pHFourthLayout->addWidget(m_pBtnOk);
    m_pHFourthLayout->addSpacing(maxWidth - calculateLabelWidth(m_pLbDescri) + TABLE_LAYOUT_SPACE);
    m_pHFourthLayout->addWidget(m_pBtnCancel);

    m_pVMainLayout->addLayout(m_pHFirstLayout);
    m_pVMainLayout->addLayout(m_pHSecondLayout);
    m_pVMainLayout->addLayout(m_pHThirdLayout);
    m_pVMainLayout->addLayout(m_pHFourthLayout);
    m_pVMainLayout->setContentsMargins(TABLE_LAYOUT_SPACE, 0, TABLE_LAYOUT_SPACE, 0);

    setLayout(m_pVMainLayout);
}

void CDBTableDialog::initLanguage()
{
}

void CDBTableDialog::initData(IMDDataSource *pDBSource, IMDDataTable *pTable, IMDDataGroupManager *pGrpManager)
{
    if (NULL == pTable || NULL == pDBSource)
    {
        return;
    }

    m_pDBSource = pDBSource;
    m_pDataTable = pTable;
    initDataGrpNameList(pGrpManager);
}

void CDBTableDialog::changeLanguage()
{
    initLanguage();
}

void CDBTableDialog::initDataGrpNameList(IMDDataGroupManager *pGrpManager)
{
    if (NULL == pGrpManager)
    {
        return;
    }

    m_pDataGrpManager = pGrpManager;
    m_pCombDataGrp->addItem(pGrpManager->getAlarmDataGroup()->name());
    m_pCombDataGrp->addItem(pGrpManager->getLogDataGroup()->name());

    IMDDataGroup *pHisGrp = NULL;
    int count = pGrpManager->hisDataGroupCount();
    for (int i = 0; i < count; ++i)
    {
        pHisGrp = pGrpManager->getHisDataGroup(i);
        if (NULL == pHisGrp)
        {
            continue;
        }

        m_pCombDataGrp->addItem(pHisGrp->name());
    }
}

int CDBTableDialog::calCurrTypeSize(MDDataType type)
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

IMDDataField *CDBTableDialog::createField(const QString &description, const QString &name, MDDataType type)
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

void CDBTableDialog::on_OkButton_clicked()
{
    if (NULL == m_pDataTable || NULL == m_pDBSource || NULL == m_pDataGrpManager)
    {
        return;
    }

    QMessageBox message(this);
    message.addButton(tr("确认"), QMessageBox::YesRole);
    if (m_pLineTabName->text().isEmpty())
    {
        message.setWindowTitle(tr("提示"));
        message.setText(tr("表名不能为空！"));
        message.exec();
        return;
    }

    if (NULL != (m_pDBSource->getDataTable(m_pLineTabName->text())))
    {
        message.setWindowTitle(tr("提示"));
        QString strFront = tr("表 ");
        QString strName = "\'" + m_pLineTabName->text() + "\'";
        QString strBack = tr("已存在！");
        message.setText(strFront + strName + strBack);
        message.exec();
        return;
    }

    QString strGrpName = m_pCombDataGrp->currentText();
    m_pDataTable->setDataGroupName(strGrpName);
    m_pDataTable->setName(m_pLineTabName->text());
    m_pDataTable->setDescription(m_pLineDescri->text());

    IMDDataGroup *pDataGrp = m_pDataGrpManager->getDataGroup(strGrpName);
    if (NULL != pDataGrp)
    {
        IMDDataItem *pItem = NULL;
        IMDDataField *pField = NULL;
        QString strName = "";
        for (int i = 0; i < pDataGrp->itemCount(); ++i)
        {
            pItem = pDataGrp->getItem(i);
            if (NULL == pItem)
            {
                continue;
            }

            strName = pItem->name();

            pField = createField(pItem->description(), strName, pItem->dataType());
            if (NULL == pField)
            {
                continue;
            }

            m_pDataTable->addDataField(pField);
        }
    }

    QDialog::accept();
}

void CDBTableDialog::on_CancelButton_clicked()
{
    close();
}

void CDBTableDialog::initData()
{
    m_pBtnOk         = NULL;  //! 确定按钮
    m_pBtnCancel     = NULL;  //! 取消按钮
    m_pLbGrpName     = NULL;  //! 数据组名标签
    m_pLbTabName     = NULL;  //! 数据表标签
    m_pLbDescri      = NULL;  //! 描述标签
    m_pCombDataGrp   = NULL;  //! 数据组下拉框
    m_pLineTabName   = NULL;  //! 数据表名编辑框
    m_pLineDescri    = NULL;  //! 描述编辑框

    m_pHFirstLayout      = NULL;  //! 数据组水平布局
    m_pHSecondLayout     = NULL;  //! 数据表名水平布局
    m_pHThirdLayout      = NULL;  //! 描述水平布局
    m_pHFourthLayout     = NULL;  //! 操作按钮水平布局
    m_pVMainLayout       = NULL;  //! 整体垂直布局

    m_pDataTable       = NULL;  //! 数据表
    m_pDBSource        = NULL;  //! 数据源
    m_pDataGrpManager  = NULL;  //! 数据组管理器
}
