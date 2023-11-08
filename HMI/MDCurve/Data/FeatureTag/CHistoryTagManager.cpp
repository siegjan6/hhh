#include "CHistoryTagManager.h"
#include "CFeatureTagLine.h"
#include "CPoint.h"
#include "CXAxis.h"
#include "CXAxisManager.h"
#include "Data.h"
#include "CFeatureTagTimeItem.h"
#include "CFeatureTagValueItem.h"
#include "CFeatureTagCommentItem.h"

#include "CFind.h"
#include "CFeatureTag.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QPainter>
#include <QDebug>
//#include <sys/time.h>
#include <time.h>

CHistoryTagManager::CHistoryTagManager()
{
}

CHistoryTagManager::~CHistoryTagManager()
{ 
}

/*!
\section
*/

QList<CFeatureTag*> & CHistoryTagManager::tagList()
{
    return m_tagList;
}

void CHistoryTagManager::setTagList(const QList<CFeatureTag *> &tagList)
{
    m_tagList = tagList;
}

QMap<QString, CTagPos> CHistoryTagManager::itemPosMap() const
{
    return m_itemPosMap;
}

void CHistoryTagManager::setItemPosMap(const QMap<QString, CTagPos> &map)
{
    m_itemPosMap = map;
}

/*!
\section
*/
CTagHPos CHistoryTagManager::hPos() const
{
    return m_hPos;
}

void CHistoryTagManager::setHPos(const CTagHPos &hPos)
{
    m_hPos = hPos;
}

void CHistoryTagManager::setProjectPath(const QString &path)
{
    m_projectPath = path;
}

QString CHistoryTagManager::projectPath() const
{
    return m_projectPath;
}

CTagVPos CHistoryTagManager::vPos() const
{
    return m_vPos;
}

void CHistoryTagManager::setVPos(const CTagVPos &vPos)
{
    m_vPos = vPos;
}

void CHistoryTagManager::setXList()
{
    for(int i = 0; i < m_tagList.count();i++)
    {
        CFeatureTag *tag = m_tagList.at(i);
        if(xs())
        {
            tag->setXAxis(xs()->findAxis(tag->xName()));
        }
    }
}

