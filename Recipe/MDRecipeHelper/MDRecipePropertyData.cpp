#include "MDRecipePropertyData.h"
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QCoreApplication>

CMDRecipeHeadData::CMDRecipeHeadData()
{
    m_name = "";
    m_comment = "";
    m_downloadMode = DownloadMode::general;
    m_itemsCountOnGeneral = 50;
    m_isDynamic = false;

    m_isSameValueIgnored = true;
    m_timeOut = 10;
    m_retryCount = 2;
}

CMDRecipeHeadData::~CMDRecipeHeadData()
{
   m_spList.deleteAll();
}

CMDRecipeHeadData::CMDRecipeHeadData(const CMDRecipeHeadData& obj)
{
    copy(&obj);
}

CMDRecipeHeadData& CMDRecipeHeadData::operator = (const CMDRecipeHeadData& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDRecipeHeadData::copy(const CMDRecipeHeadData* source)
{
    m_name = source->m_name;
    m_comment = source->m_comment;
    m_downloadMode = source->m_downloadMode;
    m_itemsCountOnGeneral = source->m_itemsCountOnGeneral;
    m_isDynamic = source->m_isDynamic;

    m_isSameValueIgnored = source->m_isSameValueIgnored;
    m_timeOut = source->m_timeOut;
    m_retryCount = source->m_retryCount;
    m_spList = source->m_spList;

    return true;
}

void CMDRecipeHeadData::deserialize(const QJsonObject &json)
{
    m_name = json["Name"].toString();
    m_comment = json["Comment"].toString();
    m_downloadMode = (DownloadMode)(json["DownloadMode"].toInt());
    m_itemsCountOnGeneral = json["ItemsCountOnGeneral"].toInt();
    m_isDynamic = (json["IsDynamic"].toInt() == 0 ? false : true);

    m_isSameValueIgnored = (json["IsSameValueIgnored"].toInt() == 0 ? false : true);
    m_timeOut = json["DownloadTimeOut"].toInt();
    m_retryCount = json["DownloadRetryCount"].toInt();

    QJsonArray spListJson = json.value("SPList").toArray();
    m_spList.deserialize(spListJson);
}

void CMDRecipeHeadData::serialize(QJsonObject &json)
{
    json["Name"] = m_name;
    json["Comment"] = m_comment;
    json["DownloadMode"] = QVariant((uint)m_downloadMode).toInt();
    json["ItemsCountOnGeneral"] = QVariant(m_itemsCountOnGeneral).toInt();
    int dynamic = (m_isDynamic ? 1: 0);
    json["IsDynamic"] = dynamic;

    json["IsSameValueIgnored"] = (m_isSameValueIgnored ? 1: 0);
    json["DownloadTimeOut"] = QVariant(m_timeOut).toInt();
    json["DownloadRetryCount"] = QVariant(m_retryCount).toInt();
    QJsonArray spListJson;
    m_spList.serialize(spListJson);
    json.insert("SPList", spListJson);
}

CMDRecipeItemData::CMDRecipeItemData()
{
}

CMDRecipeItemData::CMDRecipeItemData(const QString& id, const QString& value)
{
    m_id = id;
    m_value = value;
}

CMDRecipeItemData::~CMDRecipeItemData()
{
}

CMDRecipeItemData::CMDRecipeItemData(const CMDRecipeItemData& obj)
{
    copy(&obj);
}

CMDRecipeItemData& CMDRecipeItemData::operator = (const CMDRecipeItemData& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDRecipeItemData::copy(const CMDRecipeItemData* source)
{
    m_id = source->m_id;
    m_value = source->m_value;
    return true;
}

void CMDRecipeItemData::deserialize(const QJsonObject &json)
{
    m_id = json["ID"].toString();
    m_value = json["Value"].toString();
}

void CMDRecipeItemData::serialize(QJsonObject &json)
{
    json["ID"] = m_id;
    json["Value"] = m_value;
}

CMDRecipeItemsData::CMDRecipeItemsData()
{
}

CMDRecipeItemsData::~CMDRecipeItemsData()
{
    deleteAll();
}

CMDRecipeItemsData::CMDRecipeItemsData(const CMDRecipeItemsData& obj)
{
    copy(&obj);
}

CMDRecipeItemsData& CMDRecipeItemsData::operator = (const CMDRecipeItemsData& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDRecipeItemsData::copy(const CMDRecipeItemsData* source)
{
    deleteAll();

    foreach (CMDRecipeItem* item, source->m_itemMap)
    {
        addItem(*item);
    }
    return true;
}

void CMDRecipeItemsData::deserialize(const QJsonObject &json)
{
    QJsonArray dataArray = json["RecipeItems"].toArray();
    for (int index = 0; index < dataArray.size(); ++index)
    {
        QJsonObject jsItem = dataArray[index].toObject();
        CMDRecipeItem item;
        item.deserialize(jsItem);
        if(NULL == findItem(item.id()))
        {
            addItem(item);
        }
    }
}

void CMDRecipeItemsData::serialize(QJsonObject &json)
{
    QJsonArray dataArray;
    foreach (CMDRecipeItem* item, m_itemMap)
    {
        QJsonObject jsItem;
        item->serialize(jsItem);
        dataArray.append(jsItem);
    }
    json["RecipeItems"] = dataArray;
}

CMDRecipeItem* CMDRecipeItemsData::findItem(const QString& id)
{
    CMDRecipeItem* object = NULL;

    QMap<QString, CMDRecipeItem*>::const_iterator iPosition = m_itemMap.find(id.toUpper());
    if(iPosition != m_itemMap.constEnd())
        object = iPosition.value();

    return object;
}

bool CMDRecipeItemsData::addItem(const QString& id, const QString& value)
{
    return addItem(CMDRecipeItem(id, value));
}

bool CMDRecipeItemsData::addItem(const CMDRecipeItem& obj)
{
    CMDRecipeItem* newItem = new CMDRecipeItem(obj);
    QString mapId = newItem->id().toUpper();
    m_itemMap.insert(mapId, newItem);
    return true;
}

bool CMDRecipeItemsData::deleteItem(const QString& id)
{
    bool result = false;

    QMap<QString, CMDRecipeItem*>::iterator iPosition = m_itemMap.find(id.toUpper());
    if(iPosition != m_itemMap.constEnd())
   {
        CMDRecipeItem* object = iPosition.value();
        delete object;

        m_itemMap.erase(iPosition);

        result = true;
    }
    return result;
}

bool CMDRecipeItemsData::setValue(const QString& id, const QString& value)
{
    CMDRecipeItem* item = findItem(id);
    if(item)
        item->setValue(value);
    return (item != NULL);
}

QString CMDRecipeItemsData::getValue(const QString& id)
{
    CMDRecipeItem* item = findItem(id);
    return (item ? item->value() : "");
}

void CMDRecipeItemsData::deleteAll()
{
    foreach (CMDRecipeItem* item, m_itemMap)
    {
        delete item;
    }
    m_itemMap.clear();
}

void CMDRecipeItemsData::enumIds(QStringList& itemIDList)
{
    foreach (CMDRecipeItem* item, m_itemMap)
    {
        itemIDList.append(item->id());
    }
}

void CMDRecipeItemsData::enumItems(QList<CMDRecipeItem*>& itemList)
{
    foreach (CMDRecipeItem* item, m_itemMap)
    {
        itemList.append(item);
    }
}

QString CMDRecipeItemsData::toString()
{
    QJsonObject json;
    serialize(json);

    QJsonDocument doc;
    doc.setObject(json);
    QByteArray ba = doc.toJson();
    QString ss(ba);
    return ss;
}

CMDRecipePropertyData_Config::CMDRecipePropertyData_Config()
{

}

CMDRecipePropertyData_Config::~CMDRecipePropertyData_Config()
{

}

CMDRecipePropertyData_Config::CMDRecipePropertyData_Config(const CMDRecipePropertyData_Config& obj)
{
    copy(&obj);
}

CMDRecipePropertyData_Config& CMDRecipePropertyData_Config::operator = (const CMDRecipePropertyData_Config& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDRecipePropertyData_Config::copy(const CMDRecipePropertyData_Config* source)
{
    m_head = source->m_head;
    m_items = source->m_items;
    return true;
}

void CMDRecipePropertyData_Config::deserialize(const QJsonObject &json)
{
    m_head.deserialize(json);
    m_items.deserialize(json);
}

void CMDRecipePropertyData_Config::serialize(QJsonObject &json)
{
    m_head.serialize(json);
    m_items.serialize(json);
}

// 运行时属性
CMDRecipePropertyData_Runtime::CMDRecipePropertyData_Runtime()
{
    m_retryCount = 0;
    m_downloadBeginTime = QDateTime();
    m_downloadEndTime = QDateTime();

    m_dStatus = DownloadStatus::ready;
    m_dFinishFlag = DownloadFinishFlag::success;
}

CMDRecipePropertyData_Runtime::~CMDRecipePropertyData_Runtime()
{

}

CMDRecipePropertyData_Runtime::CMDRecipePropertyData_Runtime(const CMDRecipePropertyData_Runtime& obj)
{
    m_retryCount = obj.m_retryCount;
    m_downloadBeginTime = obj.m_downloadBeginTime;
    m_downloadEndTime = obj.m_downloadEndTime;

    m_dStatus = obj.m_dStatus;
    m_dFinishFlag = obj.m_dFinishFlag;
}

CMDRecipePropertyData_Runtime& CMDRecipePropertyData_Runtime::operator = (const CMDRecipePropertyData_Runtime& obj)
{
    if( this != &obj )
    {
        m_retryCount = obj.m_retryCount;
        m_downloadBeginTime = obj.m_downloadBeginTime;
        m_downloadEndTime = obj.m_downloadEndTime;

        m_dStatus = obj.m_dStatus;
        m_dFinishFlag = obj.m_dFinishFlag;
    }
    return *this;
}

