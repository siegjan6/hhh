#include "MDFilterTreePlugin.h"
#include "FilterTreePlugin.h"

CMDFilterTreePlugin::CMDFilterTreePlugin()
{
    m_pFilterTree = new CFilterTreePlugin;
}

CMDFilterTreePlugin::~CMDFilterTreePlugin()
{
    if (m_pFilterTree)
    {
        delete m_pFilterTree;
        m_pFilterTree = NULL;
    }
}

QDialog *CMDFilterTreePlugin::widget()
{
    return m_pFilterTree;
}

bool CMDFilterTreePlugin::load(const QString &condition)
{
    if (m_pFilterTree)
    {
        return m_pFilterTree->load(condition);
    }

    return false;
}


bool CMDFilterTreePlugin::load(CMDFilterTreeData *pFilterData)
{
    if (m_pFilterTree)
    {
        return m_pFilterTree->load(pFilterData);
    }

    return false;
}

bool CMDFilterTreePlugin::save(CMDFilterTreeData *pFilterData)
{
    if (m_pFilterTree)
    {
        return m_pFilterTree->save(pFilterData);
    }

    return false;
}

void CMDFilterTreePlugin::initData(IMDDataTable *pTable)
{
    if (m_pFilterTree)
    {
        m_pFilterTree->initData(pTable);
    }
}

void CMDFilterTreePlugin::initData(const QString &tableName, QList<CMDFilterTreeDataItem *> &customFieldList)
{
    if (m_pFilterTree)
    {
        m_pFilterTree->initData(tableName, customFieldList);
    }
}

QString CMDFilterTreePlugin::getFilterCondition()
{
    if (m_pFilterTree)
    {
        return m_pFilterTree->getFilterCondition();
    }

    return QString();
}


CMDFilterTreeData::CMDFilterTreeData()
{
    m_itemList.clear();
    m_strName = "";
    m_strTableName = "";
    m_strCondition = "";
    m_iType   = 0;
    m_iPosition = 1;
    m_iCount = 0;
}

CMDFilterTreeData::~CMDFilterTreeData()
{
    if (!m_itemList.isEmpty())
    {
        qDeleteAll(m_itemList.begin(), m_itemList.end());
    }
}

void CMDFilterTreeData::serialize(QDataStream *ar, bool bSave)
{
    if(NULL == ar)
    {
        return;
    }

    if(bSave)
    {
        qint32 iVersion = 1;
        *ar << iVersion;
        *ar << m_strTableName;
        *ar << m_strName;
        *ar << m_iType;
        *ar << m_iCount;
        *ar << m_strCondition;

        CMDFilterTreeDataItem *pItem = NULL;
        for (int i = 0; i < m_itemList.size(); ++i)
        {
            pItem = m_itemList.at(i);
            if (NULL == pItem)
            {
                continue;
            }

            pItem->serialize(ar, true);
        }
    }
    else
    {
//        removeAllItem();
        qint32 iVersion = 1;
        *ar >> iVersion;
        if (1 == iVersion)
        {
            *ar >> m_strTableName;
            *ar >> m_strName;
            *ar >> m_iType;
            *ar >> m_iCount;
            *ar >> m_strCondition;

            CMDFilterTreeDataItem *pItem = NULL;
            for (int i = 0; i < m_iCount; ++i)
            {
                pItem = new CMDFilterTreeDataItem();
                if (NULL == pItem)
                {
                    continue;
                }

                pItem->serialize(ar, false);
                m_itemList.append(pItem);
            }
        }
    }
}

CMDFilterTreeData *CMDFilterTreeData::clone()
{
    CMDFilterTreeData *pFilterData = new CMDFilterTreeData();
    *pFilterData = *this;
    return pFilterData;
}

void CMDFilterTreeData::operator=(const CMDFilterTreeData &otherObj)
{
    m_itemList.clear();
    for (int i = 0; i < otherObj.m_iCount; ++i)
    {
        CMDFilterTreeDataItem *pItem = otherObj.m_itemList.at(i);
        if (NULL == pItem)
        {
            continue;
        }

        m_itemList.append(pItem->clone());
    }

    m_strName = otherObj.m_strName;
    m_strTableName = otherObj.m_strTableName;
    m_strCondition = otherObj.m_strCondition;
    m_iType = otherObj.m_iType;
    m_iPosition = otherObj.m_iPosition;
    m_iCount = otherObj.m_iCount;
}

CMDFilterTreeDataItem *CMDFilterTreeData::getItem(int index)
{
    return m_itemList.at(index);
}

