#include "CFeatureTag.h"
#include "CFeatureTagItem.h"
#include "CFeatureTagTimeItem.h"
#include "CFeatureTagValueItem.h"
#include "CFeatureTagCommentItem.h"
#include "CXAxis.h"

#include "CFeatureTagLine.h"
#include "CPoint.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

CFeatureTag::CFeatureTag():
    m_internal(5),
    m_xPoint(0),
    m_visible(true),
    m_tagLine(new CFeatureTagLine),
    m_topPoint(new CPoint),
    m_bottomPoint(new CPoint)
{
    m_topPoint->setVisible(true);
    m_bottomPoint->setVisible(true);
    CFeatureTagItem *commentItem = new CFeatureTagCommentItem;
    CFeatureTagItem *timeItem = new CFeatureTagTimeItem;
    CFeatureTagItem *valueItem = new CFeatureTagValueItem;

    m_itemList.append(commentItem);
    m_itemList.append(timeItem);
    m_itemList.append(valueItem);
}

CFeatureTag::~CFeatureTag()
{
    clearItemList();
    if(m_tagLine)
    {
        delete m_tagLine;
        m_tagLine = NULL;
    }

    if(m_topPoint)
    {
        delete m_topPoint;
        m_topPoint = NULL;
    }

    if(m_bottomPoint){
        delete m_bottomPoint;
        m_bottomPoint = NULL;
    }
}

CFeatureTagItem *CFeatureTag::item(const CFeatureTagItem::ItemType &type) const
{
    for(int i = 0; i < m_itemList.count();i++){
        CFeatureTagItem *item = m_itemList.at(i);
        if(type == item->type())
            return item;
    }
    return NULL;
}

QList<CFeatureTagItem *> &CFeatureTag::itemList()
{
    return m_itemList;
}

void CFeatureTag::setItemList(const QList<CFeatureTagItem *> &itemList)
{
    m_itemList = itemList;
}

double CFeatureTag::totalHeight()
{
    double total =0;
    for(int i = 0; i < m_itemList.count();i++)
    {
        CFeatureTagItem *item = m_itemList.at(i);
        if(item->visible())
        {
            total += item->textHeight();
            total += m_internal;
        }
    }
    total -= m_internal;
    return {total > 0 ? total : 0};
}

void CFeatureTag::setDrawArea(const QRectF &drawArea)
{
    m_drawArea = drawArea;
}

void CFeatureTag::setViewName(const QString &viewName)
{
    m_viewName = viewName;
}

QString CFeatureTag::viewName() const
{
    return m_viewName;
}

void CFeatureTag::setTimeVariant(const QString &timeVariant)
{
    m_timeVariant = timeVariant;
}

QString CFeatureTag::timeVariant() const
{
    return m_timeVariant;
}

void CFeatureTag::setProjectPath(const QString &path)
{
    m_projectPath = path;
}

QString CFeatureTag::projectPath() const
{
    return m_projectPath;
}

CFeatureTagLine *CFeatureTag::tagLine()
{
    return m_tagLine;
}

CPoint *CFeatureTag::topPoint()
{
    return m_topPoint;
}

CPoint *CFeatureTag::bottomPoint()
{
    return m_bottomPoint;
}

void CFeatureTag::clearOldData()
{
    m_tagLine->clearLinePoints();
    m_topPoint->clearPath();
    m_bottomPoint->clearPath();
}

double CFeatureTag::internal() const
{
    return m_internal;
}

void CFeatureTag::setInternal(double internal)
{
    m_internal = internal;
}

double CFeatureTag::xPoint() const
{
    return m_xPoint;
}

void CFeatureTag::setXPoint(double xPoint)
{
    m_xPoint = xPoint;
}

CFeatureTagItem *CFeatureTag::timeItem() const
{
    for(int i = 0; i < m_itemList.count();i++){
        CFeatureTagItem *item = m_itemList.at(i);
        if(CFeatureTagItem::ItemType::Time == item->type())
            return item;
    }
    return NULL;
}

QString CFeatureTag::xName() const
{
    return m_xName;
}

void CFeatureTag::setXName(const QString &xName)
{
    m_xName = xName;
}

CXAxis *CFeatureTag::xAxis() const
{
    return m_xAxis;
}

void CFeatureTag::setXAxis(CXAxis *xAxis)
{
    m_xAxis = xAxis;
}

QString CFeatureTag::name() const
{
    return m_name;
}

void CFeatureTag::setName(const QString &name)
{
    m_name = name;
}

bool CFeatureTag::visible() const
{
    return m_visible;
}

