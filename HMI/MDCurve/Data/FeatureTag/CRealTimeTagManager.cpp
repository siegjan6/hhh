#include "CRealTimeTagManager.h"
#include "CPoint.h"
#include "CFeatureTagLine.h"
#include "CXAxis.h"
#include "CXAxisManager.h"
#include <QJsonArray>
#include <QJsonObject>
//#include <sys/time.h>
#include <time.h>
#include <QVector>
#include <QDebug>

CRealTimeTagManager::CRealTimeTagManager()
{
}

CRealTimeTagManager::~CRealTimeTagManager()
{
    clearItemList();
    clearOldData();
}

void CRealTimeTagManager::calculate()
{
//    double startTime = (double)clock();
    if(!calCheck())
        return;
    int index = maxRawData();
    if(index == -1)
        return;
    clearOldData();
    double xWidth = drawArea().width();
    for(int i = 0; i < m_items.count();i++)
    {
        CRealFTag *realItem = m_items.at(i);
        realItem->setXAxis(xs()->findAxis(realItem->xName()));

        qint64 valueLength = xs()->findAxis(realItem->xName())->valueLength();
        QDateTime timeMark = xs()->findAxis(realItem->xName())->timeMark();
        QDateTime startTime = xs()->findAxis(realItem->xName())->endTime().addSecs(-valueLength);
        QDateTime rawTime(startTime);

        if(!realItem->visible())
            continue;

        CCurveData data = rawDataList().at(0);
        for(int j = 0; j < data.timeList().count();j++)
        {
            for(int t = 0; t < rawDataList().count();t++)
            {
                CCurveData curveData = rawDataList().at(t);
                for(int k = 0;k <realItem->itemList().count();k++)
                {
                    if(curveData.name() != realItem->itemList().at(k)->itemName())
                    {
                        continue;
                    }
                    else
                    {
                        if(curveData.timeList().isEmpty())
                        {
                            break; // if the data is empty,break,goto next data loop
                        }
                        else
                        {
                            CTimeValuePoint point = curveData.timeList().at(j);
                            double value = point.y();
                            rawTime = point.xTime();
                            realItem->itemList().at(k)->setValue(value);
                        }
                    }
                }
            }

            qint64 timeConst = timeMark.secsTo(rawTime);
            qint64 time = startTime.secsTo(timeMark);
            // tX
            double tx = xWidth *timeConst / valueLength;
            int cx = xWidth*time/valueLength;
            tx += cx;

            CRealFTag *tag = new CRealFTag;
            *tag = *realItem;
            tag->setTime(rawTime);
            calTagPosition(tag,tx);
            tag->calclualte(drawArea().height(),tx,m_itemPosMap.value(tag->name()));
            m_itemLists.append(tag);
        }
    }
    setIsCalculated(true);
//    double endTime = (double)clock();
//    qDebug()<<"calculate:"<<(endTime - startTime) / 1000;

}

void CRealTimeTagManager::draw(QPainter *p)
{
//    double start = (double)clock();
    if(!isCalculated())
        return;
    p->save();

    //左上角为 (0,0) 点
    p->translate(drawArea().topLeft());

    QRegion region(0,0,drawArea().width(),drawArea().height());
    p->setClipRegion(region);

    for(int i = 0; i < m_itemLists.count();i++)
    {
        CRealFTag *item = m_itemLists.at(i);
        item->draw(p);
    }
    p->restore();
//    double end = (double)clock();
//    qDebug()<<"drawTime:"<<(end - start) /1000;
}

CItemTag CRealTimeTagManager::items() const
{
    return m_items;
}

void CRealTimeTagManager::setItems(const CItemTag &items)
{
    m_items = items;
}