void CMDFilterTreeData::insert(CMDFilterTreeDataItem *pItem)
{
    pItem->setID(m_iCount);
    ++m_iCount;
    m_itemList.append(pItem);
}

void CMDFilterTreeData::insert(CMDFilterTreeData *pData)
{
    removeAllItem();
    for (int i = 0; i < pData->m_itemList.size(); ++i)
    {
        insert(pData->m_itemList.at(i));
    }
}

void CMDFilterTreeData::removeAllItem()
{
    if (m_itemList.isEmpty())
    {
        return;
    }

    m_iCount = 0;
    qDeleteAll(m_itemList.begin(), m_itemList.end());
    m_itemList.clear();
}

QList<CMDFilterTreeDataItem *> CMDFilterTreeData::dynamicInputList()
{
    QList<CMDFilterTreeDataItem *> items;
    items.clear();

    CMDFilterTreeDataItem *pItem = NULL;
    for (int i = 0; i < m_itemList.size(); ++i)
    {
        pItem = m_itemList.at(i);
        if (NULL == pItem)
        {
            continue;
        }

        if (CMDFilterTreeDataItem::Filter_Dynamic != pItem->getType())
        {
            continue;
        }

        items.append(pItem);
    }

    return items;
}

QString CMDFilterTreeData::updateCondition(QList<CMDFilterTreeDataItem *> items)
{
    int index = 0;
    int offset = 0;
    int size = items.size();
    QString strReplace = "[value]";
    QString strCondition = m_strCondition;
    while (-1 != (index = strCondition.indexOf(strReplace)))
    {
        CMDFilterTreeDataItem *pItem = NULL;
        pItem = items.at(offset);
        if (NULL == pItem)
        {
            continue;
        }

        QVariant::Type varType = (QVariant::Type)pItem->getDataType();
        switch (varType)
        {
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::Double:
            strCondition.replace(index, strReplace.size(), pItem->getValue());
            break;
        default://DateTime String
            strCondition.replace(index, strReplace.size(), "\'" + pItem->getValue() + "\'");
            break;
        }

        if (++offset >= size)
        {
            break;
        }
    }

    return strCondition;
}


CMDFilterTreeDataManager::CMDFilterTreeDataManager()
{
    m_pDataStream = NULL;
    m_FilterDataManaList.clear();
}

CMDFilterTreeDataManager::~CMDFilterTreeDataManager()
{
    removeAllData();
}

CMDFilterTreeDataManager *CMDFilterTreeDataManager::clone()
{
    CMDFilterTreeDataManager *pMana = new CMDFilterTreeDataManager();
    *pMana = *this;
    return pMana;
}

void CMDFilterTreeDataManager::operator=(const CMDFilterTreeDataManager &obj)
{
    for (int i = 0; i < obj.m_FilterDataManaList.size(); ++i)
    {
        m_FilterDataManaList.append(obj.m_FilterDataManaList.at(i)->clone());
    }

    m_pDataStream = obj.m_pDataStream;
}

void CMDFilterTreeDataManager::load(QDataStream *ar)
{
    if (NULL == ar)
    {
        return;
    }

    m_pDataStream = ar;
    CMDFilterTreeData *pData = new CMDFilterTreeData();
    pData->serialize(ar, false);
    if (pData->getCondition().isEmpty())
    {
        delete pData;
        pData = NULL;
        return;
    }

    m_FilterDataManaList.append(pData);
}

void CMDFilterTreeDataManager::save(QDataStream *ar)
{
    CMDFilterTreeData *pData = NULL;
    for (int i = 0; i < m_FilterDataManaList.size(); ++i)
    {
        pData = m_FilterDataManaList.at(i);
        if (NULL == pData)
        {
            continue;
        }

        pData->serialize(ar, true);
    }
}

void CMDFilterTreeDataManager::save()
{
    if (NULL == m_pDataStream)
    {
        return;
    }

    save(m_pDataStream);
}

CMDFilterTreeData *CMDFilterTreeDataManager::getFilterData(int index)
{
    if (index >=0 && index < m_FilterDataManaList.size())
    {
        return m_FilterDataManaList.at(index);
    }

    return NULL;
}

CMDFilterTreeData *CMDFilterTreeDataManager::getFilterData(const QString &name)
{
    CMDFilterTreeData *pData = NULL;
    for (int i = 0; i < m_FilterDataManaList.size(); ++i)
    {
        pData = m_FilterDataManaList.at(i);
        if (NULL == pData)
        {
            continue;
        }

        if (pData->getName().isEmpty())
        {
            continue;
        }

        if (name != pData->getName())
        {
            continue;
        }

        return pData;
    }

    return NULL;
}

