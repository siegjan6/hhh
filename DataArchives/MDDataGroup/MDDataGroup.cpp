#include "MDDataGroup.h"
#include "MDDataDefine.h"
#include "MDDataItem.h"
#include <QJsonObject>
#include <QJsonArray>

CMDDataGroup::CMDDataGroup()
{
    m_name                  = "";       //! 名称
    m_description           = "";       //! 描述
    m_iEnable               = false;    //! 启用
    m_iVersion              = 1;        //! 版本号

    m_itemList.clear();
}

CMDDataGroup::~CMDDataGroup()
{
    removeAllItem();
}

/*!
\section 克隆
*/
IMDDataGroup *CMDDataGroup::clone()
{
    CMDDataGroup *pDataGrp = new CMDDataGroup();
    *pDataGrp = *this;
    return pDataGrp;
}

/*!
\section 赋值运算符
*/
void CMDDataGroup::operator =(const CMDDataGroup &otherObj)
{
    m_name          = otherObj.m_name;
    m_description   = otherObj.m_description;
    m_iEnable       = otherObj.m_iEnable;
    m_iVersion      = otherObj.m_iVersion;
    m_itemList.clear();
    removeAllItem();
    int iCount = otherObj.m_itemList.count();
    for(int i = 0; i < iCount; ++i)
    {
        IMDDataItem *pDataItem = otherObj.m_itemList.at(i);
        if(pDataItem)
        {
            addItem(pDataItem->clone());
        }
    }
}

/*!
\section 序列化
*/
void CMDDataGroup::serialize(QDataStream *ar, bool bSave)
{
    if(ar)
    {
        if(bSave)
        {
            int iVersion = 1;
            *ar << iVersion;
            m_iVersion = iVersion;

            if(iVersion >= 1)
            {
                *ar << m_name;
                *ar << m_description;
                *ar << m_iEnable;
                *ar << m_iVersion;
                int iCount = m_itemList.size();
                *ar << iCount;
                for (int i = 0; i < m_itemList.size(); ++i)
                {
                    m_itemList.at(i)->serialize(ar, true);
                }
            }
        }
        else
        {
            int iVersion = 0;
            *ar >> iVersion;
            m_iVersion = iVersion;

            if(iVersion >= 1)
            {
                *ar >> m_name;
                *ar >> m_description;
                *ar >> m_iEnable;
                *ar >> m_iVersion;
                int iCount = 0;
                *ar >> iCount;
                IMDDataItem *pItem = NULL;
                for (int i = 0; i < iCount; ++i)
                {
                    pItem = new CMDDataItem();
                    if (NULL == pItem)
                    {
                        continue;
                    }

                    pItem->serialize(ar, false);
                    if (NULL != getItem(pItem->name()))
                    {
                        delete pItem;
                        pItem = NULL;
                        continue;
                    }

                    m_itemList.append(pItem);
                }
            }
        }
    }
}

void CMDDataGroup::serialize(QJsonObject& json)
{
    json.insert("Name", m_name);
    json.insert("Desc", m_description);
    json.insert("Enable", m_iEnable);
    QJsonArray arr;
    int iCount = m_itemList.size();
    for (int i = 0; i < iCount; ++i)
    {
        QJsonObject obj;
        m_itemList.at(i)->serialize(obj);
        arr.append(obj);
    }
    json.insert("FieldArray", arr);
}

void CMDDataGroup::deserialize(QJsonObject json)
{
    m_name = json.value("Name").toString();
    m_description = json.value("Desc").toString();
    m_iEnable = json.value("Enable").toBool();
    QJsonArray arr = json.value("FieldArray").toArray();
    int iCount = arr.count();
    for (int i = 0; i < iCount; ++i)
    {
        IMDDataItem* item = new CMDDataItem();
        item->deserialize(arr.at(i).toObject());
        if (NULL != getItem(item->name()))
        {
            delete item;
            item = NULL;
            continue;
        }
        m_itemList.append(item);
    }

}


/*!
\section 增加item
*/
bool CMDDataGroup::addItem(IMDDataItem *item)
{
    bool success = false;
    if (item && (NULL == getItem(item->name())))
    {
        m_itemList.append(item);
        success = true;
    }

    return success;
}

/*!
\section 删除item
*/
bool CMDDataGroup::removeItem(IMDDataItem *item)
{
    bool success = false;
    if (item)
    {
//        int index = m_itemList.indexOf(item);
//        if (-1 != index)
//        {
//            m_itemList.removeAt(index);
//            success = true;
//        }
        for (int i = 0; i < itemCount(); ++i)
        {
            if (item == m_itemList.at(i))
            {
                m_itemList.removeAt(i);
                delete item;
                item = NULL;
                success = true;
                return success;
            }
        }
    }

    return success;
}

/*!
\section 删除所有item
*/
void CMDDataGroup::removeAllItem()
{
    if (0 < m_itemList.size())
    {
        qDeleteAll(m_itemList.begin(), m_itemList.end());
        m_itemList.clear();
    }
}

/*!
\section 查找item--通过item名称
*/
IMDDataItem *CMDDataGroup::getItem(const QString &name)
{
    IMDDataItem *pDataItem = NULL;
    for (int i = 0; i < m_itemList.size(); ++i)
    {
        pDataItem = m_itemList.at(i);
        if (pDataItem && pDataItem->name() == name)
        {
            return pDataItem;
        }
    }

    return NULL;
}

/*!
\section 查找item--通过索引
*/
IMDDataItem *CMDDataGroup::getItem(int index)
{
    IMDDataItem *pDataItem = NULL;
    if (0 <= index && index < m_itemList.size())
    {
        pDataItem = m_itemList.at(index);
    }

    return pDataItem;
}