void CRealTimeTagManager::addItem(CRealFTag *itemTag)
{
    if(itemTag)
    {
        if(CFeatureTagItem* valueItem= itemTag->item(CFeatureTagItem::ItemType::Value))
        {
            QString name = valueItem->itemName();
            if(m_itemPosMap.contains(name))
                return;
            m_itemPosMap.insert(name,CTagPos());
            m_items.append(itemTag);
        }
    }
}

void CRealTimeTagManager::removeItem(CRealFTag *itemTag)
{
    if(itemTag){
        if(CFeatureTagItem* valueItem= itemTag->item(CFeatureTagItem::ItemType::Value)){
            QString name = valueItem->itemName();
            m_itemPosMap.remove(name);
            m_items.removeOne(itemTag);
            delete itemTag;
            itemTag = NULL;
        }
    }
}

void CRealTimeTagManager::addItem(const QString &itemName)
{
    if(!m_itemPosMap.contains(itemName)){
        m_itemPosMap.insert(itemName,CTagPos());
    }
}

void CRealTimeTagManager::removeItem(const QString &itemName)
{
    m_itemPosMap.remove(itemName);
}

bool CRealTimeTagManager::calCheck()
{
    if(isStudioMode())
    {
        calculateStudioMode();
        return false;
    }
    if(!xs() || m_items.isEmpty() || drawArea().isNull() || rawDataList().isEmpty())
    {
        setIsCalculated(false);
        return false;
    }
    setIsCalculated(true);
    return true;
}

void CRealTimeTagManager::calculateStudioMode()
{
    if(!xs() || m_items.isEmpty() || drawArea().isNull())
    {
        setIsCalculated(false);
        return;
    }
    clearOldData();
    int totalVisible = 0;
    QList<int> visibleIndexList;
    for(int i = 0; i < m_items.count();i++)
    {
        CRealFTag *item = m_items.at(i);
        if(item->visible())
        {
           totalVisible++;
           visibleIndexList.append(i);
        }
    }
    for(int i= 0; i< totalVisible; i++)
    {
        CRealFTag *item = m_items.at(visibleIndexList.at(i));
        qint64 valueLength = item->xAxis()->valueLength();
        QDateTime startTime = item->xAxis()->endTime().addSecs(-valueLength);
        int averageValue = valueLength / (totalVisible + 1);
        QDateTime rawTime (startTime.addSecs(averageValue*(i+1)));

        // tX
        double xWidth = drawArea().width();
        double tx = xWidth / (totalVisible+ 1);
        CRealFTag *tag = new CRealFTag;
        *tag = *item;
        tag->setValue(0);
        tag->setTime(rawTime);

        tag->calclualte(drawArea().height(),tx*(i+1),m_itemPosMap.value(tag->name()));
        m_itemLists.append(tag);
    }
    setIsCalculated(true);
}

