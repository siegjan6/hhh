#include "DataArchivesNotifyParam.h"
#include "IMDDataItem.h"

CDataArchivesNotifyParam::CDataArchivesNotifyParam()
    : m_iRecordMS(0),
      m_sql(QString()),
      m_dataGrpName(QString()),
      m_recDatetime(QDateTime())
{
    m_itemList.clear();
}

CDataArchivesNotifyParam::~CDataArchivesNotifyParam()
{
    removeAllItem();
}

void CDataArchivesNotifyParam::operator =(const CDataArchivesNotifyParam &otherObj)
{
    m_iRecordMS   = otherObj.m_iRecordMS;
    m_sql   = otherObj.m_sql;
    m_dataGrpName = otherObj.m_dataGrpName;
    m_recDatetime = otherObj.m_recDatetime;
    removeAllItem();

    int count = otherObj.m_itemList.count();
    IMDDataItem *pDataItem = NULL;
    for (int i = 0; i < count; ++i)
    {
        pDataItem = otherObj.m_itemList.at(i);
        if (NULL == pDataItem)
        {
            continue;
        }

        m_itemList.append(pDataItem->clone());
    }
}

bool CDataArchivesNotifyParam::addItem(IMDDataItem *item)
{
    bool success = false;
    if (item)
    {
        m_itemList.append(item);
        success = true;
    }

    return success;
}

bool CDataArchivesNotifyParam::removeItem(IMDDataItem *item)
{
    bool success = false;
    if (item)
    {
        int index = m_itemList.indexOf(item);
        if (-1 != index)
        {
            m_itemList.removeAt(index);
            delete item;
            item = NULL;
            success = true;
        }
    }

    return success;
}

void CDataArchivesNotifyParam::removeAllItem()
{
    if (!m_itemList.isEmpty())
    {
        qDeleteAll(m_itemList.begin(), m_itemList.end());
        m_itemList.clear();
    }
}

IMDDataItem *CDataArchivesNotifyParam::getItem(const QString &name)
{
    IMDDataItem *pDataItem = NULL;
    for (int i = 0; i < m_itemList.size(); ++i)
    {
        pDataItem = m_itemList.at(i);
        if (pDataItem)
        {
            if (pDataItem->name() == name)
            {
                return pDataItem;
            }
        }
    }

    return NULL;
}

IMDDataItem *CDataArchivesNotifyParam::getItem(int index)
{
    IMDDataItem *pDataItem = NULL;

    if (0 <= index && index < m_itemList.size())
    {
        pDataItem = m_itemList.at(index);
    }

    return pDataItem;
}
