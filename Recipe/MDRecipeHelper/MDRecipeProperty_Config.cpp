#include "MDRecipeProperty_Config.h"
#include "MDRecipePropertyData.h"

CMDRecipeProperty_Config::CMDRecipeProperty_Config()
{
    m_data = new CMDRecipePropertyData_Config;
}

CMDRecipeProperty_Config::~CMDRecipeProperty_Config()
{
    if(m_data)
        delete m_data;
}

CMDRecipeProperty_Config::CMDRecipeProperty_Config(const CMDRecipeProperty_Config& obj)
{
    m_data = new CMDRecipePropertyData_Config();

    copy(&obj);
}

CMDRecipeProperty_Config& CMDRecipeProperty_Config::operator = (const CMDRecipeProperty_Config& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

/*!
\section 属性
*/
CMDRecipeHead* CMDRecipeProperty_Config::head()
{
    return &m_data->m_head;
}

CMDRecipeItems* CMDRecipeProperty_Config::items()
{
    return &m_data->m_items;
}

bool CMDRecipeProperty_Config::copy(const CMDRecipeProperty_Config* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

/*!
\section 序列化：JSON格式
*/
void CMDRecipeProperty_Config::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDRecipeProperty_Config::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}

// 配方头
CMDRecipeHead::CMDRecipeHead()
{
    m_data = new CMDRecipeHeadData;
}

CMDRecipeHead::~CMDRecipeHead()
{
    if(m_data)
        delete m_data;
}


CMDRecipeHead::CMDRecipeHead(const CMDRecipeHead& obj)
{
    m_data = new CMDRecipeHeadData();

    copy(&obj);
}

CMDRecipeHead& CMDRecipeHead::operator = (const CMDRecipeHead& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDRecipeHead::copy(const CMDRecipeHead* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

QString CMDRecipeHead::name() const
{
    return (m_data ? m_data->m_name : "");
}
void CMDRecipeHead::setName(const QString& name)
{
    if(m_data)
        m_data->m_name = name;
}

DownloadMode CMDRecipeHead::downloadMode() const
{
    return (m_data ? m_data->m_downloadMode : DownloadMode::general);
}
void CMDRecipeHead::setDownloadMode(DownloadMode mode)
{
    if(m_data)
        m_data->m_downloadMode = mode;
}

qint16 CMDRecipeHead::itemsCountOnGeneral() const
{
    return (m_data ? m_data->m_itemsCountOnGeneral : 50);
}
void CMDRecipeHead::setItemsCountOnGeneral(qint16 itemsCount)
{
    if(m_data)
        m_data->m_itemsCountOnGeneral = itemsCount;
}

QString CMDRecipeHead::comment() const
{
    return (m_data ? m_data->m_comment : "");
}
void CMDRecipeHead::setComment(const QString& comment)
{
    if(m_data)
        m_data->m_comment = comment;
}

bool CMDRecipeHead::isDynamic() const
{
    return (m_data ? m_data->m_isDynamic : false);
}
void CMDRecipeHead::setDynamic(bool isDynamic)
{
    if(m_data)
        m_data->m_isDynamic = isDynamic;
}

bool CMDRecipeHead::isSameValueIgnored() const
{
    return (m_data ? m_data->m_isSameValueIgnored : false);
}
void CMDRecipeHead::setSameValueIgnored(bool isSameValueIgnored)
{
    if(m_data)
        m_data->m_isSameValueIgnored = isSameValueIgnored;
}

int CMDRecipeHead::timeOut() const
{
    return (m_data ? m_data->m_timeOut : 10);
}
void CMDRecipeHead::setTimeOut(int timeOut)
{
    if(m_data)
        m_data->m_timeOut = timeOut;
}

int CMDRecipeHead::retryCount() const
{
    return (m_data ? m_data->m_retryCount : 2);
}
void CMDRecipeHead::setRetryCount(int retryCount)
{
    if(m_data)
        m_data->m_retryCount = retryCount;
}

CMDActionSPList* CMDRecipeHead::spList()
{
    return (m_data ? &m_data->m_spList : NULL);
}
bool CMDRecipeHead::setSPList(const CMDActionSPList* spList)
{
    if(!m_data || !spList)
        return false;

    m_data->m_spList = *spList;
    return true;
}

/*!
\section 序列化：JSON格式
*/
void CMDRecipeHead::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDRecipeHead::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}

// 配方项
CMDRecipeItems::CMDRecipeItems()
{
    m_data = new CMDRecipeItemsData;
}

CMDRecipeItems::~CMDRecipeItems()
{
    if(m_data)
        delete m_data;
}

CMDRecipeItems::CMDRecipeItems(const CMDRecipeItems& obj)
{
    m_data = new CMDRecipeItemsData();

    copy(&obj);
}

CMDRecipeItems& CMDRecipeItems::operator = (const CMDRecipeItems& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDRecipeItems::copy(const CMDRecipeItems* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

void CMDRecipeItems::deserialize(const QJsonObject &json)
{
    m_data->deserialize(json);
}

void CMDRecipeItems::serialize(QJsonObject &json)
{
    m_data->serialize(json);
}

bool CMDRecipeItems::addItem(const QString& id, const QString& value)
{
    return (m_data ? m_data->addItem(id, value) : false);
}

bool CMDRecipeItems::deleteItem(const QString& id)
{
    return (m_data ? m_data->deleteItem(id) : false);
}

bool CMDRecipeItems::setItemValue(const QString& id, const QString& value)
{
    return (m_data ? m_data->setValue(id, value) : false);
}

QString CMDRecipeItems::itemValue(const QString& id)
{
    return (m_data ? m_data->getValue(id) : "");
}

void CMDRecipeItems::deleteAll()
{
    m_data->deleteAll();
}

int CMDRecipeItems::count()
{
    return m_data->m_itemMap.size();
}

void CMDRecipeItems::enumIds(QStringList& itemIDList)
{
    m_data->enumIds(itemIDList);
}

void CMDRecipeItems::enumItems(QList<CMDRecipeItem*>& itemList)
{
    m_data->enumItems(itemList);
}

QString CMDRecipeItems::toString()
{
    return m_data->toString();
}

// 配方项
CMDRecipeItem::CMDRecipeItem()
{
    m_data = new CMDRecipeItemData;
}

CMDRecipeItem::CMDRecipeItem(const QString& id, const QString& value)
{
    m_data = new CMDRecipeItemData(id, value);
}

CMDRecipeItem::~CMDRecipeItem()
{
    if(m_data)
        delete m_data;
}

CMDRecipeItem::CMDRecipeItem(const CMDRecipeItem& obj)
{
    m_data = new CMDRecipeItemData;

    copy(&obj);
}

CMDRecipeItem& CMDRecipeItem::operator = (const CMDRecipeItem& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDRecipeItem::copy(const CMDRecipeItem* source)
{
    return m_data->copy(source->m_data);
}

void CMDRecipeItem::deserialize(const QJsonObject &json)
{
    m_data->deserialize(json);
}

void CMDRecipeItem::serialize(QJsonObject &json)
{
    m_data->serialize(json);
}

QString CMDRecipeItem::id()
{
    return m_data->m_id;
}

void CMDRecipeItem::setID(const QString& id)
{
    m_data->m_id = id;
}

QString CMDRecipeItem::value()
{
    return m_data->m_value;
}

void CMDRecipeItem::setValue(const QString& value)
{
    m_data->m_value = value;
}