/*!
\section 创建报警默认项
*/
void CMDDataGroup::createDefaultAlarmItem()
{   
    addDefaultItem(ALM_ID, QObject::tr("ID"), MDDataType::String);
    addDefaultItem(ALM_AID, QObject::tr("AID"), MDDataType::String);
    addDefaultItem(ALM_NAME, QObject::tr("名称"), MDDataType::String);
    addDefaultItem(ALM_MESSAGE, QObject::tr("消息"), MDDataType::String);
    addDefaultItem(ALM_LEVEL, QObject::tr("报警类别"), MDDataType::String);
    addDefaultItem(ALM_SERVERRITY, QObject::tr("报警级别"), MDDataType::Int32);
    addDefaultItem(ALM_STARTVALUE, QObject::tr("发生值"), MDDataType::String);
    addDefaultItem(ALM_ENDVALUE, QObject::tr("结束值"), MDDataType::String);
    addDefaultItem(ALM_STARTTIME, QObject::tr("发生时间"), MDDataType::DateTime);
    addDefaultItem(ALM_STARTTIME_MS, QObject::tr("发生时间毫秒"), MDDataType::Int32);
    addDefaultItem(ALM_ENDTIME, QObject::tr("结束时间"), MDDataType::DateTime);
    addDefaultItem(ALM_ENDTIME_MS, QObject::tr("结束时间毫秒"), MDDataType::Int32);
    addDefaultItem(ALM_ACKNOWLEDGETIME, QObject::tr("知悉时间"), MDDataType::DateTime);
    addDefaultItem(ALM_ACKNOWLEDGETIME_MS, QObject::tr("知悉时间毫秒"), MDDataType::Int32);
    addDefaultItem(ALM_ACKNOWLEDGESTATE, QObject::tr("知悉状态"), MDDataType::Int32);
    addDefaultItem(ALM_CONFIRMTIME, QObject::tr("确认时间"), MDDataType::DateTime);
    addDefaultItem(ALM_CONFIRMTIME_MS, QObject::tr("确认时间毫秒"), MDDataType::Int32);
    addDefaultItem(ALM_CONFIRMSTATE, QObject::tr("确认状态"), MDDataType::Int32);
    addDefaultItem(ALM_SUPPRESSEDORSHELVED, QObject::tr("抑制或者搁置状态"), MDDataType::Int32);
    addDefaultItem(ALM_SUPPRESSTIME, QObject::tr("抑制时间"), MDDataType::DateTime);
    addDefaultItem(ALM_SUPPRESSTIME_MS, QObject::tr("抑制时间毫秒"), MDDataType::Int32);
    addDefaultItem(ALM_SUPPRESSSTATE, QObject::tr("抑制状态"), MDDataType::Int32);
    addDefaultItem(ALM_SHELVETIME, QObject::tr("搁置时间"), MDDataType::DateTime);
    addDefaultItem(ALM_SHELVETIME_MS, QObject::tr("搁置时间毫秒"), MDDataType::Int32);
    addDefaultItem(ALM_SHELVESTATE, QObject::tr("搁置状态"), MDDataType::Int32);
    addDefaultItem(ALM_STATUS, QObject::tr("状态"), MDDataType::Int32);
    addDefaultItem(ALM_USER, QObject::tr("当前用户"), MDDataType::String);
    addDefaultItem(ALM_COMMENT, QObject::tr("备注"), MDDataType::String);
    addDefaultItem(ALM_FORECOLOR, QObject::tr("前景色"), MDDataType::Double);
    addDefaultItem(ALM_BACKCOLOR, QObject::tr("背景色"), MDDataType::Double);
}

/*!
\section 创建日志默认项
*/
void CMDDataGroup::createDefaultLogItem()
{
    addDefaultItem(LOG_RECORDTIME, QObject::tr("记录时间"), MDDataType::DateTime);
    addDefaultItem(LOG_RECORDTIME_MS, QObject::tr("记录时间毫秒"), MDDataType::Int32);
    addDefaultItem(LOG_USER, QObject::tr("当前用户"), MDDataType::String);
    addDefaultItem(LOG_MESSAGE, QObject::tr("消息"), MDDataType::String);
    addDefaultItem(LOG_TYPE, QObject::tr("对象类型"), MDDataType::Int32);
    addDefaultItem(LOG_NAME, QObject::tr("对象名称"), MDDataType::String);
    addDefaultItem(LOG_PROTECTMODE, QObject::tr("保护模式"), MDDataType::Int32);
    addDefaultItem(LOG_OPERATER, QObject::tr("操作者"), MDDataType::String);
    addDefaultItem(LOG_OPERATETIME, QObject::tr("操作时间"), MDDataType::DateTime);
    addDefaultItem(LOG_VERIFIER, QObject::tr("校验者"), MDDataType::String);
    addDefaultItem(LOG_VERIFYTIME, QObject::tr("校验时间"), MDDataType::DateTime);
    addDefaultItem(LOG_OPERATECOMMENT, QObject::tr("操作说明"), MDDataType::String);
    addDefaultItem(LOG_VERIFYCOMMENT, QObject::tr("校验说明"), MDDataType::String);
}

/*!
\section 配置数据项
*/
void CMDDataGroup::addDefaultItem(const QString &itemName, const QString &descript, MDDataType dataType)
{
    IMDDataItem *item = new CMDDataItem();
    item->setDataType(dataType);
    item->setName(itemName);
    item->setDescription(descript);
    addItem(item);
}
