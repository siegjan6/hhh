#include "MDDataSelectUI.h"
#include "DataSelectUI.h"

CMDDataSelectUI::CMDDataSelectUI()
{
    m_pDataSelUI = new CDataSelectUI;
    connect(m_pDataSelUI, SIGNAL(currentTableChange(const QString &)), this, SIGNAL(currentTableNameChange(const QString &)));
    connect(m_pDataSelUI, SIGNAL(currentSourceChange(const QString &)), this, SIGNAL(currentSourceNameChange(const QString &)));
}

CMDDataSelectUI::~CMDDataSelectUI()
{
    if (m_pDataSelUI)
    {
        delete m_pDataSelUI;
        m_pDataSelUI = NULL;
    }
}

QWidget *CMDDataSelectUI::dataSelectUI()
{
    return m_pDataSelUI;
}

void CMDDataSelectUI::setDataArchives(IMDDataArchives *pDataArchives)
{
    if (m_pDataSelUI)
    {
        m_pDataSelUI->setDataArchives(pDataArchives);
    }
}

void CMDDataSelectUI::setComboxText(const QString &strSrc, const QString &strTab)
{
    if (m_pDataSelUI)
    {
        m_pDataSelUI->setComboxText(strSrc, strTab);
    }
}

QString CMDDataSelectUI::dataSourceName()
{
    if (m_pDataSelUI)
    {
        return m_pDataSelUI->dataSourceName();
    }

    return QString();
}

QString CMDDataSelectUI::dataTableName()
{
    if (m_pDataSelUI)
    {
        return m_pDataSelUI->dataTableName();
    }

    return QString();
}

IMDDataSource *CMDDataSelectUI::dataSource()
{
    if (m_pDataSelUI)
    {
        return m_pDataSelUI->dataSource();
    }

    return NULL;
}

IMDDataTable *CMDDataSelectUI::dataTable()
{
    if (m_pDataSelUI)
    {
        return m_pDataSelUI->dataTable();
    }

    return NULL;
}