void CRealTimeTagManager::calTagPosition(CRealFTag *item,double x)
{
    if(item)
    {
        double totalHeight = item->totalHeight();
        CTagPos pos = m_itemPosMap.value(item->name());
        CTagHPos hPos = pos.hPos();
        CTagVPos vPos = pos.vPos();
        double yHeight = drawArea().height();
        double vOffset = vPos.offset();
        double hOffset = hPos.offset();
        QPointF point;

        switch (hPos.position()) {
        case CTagHPos::Position::Left:
        case CTagHPos::Position::Right:
            switch (vPos.position()) {
            case CTagVPos::Position::Top:
                point.setX(x+hOffset);
                point.setY(vOffset);
                break;
            case CTagVPos::Position::Bottom:
                point.setX(x+hOffset);
                point.setY(yHeight - totalHeight-vOffset);
                break;
            case CTagVPos::Position::Middle:
                point.setX(x);
                point.setY(yHeight/2 - totalHeight);
                break;
            default:
                break;
            }
            break;
        case CTagHPos::Position::Middle:
            switch (vPos.position()) {
            case CTagVPos::Position::Top:
                point.setX(x);
                point.setY(vOffset);
                break;
            case CTagVPos::Position::Bottom:
                point.setX(x);
                point.setY(yHeight - totalHeight-vOffset);
                break;
            case CTagVPos::Position::Middle:
                point.setX(x);
                point.setY(yHeight/2 - totalHeight);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        item->setTopLeft(point);
    }
}

void CRealTimeTagManager::clearOldData()
{
    if(!m_itemLists.isEmpty())
        qDeleteAll(m_itemLists.begin(),m_itemLists.end());
    m_itemLists.clear();
}
QMap<QString, CTagPos> CRealTimeTagManager::itemPosMap() const
{
    return m_itemPosMap;
}

void CRealTimeTagManager::setItemPosMap(const QMap<QString, CTagPos> &map)
{
    m_itemPosMap = map;
}

void CRealTimeTagManager::setXList()
{
    for(int i = 0; i < m_items.count();i++)
    {
        CRealFTag *realTag = m_items.at(i);
        if(xs())
        {
            realTag->setXAxis(xs()->findAxis(realTag->xName()));
        }
    }
}


CRealTimeTagManager &CRealTimeTagManager::operator =(const CRealTimeTagManager &other)
{
    if(this == &other)
        return *this;

    this->m_itemPosMap = other.m_itemPosMap;
    clearItemList();
    for(int i = 0; i <other.m_items.count();i++){
        CRealFTag *item = other.m_items.at(i);
        CRealFTag *tag = new CRealFTag;
        *tag = *item;
        m_items.append(tag);
    }
    CTagManager::operator =(other);
    return *this;
}

void CRealTimeTagManager::serialize(QJsonObject &json)
{
    CTagManager::serialize(json);
    QJsonObject myJson;
    int posCount = m_itemPosMap.count();
    QJsonArray posArray;
    for(int i = 0 ; i<posCount; i++)
    {
        QJsonObject tagJson;
        QString str = m_itemPosMap.keys().at(i);
        tagJson.insert("Key",str);
        CTagPos tagPos = m_itemPosMap.find(str).value();
        tagPos.serialize(tagJson);
        posArray.append(tagJson);
    }
    myJson.insert("ItemPosMap",posArray);

    int count = m_items.count();
    QJsonArray array;
    for(int i = 0; i<count; i++)
    {
        QJsonObject listJson;
        CRealFTag *realTag = m_items.at(i);
        realTag->serialize(listJson);
        array.append(listJson);
    }
    myJson.insert("TagList",array);
    json.insert("RealTimeTagManager",myJson);
}

void CRealTimeTagManager::deserialize(const QJsonObject &json)
{
    clearItemList();
    CTagManager::deserialize(json);
    QJsonObject myJson = json.value("RealTimeTagManager").toObject();
    QJsonArray posArray = myJson.value("ItemPosMap").toArray();
    int posCount = posArray.count();
    for(int i = 0; i< posCount;i++)
    {
        QJsonObject tagJson =posArray.at(i).toObject();
        QString str = tagJson.value("Key").toString();
        CTagPos tagPos;
        tagPos.deserialize(tagJson);
        m_itemPosMap.insert(str,tagPos);
    }

    QJsonArray array = myJson.value("TagList").toArray();
    int count = array.count();
    for(int i = 0; i < count; i++)
    {
        QJsonObject listJson = array.at(i).toObject();
        CRealFTag *real = new CRealFTag;
        real->deserialize(listJson);
        m_items.append(real);
    }
}

void CRealTimeTagManager::clearItemList()
{
    if(!m_items.isEmpty())
        qDeleteAll(m_items.begin(),m_items.end());
    m_items.clear();
}

int CRealTimeTagManager::maxRawData()
{
    double start = (double)clock();
    QList<int> dataItemList;
    for(int i = 0; i< rawDataList().count(); i++)
    {
        CCurveData data = rawDataList().at(i);
        dataItemList.append(data.timeList().count());
    }
    int unEquire = 0;
    for(int i = 0; i< dataItemList.count();i++)
    {
        int count = dataItemList.at(0);
        if(count != 0)
        {
            if(dataItemList.at(i) % count != 0)
                unEquire++;
        }
    }
    if(!unEquire)
        return 0;

    if(rawDataList().count() == 0)
        return -1;

    int timeCountZero = 0;
    for(int i = 0; i< rawDataList().count(); i++)
    {
        CCurveData data = rawDataList().at(i);
        if(data.timeList().count() == 0)
        {
            timeCountZero++;
        }
    }
    if(timeCountZero != 0)
        return -1;

    CCurveData data = rawDataList().at(0);
    int maxCount = data.timeList().count();
    if(maxCount <= 0)
        return -1;
    QList<QDateTime> allDateTime;
    for(int i = 0; i<rawDataList().count();i++)
    {
        CCurveData curveData = rawDataList().at(i);
        for(int j = 0; j<curveData.timeList().count();j++)
        {
            QDateTime dateTime = curveData.timeList().at(j).xTime();
            bool isContained = false;
            if(allDateTime.count() != 0)
            {
                for(int k = 0; k<allDateTime.count();k++)
                {
                    if(allDateTime.at(k).date() == dateTime.date()
                            && allDateTime.at(k).time().hour() == dateTime.time().hour()
                            && allDateTime.at(k).time().minute() == dateTime.time().minute()
                            && allDateTime.at(k).time().second() == dateTime.time().second())
                    {
                        isContained = true;
                    }
                }
                if(!isContained)
                {
                    allDateTime.append(dateTime);
                }
            }
            else
                allDateTime.append(dateTime);

        }
    }

    for(int i = 0; i<allDateTime.count(); i++)
    {
        for(int j = 0; j <allDateTime.count() -i-1;j++)
        {
            if(allDateTime[j]>allDateTime[j+1])
            {
                QDateTime bigger = allDateTime[j];
                allDateTime[j]=allDateTime[j+1];
                allDateTime[j+1] = bigger;
            }
        }
    }

    for(int i = 0; i<allDateTime.count();i++)
    {
        QDateTime dateTime = allDateTime.at(i);
        for(int j = 0; j< rawDataList().count(); j++)
        {
            CCurveData curveData = rawDataList().at(j);
            QDateTime maxDateTime = curveData.timeList().last().xTime();
            QDateTime minDateTime = curveData.timeList().first().xTime();
            bool isContained = false;
            for(int t = 0; t<curveData.timeList().count(); t++)
            {
                QDateTime valueDateTime = curveData.timeList().at(t).xTime();
                if( valueDateTime.date() == dateTime.date()
                        && valueDateTime.time().hour() == dateTime.time().hour()
                        && valueDateTime.time().minute() == dateTime.time().minute()
                        && valueDateTime.time().second() == dateTime.time().second())
                {
                    isContained = true;
                    break;
                }
            }

            if(!isContained)
            {
                CTimeValuePoint point;
                point.setXTime(dateTime);
                if(dateTime < minDateTime)
                    point.setY(0);
                else
                {
                    CTimeValuePoint valuePoint = curveData.timeList().at(i-1);
                    point.setY(valuePoint.y());
                }
                curveData.timeList().insert(i,point);
            }
            rawDataList().removeAt(j);
            rawDataList().insert(j,curveData);
        }
    }
    double end = (double)clock();
    qDebug()<<"maxTime:"<<(end - start) / 1000;
    return 0;
}

void CRealTimeTagManager::changeLanguage()
{
    QListIterator<CRealFTag*> iter(m_itemLists);
    while(iter.hasNext()){
        iter.next()->changeLanguage();
    }
}

void CRealTimeTagManager::collectWords()
{
    QListIterator<CRealFTag*> iter(m_itemLists);
    while(iter.hasNext()){
        iter.next()->collectWords();
    }
}
