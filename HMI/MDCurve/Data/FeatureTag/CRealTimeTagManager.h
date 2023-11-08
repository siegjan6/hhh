/*!

\author eHVr

\section 功能
    实时标签组
    每个变量绑定的标签相互独立，当有值触发时，及时绘制。
*/
#ifndef CREALTIMETAGMANAGER_H
#define CREALTIMETAGMANAGER_H

#include <QList>
#include <QPainter>
#include "CTagPosition.h"
#include "CRealFTag.h"
#include "CDataSource.h"
#include "Data.h"
#include "CFeatureTag.h"
#include "CTagManager.h"

#include <QMap>

typedef QList<CRealFTag *> CItemTag;

class CPoint;
class CXAxis;
class CFeatureTagItem;
class CFeatureTagLine;
class CRealTimeTagManager:public CTagManager
{
public:
    CRealTimeTagManager();
    ~CRealTimeTagManager();
public:
    void calculate();
    void draw(QPainter *p);

    CItemTag items() const;
    void setItems(const CItemTag &items);

    void addItem(CRealFTag*itemTag);
    void removeItem(CRealFTag *itemTag);

    void addItem(const QString &itemName);
    void removeItem(const QString &itemName);

    QMap<QString, CTagPos> itemPosMap() const;
    void setItemPosMap(const QMap<QString,CTagPos> & map);
    // no use
public:
//    CFeatureTagLine *tagLine() const;
//    CPoint *topPoint() const;
//    CPoint *bottomPoint() const;
    void setXList(); // setting the x axis pointers.
private:
    bool calCheck();                               //! 计算检测
    void calculateStudioMode();                    //! Studio 模式下的特征标签
    void calTagPosition(CRealFTag *item, double x);//! 计算每个tag的位置
    void clearOldData();                           //! 清除之前的数据
private:
    CItemTag m_items;                   //! tag list
    CItemTag m_itemLists;               //! 绘制时的所有标签
    QMap<QString,CTagPos> m_itemPosMap; //! 特征标签的位置

/*!
    \section 序列化和反序列化
*/
public:
    CRealTimeTagManager &operator = (const CRealTimeTagManager& other);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

private:
    void clearItemList();
    int maxRawData();
/*!
    \section 多语言
*/
public:
    void changeLanguage();
    void collectWords();

};
#endif // CREALTIMETAGMANAGER_H