void CFeatureTag::setVisible(bool visible)
{
    m_visible = visible;
}

void CFeatureTag::setComment(const QString &commnet)
{
    CFeatureTagItem *it = item(CFeatureTagItem::ItemType::Comment);
    if(it)
    {
       it->setComment(commnet);
    }
}

QString CFeatureTag::comment() const
{
    CFeatureTagItem *it = item(CFeatureTagItem::ItemType::Comment);
    if(it)
    {
       return it->comment();
    }
    return QString();
}

void CFeatureTag::setValue(double value)
{
    CFeatureTagItem *it = item(CFeatureTagItem::ItemType::Value);
    if(it)
    {
        it->setValue(value);
    }
}

void CFeatureTag::setTime(const QDateTime &time)
{
    CFeatureTagItem *it = item(CFeatureTagItem::ItemType::Time);
    if(it)
    {
        it->setDateTime(time);
    }
}

void CFeatureTag::calculate(double areaHeight, double xPoint,const CTagPos &pos)
{
    double yStart = m_topPoint->size()/2;
    double yEnd = areaHeight - m_bottomPoint->size()/2;

    CTagHPos hPos = pos.hPos();
    CTagVPos vPos = pos.vPos();
    double vOffset = vPos.offset();
    double hOffset = hPos.offset();
    double yHeight = areaHeight;
    double itemTotalHeight = totalHeight();
    double itemInternal = 0;
    QPointF point;

    for(int i = 0; i < m_itemList.count();i++)
    {
        CFeatureTagItem *item = m_itemList.at(i);
        if(!item->visible())
            continue;

        double itemHeight = item->textHeight();

        switch (hPos.position())
        {
        case CTagHPos::Position::Left:
        case CTagHPos::Position::Right:
            switch (vPos.position())
            {
            case CTagVPos::Position::Top:
                point.setX(xPoint+hOffset);
                point.setY(vOffset + itemInternal);
                break;
            case CTagVPos::Position::Bottom:
                point.setX(xPoint+hOffset);
                point.setY(yHeight - itemTotalHeight-vOffset+itemInternal);
                break;
            case CTagVPos::Position::Middle: // 传递的值为中间点坐标
                point.setX(xPoint);
                point.setY(yHeight/2 - itemTotalHeight/2 + itemInternal);
                break;
            default:
                break;
            }
            break;
        case CTagHPos::Position::Middle:
            switch (vPos.position())
            {
            case CTagVPos::Position::Top:
                point.setX(xPoint);
                point.setY(vOffset+itemInternal);
                break;
            case CTagVPos::Position::Bottom:
                point.setX(xPoint);
                point.setY(yHeight - itemTotalHeight-vOffset+itemInternal);
                break;
            case CTagVPos::Position::Middle: // 传递的值为中间点坐标
                point.setX(xPoint);
                point.setY(yHeight/2 - itemTotalHeight/2 +itemInternal);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        itemInternal += itemHeight + m_internal;

        item->setPoint(point);
        item->setHPos(hPos);
        item->setVPos(vPos);

        item->calculate();
    }

    m_tagLine->appendLinePoints(QPointF(xPoint,yStart),QPointF(xPoint,yEnd));
    m_topPoint->figurePoint(QPointF(xPoint,yStart));
    m_bottomPoint->figurePoint(QPointF(xPoint,yEnd));
}

void CFeatureTag::draw(QPainter *p)
{
    for(int i = 0; i < m_itemList.count();i++)
    {
        CFeatureTagItem *item = m_itemList.at(i);
        item->drawItem(p);
    }
    m_tagLine->draw(p);
    m_topPoint->draw(p);
    m_bottomPoint->draw(p);
}

QPointF CFeatureTag::topLeft() const
{
    return m_topLeft;
}

void CFeatureTag::setTopLeft(const QPointF &topLeft)
{
    m_topLeft = topLeft;
}

void CFeatureTag::serialize(QJsonObject &json)
{
    json.insert("Internal",m_internal);
    json.insert("XName",m_xName);
    json.insert("Name",m_name);
    json.insert("Visible",m_visible);
    json.insert("ViewName",m_viewName);
    json.insert("TimeVariant",m_timeVariant);
    json.insert("ProjectPath",m_projectPath);
    QJsonArray array;
    int count = m_itemList.count();
    for(int i = 0 ; i <count; i++)
    {
        QJsonObject itemJson;
        CFeatureTagItem *item = m_itemList.at(i);
        item->serialize(itemJson);
        array.append(itemJson);
    }
    json.insert("ItemList",array);
    QJsonObject tagLineJson;
    m_tagLine->serialize(tagLineJson);
    json.insert("TagLine",tagLineJson);
    QJsonObject topJson;
    m_topPoint->serialize(topJson);
    json.insert("TopPoint",topJson);
    QJsonObject bottomJson;
    m_bottomPoint->serialize(bottomJson);
    json.insert("BottomPoint",bottomJson);
}

void CFeatureTag::deserialize(const QJsonObject &json)
{
    clearItemList();
    m_internal = json.value("Internal").toDouble();
    m_xName = json.value("XName").toString();
    m_name = json.value("Name").toString();
    m_visible = json.value("Visible").toBool();
    m_viewName = json.value("ViewName").toString();
    m_timeVariant = json.value("TimeVariant").toString();
    m_projectPath = json.value("ProjectPath").toString();
    QJsonArray array = json.value("ItemList").toArray();
    int count = array.count();
    for(int i = 0 ; i <count; i++)
    {
        QJsonObject itemJson = array.at(i).toObject();
        int type = itemJson.value("Type").toInt();
        switch ((CFeatureTagItem::ItemType)type)
        {
        case CFeatureTagItem::ItemType::Time:
        {
            CFeatureTagTimeItem *timeItem = new CFeatureTagTimeItem;
            timeItem->deserialize(itemJson);
            m_itemList.append(timeItem);
            break;
        }
        case CFeatureTagItem::ItemType::Comment:
        {
            CFeatureTagCommentItem *commentItem = new CFeatureTagCommentItem;
            commentItem->deserialize(itemJson);
            m_itemList.append(commentItem);
            break;
        }
        case  CFeatureTagItem::ItemType::Value:
        {
            CFeatureTagValueItem *valueItem = new CFeatureTagValueItem;
            valueItem->deserialize(itemJson);
            m_itemList.append(valueItem);
            break;
        }
        default:
            break;
        }
    }
    QJsonObject tagLineJson = json.value("TagLine").toObject();
    m_tagLine->deserialize(tagLineJson);
    QJsonObject topJson = json.value("TopPoint").toObject();
    m_topPoint->deserialize(topJson);
    QJsonObject bottomJson = json.value("BottomPoint").toObject();
    m_bottomPoint->deserialize(bottomJson);

}

CFeatureTag &CFeatureTag::operator =(const CFeatureTag &tag)
{
    if(this == &tag)
        return *this;
    this->m_internal = tag.m_internal;

    this->m_xName = tag.m_xName;
    this->m_name = tag.m_name;
    this->m_visible = tag.m_visible;
    this->m_viewName = tag.m_viewName;
    this->m_timeVariant = tag.m_timeVariant;
    this->m_projectPath = tag.m_projectPath;

    *this->m_topPoint = *tag.m_topPoint;
    *this->m_bottomPoint = *tag.m_bottomPoint;
    *this->m_tagLine = *tag.m_tagLine;

    clearItemList();

    for(int i = 0; i < tag.m_itemList.count();i++)
    {
        CFeatureTagItem *item = tag.m_itemList.at(i);
        switch (item->type())
        {
        case CFeatureTagItem::ItemType::Comment:
        {
            CFeatureTagCommentItem *newItem  = new CFeatureTagCommentItem;
            *newItem = *(static_cast<CFeatureTagCommentItem*>(item));
            m_itemList.append(newItem);
            break;
        }
        case CFeatureTagItem::ItemType::Time:
        {
            CFeatureTagTimeItem *newItem  = new CFeatureTagTimeItem;
            *newItem = *(static_cast<CFeatureTagTimeItem*>(item));
            m_itemList.append(newItem);
            break;
        }
        case CFeatureTagItem::ItemType::Value:
        {
            CFeatureTagValueItem *newItem  = new CFeatureTagValueItem;
            *newItem = *(static_cast<CFeatureTagValueItem*>(item));
            m_itemList.append(newItem);
            break;
        }
        default:
            break;
        }
    }
    return *this;
}

void CFeatureTag::clearItemList()
{
    if(!m_itemList.isEmpty())
        qDeleteAll(m_itemList.begin(),m_itemList.end());
    m_itemList.clear();
}

void CFeatureTag::changeLanguage()
{
    QListIterator<CFeatureTagItem*> iter(m_itemList);
    while(iter.hasNext()){
        iter.next()->changeLanguage();
    }
}

void CFeatureTag::collectWords()
{
    QListIterator<CFeatureTagItem*> iter(m_itemList);
    while(iter.hasNext()){
        iter.next()->collectWords();
    }
}

