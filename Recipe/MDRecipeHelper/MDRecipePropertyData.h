#ifndef MDRECIPEPROPERTYDATA_H
#define MDRECIPEPROPERTYDATA_H

#include <QString>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QDateTime>
#include "MDRecipe.h"
#include "MDActionID.h"
#include "MDActionSP.h"

/*!
 * \brief 配方头
 */
class CMDRecipeHeadData
{
public:
    CMDRecipeHeadData();
    ~CMDRecipeHeadData();
    CMDRecipeHeadData(const CMDRecipeHeadData& obj);
    CMDRecipeHeadData& operator = (const CMDRecipeHeadData& obj);
    bool copy(const CMDRecipeHeadData* source);
    void deserialize(const QJsonObject &json);
    void serialize(QJsonObject &json);
public:
    DownloadMode m_downloadMode;
    qint16 m_itemsCountOnGeneral;
    bool m_isDynamic;
    QString m_name;
    QString m_comment;
public:
    bool m_isSameValueIgnored;
    int m_timeOut;
    int m_retryCount;
public:
    CMDActionSPList m_spList;
};

/*!
 * \brief 配方项
 */
class CMDRecipeItemData
{
public:
    CMDRecipeItemData();
    CMDRecipeItemData(const QString& id, const QString& value);
    ~CMDRecipeItemData();
    CMDRecipeItemData(const CMDRecipeItemData& obj);
    CMDRecipeItemData& operator = (const CMDRecipeItemData& obj);
    bool copy(const CMDRecipeItemData* source);
    void deserialize(const QJsonObject &json);
    void serialize(QJsonObject &json);
public:
    QString m_id;
    QString m_value;
};
class CMDRecipeItemsData
{
public:
    CMDRecipeItemsData();
    ~CMDRecipeItemsData();
    CMDRecipeItemsData(const CMDRecipeItemsData& obj);
    CMDRecipeItemsData& operator = (const CMDRecipeItemsData& obj);
    bool copy(const CMDRecipeItemsData* source);
    void deserialize(const QJsonObject &json);
    void serialize(QJsonObject &json);
public:
    CMDRecipeItem* findItem(const QString& id);
    bool addItem(const QString& id, const QString& value);
    bool addItem(const CMDRecipeItem& obj);
    bool deleteItem(const QString& id);
    bool setValue(const QString& id, const QString& value);
    QString getValue(const QString& id);
    void deleteAll();
    void enumIds(QStringList& itemIDList);
    QString toString();
    void enumItems(QList<CMDRecipeItem*>& itemList);
public:
    QMap<QString, CMDRecipeItem*> m_itemMap;
};

/*!
 * \brief 配方数据基类的实现类
 */
class CMDRecipePropertyData_Config
{
public:
    CMDRecipePropertyData_Config();
    ~CMDRecipePropertyData_Config();
    CMDRecipePropertyData_Config(const CMDRecipePropertyData_Config& obj);
    CMDRecipePropertyData_Config& operator = (const CMDRecipePropertyData_Config& obj);
    bool copy(const CMDRecipePropertyData_Config* source);
    void deserialize(const QJsonObject &json);
    void serialize(QJsonObject &json);
public:
    CMDRecipeHead m_head;
    CMDRecipeItems m_items;
};

class CMDRecipePropertyData_Runtime
{
public:
    CMDRecipePropertyData_Runtime();
    ~CMDRecipePropertyData_Runtime();
   CMDRecipePropertyData_Runtime(const CMDRecipePropertyData_Runtime& obj);
   CMDRecipePropertyData_Runtime& operator = (const CMDRecipePropertyData_Runtime& obj);
public:
    int m_retryCount;
    QDateTime m_downloadBeginTime;
    QDateTime m_downloadEndTime;
public:
    DownloadStatus m_dStatus;
    DownloadFinishFlag m_dFinishFlag;
    QString m_dMsg;
};

#endif // MDRECIPEPROPERTYDATA_H