/*!
\section
*/
void CHistoryTagManager::calculate()
{
//    if(!calCheck())
//        return;
//    clearOldData();
//    double xWidth = drawArea().width();
//    double yHeight = drawArea().height();
//    double yStart = topPoint()->size();
//    double yEnd = yHeight - bottomPoint()->size();
//    qint64 valueLength = referenceAxis()->valueLength();
//    QDateTime timeMark = referenceAxis()->timeMark();
//    QDateTime startTime = referenceAxis()->endTime().addSecs(-valueLength);
//    QDateTime rawTime  = startTime;
//    // 处理参考变量的数据
//    for(int i = 0 ; i <= valueLength;i++)
//    {
//        qint64 timeConst = timeMark.secsTo(rawTime);
//        qint64 time = startTime.secsTo(timeMark);
//        // tX 为数据点到Mark点的长度
//        double tx = xWidth *timeConst / valueLength;
//        int cx = xWidth*time/valueLength;
//        tx += cx;

//        CFeatureTag *tag = new CFeatureTag;
//        *tag = *m_tag;

//        QVector<bool> valueItemVisible;
//        valueItemVisible.clear();

//        for(int i = 0; i < tag->itemList().count();i++)
//        {
//            CFeatureTagItem *item = tag->itemList().at(i);

//            switch (item->type())
//            {
//            case CFeatureTagItem::ItemType::Time:
//                item->setDateTime(rawTime);
//                break;
//            case CFeatureTagItem::ItemType::Comment:
//                break;
//            case CFeatureTagItem::ItemType::Value:
//                valueItemVisible.append(calculateRunTimeValueMode(item,rawTime));
//                break;
//            default:
//                break;
//            }
//        }

//        bool itemVisible = false;
//        for(int i = 0;i< valueItemVisible.count();i++)
//        {
//            bool value = valueItemVisible.at(i);
//            if(value)
//                itemVisible= true;
//        }
//        if(itemVisible)
//        {
//            tag->setDrawArea(drawArea());
//            tag->setXPoint(tx);
//            tag->calculate();

////            m_tagList.append(tag);

//            tagLine()->appendLinePoints(QPointF(tx,yStart),QPointF(tx,yEnd));
//            topPoint() ->figurePoint(QPointF(tx,yStart));
//            bottomPoint()->figurePoint(QPointF(tx,yEnd));
//        }
//        rawTime = rawTime.addSecs(1);
//    }

//    setIsCalculated(true);

    double startTime = (double)clock();
//    QStringList stringList;
//    for(int i = 0; i<m_tagList.count(); i++)
//    {
//        CFeatureTag *tag = m_tagList.at(i);
//        for(int j = 0; j < tag->itemList().count();j++)
//        {
//            CFeatureTagItem *item = tag->itemList().at(j);
//            if(item->type() == CFeatureTagItem::ItemType::Value)
//            {
//                stringList.append(item->itemName());
//            }
//        }
//    }
//    setFields(stringList);

    if(!calCheck())
        return;
    clearOldData();
    double xWidth = drawArea().width();
    double yHeight = drawArea().height();
    double yStart = topPoint()->size();
    double yEnd = yHeight - bottomPoint()->size();

    for(int i = 0; i<m_tagList.count(); i++)
    {
        CFeatureTag *featureTag = m_tagList.at(i);
        if(!featureTag->visible())
            continue;
        int valueLength = featureTag->xAxis()->valueLength();
        QDateTime timeMark = featureTag->xAxis()->timeMark();
        QDateTime startTime = featureTag->xAxis()->endTime().addSecs(-valueLength);
        QDateTime rawTime  = startTime;
        for(int j = 0 ; j <= valueLength;j++)
        {
            qint64 timeConst = timeMark.secsTo(rawTime);
            qint64 time = startTime.secsTo(timeMark);
            // tX 为数据点到Mark点的长度
            double tx = xWidth *timeConst / valueLength;
            int cx = xWidth*time/valueLength;
            tx += cx;

            CFeatureTag *tag = new CFeatureTag;
            *tag = *featureTag;

            QVector<bool> valueItemVisible;
            valueItemVisible.clear();

            for(int i = 0; i < tag->itemList().count();i++)
            {
                CFeatureTagItem *item = tag->itemList().at(i);

                switch (item->type())
                {
                case CFeatureTagItem::ItemType::Time:
                    item->setDateTime(rawTime);
                    break;
                case CFeatureTagItem::ItemType::Comment:
                    break;
                case CFeatureTagItem::ItemType::Value:
                    valueItemVisible.append(calculateRunTimeValueMode(item,rawTime));
                    break;
                default:
                    break;
                }
            }

            bool itemVisible = false;
            for(int i = 0;i< valueItemVisible.count();i++)
            {
                bool value = valueItemVisible.at(i);
                if(value)
                    itemVisible= true;
            }
            if(itemVisible)
            {
                tag->setDrawArea(drawArea());
                tag->setXPoint(tx);
                tag->calculate(drawArea().height(),tx,m_itemPosMap.value(tag->name()));

                tagLine()->appendLinePoints(QPointF(tx,yStart),QPointF(tx,yEnd));
                topPoint() ->figurePoint(QPointF(tx,yStart));
                bottomPoint()->figurePoint(QPointF(tx,yEnd));

                m_itemsTagList.append(tag);
            }
            rawTime = rawTime.addSecs(1);   
        }
    }
    setIsCalculated(true);
    double endTime = (double)clock();
    qDebug()<<"tagCalTime:"<<(endTime-startTime)/1000.0;
}

void CHistoryTagManager::draw(QPainter *p)
{
    if(!isCalculated())
        return;
    p->save();
    //左上角为 (0,0) 点
    p->translate(drawArea().topLeft());

    QRegion region(0,0,drawArea().width(),drawArea().height());
    p->setClipRegion(region);

    p->setPen(QColor(Qt::black));
    for(int i = 0; i < m_itemsTagList.count();i++)
    {
        CFeatureTag *tag = m_itemsTagList.at(i);
        tag->draw(p);
    }
    p->restore();
}

bool CHistoryTagManager::calCheck()
{
    if(isStudioMode())
    {
        calculateStudioMode();
        return false;
    }
    if(!xs() || rawDataList().isEmpty())
    {
        setIsCalculated(false);
        return false;
    }
    return true;
}

void CHistoryTagManager::clearOldData()
{
    if(!m_itemsTagList.isEmpty())
        qDeleteAll(m_itemsTagList.begin(),m_itemsTagList.end());
    m_itemsTagList.clear();
}


int CHistoryTagManager::findItem(const QString &name, const QList<CFeatureTagItem *> &list)
{
    for(int i = 0;i < list.count(); i++)
    {
        if(name == list.at(i)->itemName())
            return i;
    }
    return -1;
}

