#include "MDDataArchivesNotifyParam.h"
#include "DataArchivesNotifyParam.h"
#include "IMDDataItem.h"

CMDDataArchivesNotifyParam::CMDDataArchivesNotifyParam()
{
    m_pNotifyParam = new CDataArchivesNotifyParam();
}

CMDDataArchivesNotifyParam::CMDDataArchivesNotifyParam(const CMDDataArchivesNotifyParam &otehrObj)
{
    m_pNotifyParam = new CDataArchivesNotifyParam();
    *this->m_pNotifyParam = *otehrObj.m_pNotifyParam;
}

CMDDataArchivesNotifyParam &CMDDataArchivesNotifyParam::operator=(const CMDDataArchivesNotifyParam &otherObj)
{
    if (this == &otherObj)
    {
        return *this;
    }

    if (m_pNotifyParam)
    {
        delete m_pNotifyParam;
        m_pNotifyParam = NULL;
    }

    m_pNotifyParam = new CDataArchivesNotifyParam();
    *m_pNotifyParam = *otherObj.m_pNotifyParam;

    return *this;
}

CMDDataArchivesNotifyParam::~CMDDataArchivesNotifyParam()
{
    if (m_pNotifyParam)
    {
        delete m_pNotifyParam;
        m_pNotifyParam = NULL;
    }
}

QString CMDDataArchivesNotifyParam::dataGroupName()
{
    if (m_pNotifyParam)
    {
        return m_pNotifyParam->dataGroupName();
    }

    return QString();
}

void CMDDataArchivesNotifyParam::setDataGroupName(const QString &name)
{
    if (m_pNotifyParam)
    {
        m_pNotifyParam->setDataGroupName(name);
    }
}

void CMDDataArchivesNotifyParam::setRecordDateTime(QDateTime datetime)
{
    if (m_pNotifyParam)
    {
        m_pNotifyParam->setRecordDateTime(datetime);
    }
}

QDateTime CMDDataArchivesNotifyParam::recordDateTime()
{
    if (m_pNotifyParam)
    {
        return m_pNotifyParam->recordDateTime();
    }

    return QDateTime();
}

void CMDDataArchivesNotifyParam::setRecordMillisecond(int recordMS)
{
    if (m_pNotifyParam)
    {
        m_pNotifyParam->setRecordMillisecond(recordMS);
    }
}

int CMDDataArchivesNotifyParam::recordMillisecond()
{
    if (m_pNotifyParam)
    {
        return m_pNotifyParam->recordMillisecond();
    }

    return 0;
}

void CMDDataArchivesNotifyParam::setSql(QString sql)
{
    if (m_pNotifyParam)
    {
        m_pNotifyParam->setSql(sql);
    }
}

QString CMDDataArchivesNotifyParam::sql()
{
    if (m_pNotifyParam)
    {
        return m_pNotifyParam->sql();
    }

    return "";
}

bool CMDDataArchivesNotifyParam::addItem(IMDDataItem *item)
{
    bool success = false;
    if (m_pNotifyParam)
    {
        success = m_pNotifyParam->addItem(item);
    }

    return success;
}

bool CMDDataArchivesNotifyParam::removeItem(IMDDataItem *item)
{
    bool success = false;
    if (m_pNotifyParam)
    {
        success = m_pNotifyParam->removeItem(item);
    }

    return success;
}

void CMDDataArchivesNotifyParam::removeAllItem()
{
    if (m_pNotifyParam)
    {
        m_pNotifyParam->removeAllItem();
    }
}

IMDDataItem *CMDDataArchivesNotifyParam::getItem(const QString &name)
{
    IMDDataItem *pDataItem = NULL;
    if (m_pNotifyParam)
    {
        pDataItem = m_pNotifyParam->getItem(name);
    }

    return pDataItem;
}

IMDDataItem *CMDDataArchivesNotifyParam::getItem(int index)
{
    IMDDataItem *pDataItem = NULL;
    if (m_pNotifyParam)
    {
        pDataItem = m_pNotifyParam->getItem(index);
    }

    return pDataItem;
}

int CMDDataArchivesNotifyParam::itemCount()
{
    int count = 0;
    if (m_pNotifyParam)
    {
        count = m_pNotifyParam->itemCount();
    }

    return count;
}
