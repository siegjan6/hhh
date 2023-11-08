#include "DataSelectUI.h"

#include "IMDDataArchives.h"
#include "IMDDataSource.h"
#include "IMDDataTable.h"
#include "IMDDataSourceManager.h"

#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>

CDataSelectUI::CDataSelectUI(QWidget *parent)
    : QWidget(parent),
      m_pDataArchives(NULL),
      m_pLbDataSrc(NULL),
      m_pLbDataTab(NULL),
      m_pComDataSrc(NULL),
      m_pComDataTab(NULL),
      m_pBtnOk(NULL),
      m_pBtnExit(NULL),
      m_pMainLayout(NULL)
{
    initUI();
    connect(m_pComDataSrc, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_comDataSrc_indexChange(const QString &)));
    connect(m_pComDataTab, SIGNAL(currentTextChanged(const QString &)), this, SIGNAL(currentTableChange(const QString &)));
    connect(m_pComDataSrc, SIGNAL(currentTextChanged(const QString &)), this, SIGNAL(currentSourceChange(const QString &)));
//    connect(m_pBtnOk, SIGNAL(clicked()), this, SLOT(accept()));
//    connect(m_pBtnExit, SIGNAL(clicked()), this, SLOT(close()));
}

CDataSelectUI::~CDataSelectUI()
{
    if (m_pMainLayout)
    {
        m_pMainLayout->removeWidget(m_pLbDataSrc);
        m_pMainLayout->removeWidget(m_pComDataSrc);
        m_pMainLayout->removeWidget(m_pLbDataTab);
        m_pMainLayout->removeWidget(m_pComDataTab);
    }

    if (m_pMainLayout)
    {
        QLayoutItem *pItem = NULL;
        while (NULL != (pItem = m_pMainLayout->takeAt(0)))
        {
            delete pItem;
            pItem = NULL;
        }

        delete m_pMainLayout;
        m_pMainLayout = NULL;
    }

    if (m_pBtnExit)
    {
        delete m_pBtnExit;
        m_pBtnExit = NULL;
    }

    if (m_pBtnOk)
    {
        delete m_pBtnOk;
        m_pBtnOk = NULL;
    }

    if (m_pComDataTab)
    {
        delete m_pComDataTab;
        m_pComDataTab = NULL;
    }

    if (m_pComDataSrc)
    {
        delete m_pComDataSrc;
        m_pComDataSrc = NULL;
    }

    if (m_pLbDataTab)
    {
        delete m_pLbDataTab;
        m_pLbDataTab = NULL;
    }

    if (m_pLbDataSrc)
    {
        delete m_pLbDataSrc;
        m_pLbDataSrc = NULL;
    }
}

void CDataSelectUI::setDataArchives(IMDDataArchives *pDataArchives)
{
    if (NULL == pDataArchives)
    {
        return;
    }

    m_pDataArchives = pDataArchives;
    IMDDataSource *pDataSrc = NULL;
    m_pComDataSrc->clear();
    for (int i = 0; i < m_pDataArchives->dataSourceManager()->dataSourceCount(); ++i)
    {
        pDataSrc = m_pDataArchives->dataSourceManager()->getDataSource(i);
        if (NULL == pDataSrc)
        {
            continue;
        }

        m_pComDataSrc->addItem(pDataSrc->name());
    }

    m_pComDataSrc->setCurrentIndex(0);
    on_comDataSrc_indexChange(m_pComDataSrc->currentText());
}

void CDataSelectUI::setComboxText(const QString &strSrc, const QString &strTab)
{
    for (int i = 0; m_pComDataSrc->count(); ++i)
    {
        if (strSrc == m_pComDataSrc->itemText(i))
        {
            m_pComDataSrc->setCurrentIndex(i);
            break;
        }
    }

    for (int j = 0; m_pComDataTab->count(); ++j)
    {
        if (strTab == m_pComDataTab->itemText(j))
        {
            m_pComDataTab->setCurrentIndex(j);
            break;
        }
    }
}

QString CDataSelectUI::dataSourceName()
{
    return m_pComDataSrc->currentText();
}

QString CDataSelectUI::dataTableName()
{
    return m_pComDataTab->currentText();
}

IMDDataSource *CDataSelectUI::dataSource()
{
    if (NULL == m_pDataArchives)
    {
        return NULL;
    }

    QString strDataSrcName = m_pComDataSrc->currentText();
    return m_pDataArchives->dataSourceManager()->getDataSource(strDataSrcName);
}

IMDDataTable *CDataSelectUI::dataTable()
{
    if (NULL == m_pDataArchives)
    {
        return NULL;
    }

    QString strDataSrcName = m_pComDataSrc->currentText();
    QString strDataTabName = m_pComDataTab->currentText();

    IMDDataSource *pDataSrc = NULL;
    pDataSrc = m_pDataArchives->dataSourceManager()->getDataSource(strDataSrcName);

    if (NULL == pDataSrc)
    {
        return NULL;
    }

    return pDataSrc->getDataTable(strDataTabName);
}

void CDataSelectUI::initUI()
{
    m_pLbDataSrc = new QLabel(tr("数据源"), this);
    m_pLbDataTab = new QLabel(tr("数据表"), this);

    m_pComDataSrc = new QComboBox(this);
    m_pComDataTab = new QComboBox(this);

//    m_pBtnOk = new QPushButton(tr("确定"), this);
//    m_pBtnExit = new QPushButton(tr("取消"), this);

    m_pMainLayout = new QGridLayout;
    m_pMainLayout->addWidget(m_pLbDataSrc, 0, 0);
    m_pMainLayout->addWidget(m_pComDataSrc, 0, 1);

    m_pMainLayout->addWidget(m_pLbDataTab, 1, 0);
    m_pMainLayout->addWidget(m_pComDataTab, 1, 1);

//    m_pBottomLayout = new QHBoxLayout;
//    m_pBottomLayout->addStretch(8);
//    m_pBottomLayout->addWidget(m_pBtnOk);
//    m_pBottomLayout->addSpacing(20);
//    m_pBottomLayout->addWidget(m_pBtnExit);

//    m_pMainLayout->setContentsMargins(20, 20, 20, 20);
//    m_pMainLayout->addLayout(m_pBottomLayout);
    setLayout(m_pMainLayout);
}

void CDataSelectUI::on_comDataSrc_indexChange(const QString &text)
{
    if (NULL == m_pDataArchives)
    {
        return;
    }

    IMDDataSource *pDataSrc = NULL;
    IMDDataTable *pTable = NULL;

    pDataSrc = m_pDataArchives->dataSourceManager()->getDataSource(text);
    if (NULL == pDataSrc)
    {
        return;
    }

    m_pComDataTab->clear();
    for (int j = 0; j < pDataSrc->dataTableCount(); ++j)
    {
        pTable = pDataSrc->getDataTable(j);
        if (NULL == pTable)
        {
            continue;
        }

        m_pComDataTab->addItem(pTable->name());
    }
}
