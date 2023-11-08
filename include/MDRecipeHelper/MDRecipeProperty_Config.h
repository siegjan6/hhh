#ifndef MDRECIPEPROPERTY_CONFIG_H
#define MDRECIPEPROPERTY_CONFIG_H
#include "MDRecipeHelper_Global.h"
#include <QStringList>
#include <QJsonObject>

enum class DownloadMode
{
    general = 0,
    quick = 1
};

class CMDRecipeHead;
class CMDRecipeItems;
class CMDRecipePropertyData_Config;
class MDRECIPEHELPERSHARED_EXPORT CMDRecipeProperty_Config
{
public:
    CMDRecipeProperty_Config();
     virtual ~CMDRecipeProperty_Config();
    CMDRecipeProperty_Config(const CMDRecipeProperty_Config& obj);
    CMDRecipeProperty_Config& operator = (const CMDRecipeProperty_Config& obj);
/*!
\section 属性
*/
public:
    CMDRecipeHead* head();
    CMDRecipeItems* items();

/*!
\section 序列化：JSON格式
*/
public:
    bool copy(const CMDRecipeProperty_Config* source);
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
protected:
    CMDRecipePropertyData_Config* m_data;
};

class CMDActionSPList;
class CMDRecipeHeadData;
class MDRECIPEHELPERSHARED_EXPORT CMDRecipeHead
{
public:
    CMDRecipeHead();
    ~CMDRecipeHead();
    CMDRecipeHead(const CMDRecipeHead& obj);
    CMDRecipeHead& operator = (const CMDRecipeHead& obj);
public:
    bool copy(const CMDRecipeHead* source);
    void deserialize(const QJsonObject &json);
    void serialize(QJsonObject &json);
public:
    QString name() const;
    void setName(const QString& name);

    DownloadMode downloadMode() const;
    void setDownloadMode(DownloadMode mode);

    qint16 itemsCountOnGeneral() const;
    void setItemsCountOnGeneral(qint16 itemsCount);

    QString comment() const;
    void setComment(const QString& comment);

    bool isDynamic() const;
    void setDynamic(bool isDynamic);
public:
    bool isSameValueIgnored() const;
    void setSameValueIgnored(bool isSameValueIgnored);

    int timeOut() const;
    void setTimeOut(int timeOut);

    int retryCount() const;
    void setRetryCount(int count);
public:
    CMDActionSPList* spList();
    bool setSPList(const CMDActionSPList* spList);
protected:
    CMDRecipeHeadData* m_data;
};

class CMDRecipeItem;
class CMDRecipeItemsData;
class MDRECIPEHELPERSHARED_EXPORT CMDRecipeItems
{
public:
    CMDRecipeItems();
    ~CMDRecipeItems();
    CMDRecipeItems(const CMDRecipeItems& obj);
    CMDRecipeItems& operator = (const CMDRecipeItems& obj);
public:
    bool copy(const CMDRecipeItems* source);
    void deserialize(const QJsonObject &json);
    void serialize(QJsonObject &json);
public:
    bool addItem(const QString& id, const QString& value);
    bool deleteItem(const QString& id);
    bool setItemValue(const QString& id, const QString& value);
    QString itemValue(const QString& id);
    void deleteAll();
public:
    int count();
    void enumItems(QList<CMDRecipeItem*>& itemList);
    void enumIds(QStringList& itemList);
    QString toString();
protected:
    CMDRecipeItemsData* m_data;
};

/*!
 * \brief 配方项
 */
class CMDRecipeItemData;
class MDRECIPEHELPERSHARED_EXPORT CMDRecipeItem
{
public:
    CMDRecipeItem();
    CMDRecipeItem(const QString& id, const QString& value);
    ~CMDRecipeItem();
    CMDRecipeItem(const CMDRecipeItem& obj);
    CMDRecipeItem& operator = (const CMDRecipeItem& obj);
    bool copy(const CMDRecipeItem* source);
    void deserialize(const QJsonObject &json);
    void serialize(QJsonObject &json);
public:
    QString id();
    void setID(const QString& id);
    QString value();
    void setValue(const QString& value);
protected:
    CMDRecipeItemData* m_data;
};

#endif // MDRECIPEPROPERTY_CONFIG_H