void CMDFilterTreeDataManager::removeAllData()
{
    if (!m_FilterDataManaList.isEmpty())
    {
        qDeleteAll(m_FilterDataManaList.begin(), m_FilterDataManaList.end());
        m_FilterDataManaList.clear();
    }
}

void CMDFilterTreeDataManager::removeData(int index)
{
    if (index >=0 && index < m_FilterDataManaList.size())
    {
        CMDFilterTreeData *pData = m_FilterDataManaList.at(index);
        m_FilterDataManaList.removeAt(index);
        delete pData;
        pData = NULL;
    }
}

void CMDFilterTreeDataManager::removeData(const QString &name)
{
    CMDFilterTreeData *pData = NULL;
    for (int i = 0; i < m_FilterDataManaList.size(); ++i)
    {
        pData = m_FilterDataManaList.at(i);
        if (NULL == pData)
        {
            continue;
        }

        if (name != pData->getName())
        {
            continue;
        }

        m_FilterDataManaList.removeAt(i);

        delete pData;
        pData = NULL;
    }
}

QString CMDFilterTreeDataManager::updateCondition(const QString &name, QList<CMDFilterTreeDataItem *> items)
{
    CMDFilterTreeData *pData = NULL;
    for (int i = 0; i < m_FilterDataManaList.size(); ++i)
    {
        pData = m_FilterDataManaList.at(i);
        if (NULL == pData)
        {
            continue;
        }

        if (name != pData->getName())
        {
            continue;
        }

        return pData->updateCondition(items);
    }

    return QString();
}

QList<CMDFilterTreeDataItem *> CMDFilterTreeDataManager::dynamicInputList(const QString &name)
{
    CMDFilterTreeData *pData = NULL;
    for (int i = 0; i < m_FilterDataManaList.size(); ++i)
    {
        pData = m_FilterDataManaList.at(i);
        if (NULL == pData)
        {
            continue;
        }

        if (name != pData->getName())
        {
            continue;
        }

        return pData->dynamicInputList();
    }

    QList<CMDFilterTreeDataItem *> items;
    items.clear();
    return items;
}


CMDFilterTreeDataItem::CMDFilterTreeDataItem()
{
    m_iType   = CMDFilterTreeDataItem::Filter_Invalid;
    m_iCount  = 0;
    m_iDataType = 0;
    m_strName = "";
    m_strDescripe = "";
    m_strHint = "";
    m_strOperator = "";
    m_strValue = "";
    m_ID = 0;
}

void CMDFilterTreeDataItem::serialize(QDataStream *ar, bool bSave)
{
    if(NULL == ar)
    {
        return;
    }

    if(bSave)
    {
        qint32 iVersion = 1;
        *ar << iVersion;
        *ar << (int)m_iType;
        *ar << m_iCount;
        *ar << m_iDataType;
        *ar << m_strName;
        *ar << m_strDescripe;
        *ar << m_strHint;
        *ar << m_strOperator;
        *ar << m_strValue;
        *ar << m_ID;
    }
    else
    {
        qint32 iVersion;
        *ar >> iVersion;
        if (1 == iVersion)
        {
            qint32 type = 0;
            *ar >> type;
            m_iType = (CMDFilterTreeDataItem::CFilterType)type;
            *ar >> m_iCount;
            *ar >> m_iDataType;
            *ar >> m_strName;
            *ar >> m_strDescripe;
            *ar >> m_strHint;
            *ar >> m_strOperator;
            *ar >> m_strValue;
            *ar >> m_ID;
        }
    }
}

CMDFilterTreeDataItem *CMDFilterTreeDataItem::clone()
{
    CMDFilterTreeDataItem *pItem = new CMDFilterTreeDataItem();
    *pItem = *this;
    return pItem;
}

void CMDFilterTreeDataItem::operator=(const CMDFilterTreeDataItem &otherObj)
{
    m_iType = otherObj.m_iType;
    m_iCount = otherObj.m_iCount;
    m_iDataType = otherObj.m_iDataType;
    m_strName = otherObj.m_strName;
    m_strDescripe = otherObj.m_strDescripe;
    m_strHint = otherObj.m_strHint;
    m_strOperator = otherObj.m_strOperator;
    m_strValue = otherObj.m_strValue;
    m_ID = otherObj.m_ID;
}