void CHistoryTagManager::calculateStudioMode()
{
    if(!xs() || m_tagList.isEmpty())
    {
        setIsCalculated(false);
        return;
    }

    clearOldData();


    int totalVisible = 0;
    QList<int> visibleIndexList;
    for(int i = 0; i < m_tagList.count();i++)
    {
        CFeatureTag *tag = m_tagList.at(i);
//        if(!tag->visible())
//            continue;
        if(tag->visible())
        {
            totalVisible++;
            visibleIndexList.append(i);
        }
    }
    for(int i = 0; i< totalVisible; i++)
    {
        CFeatureTag *tag = m_tagList.at(visibleIndexList.at(i));
        int valueLength = tag->xAxis()->valueLength();
        QDateTime startTime = tag->xAxis()->endTime().addSecs(-valueLength);
        int averageValue = valueLength / (totalVisible + 1);
        QDateTime rawTime (startTime.addSecs(averageValue*(i+1)));

        // tX
        double xWidth = drawArea().width();
        double tx = xWidth / (totalVisible + 1);
        CFeatureTag *featureTag = new CFeatureTag;
        *featureTag = *tag;
        featureTag->setValue(0);
        featureTag->setTime(rawTime);

        featureTag->calculate(drawArea().height(),tx*(i+1),m_itemPosMap.value(tag->name()));
        m_itemsTagList.append(featureTag);

    }
    setIsCalculated(true);
}

bool CHistoryTagManager::calculateRunTimeValueMode(CFeatureTagItem *item, const QDateTime &rawTime)
{
    if(item)
    {
        for(int i = 0; i < rawDataList().count();i++)
        {
            CCurveData data = rawDataList().at(i);
            if(data.name() == item->itemName())
            {
                // find the Exactly the equal point.return the index or -1.
                int index = CFind::binarySearchExact(data.timeList(),rawTime);
                if(index == -1)
                {
                    item->setVisible(false);
                    return false;
                }
                else
                {
                    item->setValue(data.timeList().at(index).y());
                    return true;
                }
            }
        }
    }
    return false;
}

void CHistoryTagManager::serialize(QJsonObject &json)
{
    CTagManager::serialize(json);
    json.insert("ProjectPath",m_projectPath);
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
    json.insert("ItemPosMap",posArray);

    int count = m_tagList.count();
    QJsonArray array;
    for(int i = 0; i<count; i++)
    {
        QJsonObject listJson;
        CFeatureTag *featureTag = m_tagList.at(i);
        featureTag->serialize(listJson);
        array.append(listJson);
    }
    json.insert("TagList",array);

}

void CHistoryTagManager::deserialize(const QJsonObject &json)
{
    if(!m_tagList.isEmpty())
        qDeleteAll(m_tagList.begin(),m_tagList.end());
    m_tagList.clear();

   CTagManager::deserialize(json);
   m_projectPath = json.value("ProjectPath").toString();
   QJsonArray posArray = json.value("ItemPosMap").toArray();
   int posCount = posArray.count();
   for(int i = 0; i< posCount;i++)
   {
       QJsonObject tagJson =posArray.at(i).toObject();
       QString str = tagJson.value("Key").toString();
       CTagPos tagPos;
       tagPos.deserialize(tagJson);
       m_itemPosMap.insert(str,tagPos);
   }

   QJsonArray array = json.value("TagList").toArray();
   int count = array.count();
   for(int i = 0; i < count; i++)
   {
       QJsonObject listJson = array.at(i).toObject();
       CFeatureTag *feature = new CFeatureTag;
       feature->deserialize(listJson);
       m_tagList.append(feature);
   }
}

CHistoryTagManager &CHistoryTagManager::operator =(const CHistoryTagManager &other)
{
    if(&other == this)
        return *this;
    this->m_projectPath = other.m_projectPath;
    this->m_itemPosMap = other.m_itemPosMap;
    if(!m_tagList.isEmpty())
        qDeleteAll(m_tagList.begin(),m_tagList.end());
    m_tagList.clear();

    for(int i = 0; i < other.m_tagList.count();i++)
    {
        CFeatureTag *feature = other.m_tagList.at(i);
        CFeatureTag *tFeature = new CFeatureTag;
        *tFeature = *feature;
        m_tagList.append(tFeature);
    }

    CTagManager::operator =(other);
    return *this;
}

void CHistoryTagManager::changeLanguage()
{
    QListIterator<CFeatureTag*> iter(m_tagList);
    while(iter.hasNext())
    {
        iter.next()->changeLanguage();
    }
}

void CHistoryTagManager::collectWords()
{
    QListIterator<CFeatureTag*> iter(m_tagList);
    while(iter.hasNext())
    {
        iter.next()->collectWords();
    }
}

