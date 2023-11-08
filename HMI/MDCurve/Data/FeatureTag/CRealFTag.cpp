#include "CRealFTag.h"
#include "CFeatureTagTimeItem.h"
#include "CFeatureTagValueItem.h"
#include "CFeatureTagCommentItem.h"

#include "CFeatureTagLine.h"
#include "CPoint.h"

#include <QPainter>
#include <QJsonArray>
#include <QJsonObject>
#include "CTranslator.h"
#include "CXAxis.h"
#include "MDJsonConvert.h"
//#include <sys/time.h>
#include <time.h>
#include <QDebug>
using namespace Translator;

CRealFTag::CRealFTag():
    m_visible(true),
    m_internal(5),
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

CRealFTag::~CRealFTag()
{
    clearItemList();
    if(m_tagLine){
        delete m_tagLine;
        m_tagLine = NULL;
    }

    if(m_topPoint){
        delete m_topPoint;
        m_topPoint = NULL;
    }

    if(m_bottomPoint){
        delete m_bottomPoint;
        m_bottomPoint = NULL;
    }
}

CFeatureTagItem *CRealFTag::item(const CFeatureTagItem::ItemType &type) const
{
    for(int i = 0; i < m_itemList.count();i++){
        CFeatureTagItem *item = m_itemList.at(i);
        if(type == item->type())
            return item;
    }
    return NULL;
}

void CRealFTag::clearItemList()
{
    if(!m_itemList.isEmpty())
        qDeleteAll(m_itemList.begin(),m_itemList.end());
    m_itemList.clear();
}

void CRealFTag::changeLanguage()
{
    QListIterator<CFeatureTagItem*> iter(m_itemList);
    while(iter.hasNext()){
        iter.next()->changeLanguage();
    }
}

void CRealFTag::collectWords()
{
    QListIterator<CFeatureTagItem*> iter(m_itemList);
    while(iter.hasNext()){
        iter.next()->collectWords();
    }
}

void CRealFTag::clearOldData()
{
    m_tagLine->clearLinePoints();
    m_topPoint->clearPath();
    m_bottomPoint->clearPath();
}

bool CRealFTag::visible() const
{
    return m_visible;
}

void CRealFTag::setVisible(bool visible)
{
    m_visible = visible;
}


double CRealFTag::internal() const
{
    return m_internal;
}

void CRealFTag::setInternal(double internal)
{
    enum {Min = 1};
    if(internal < Min)
        internal = Min;
    m_internal = internal;
}

CRealFTag &CRealFTag::operator =(const CRealFTag &other)
{
    if(this == &other)
        return *this;
    this->m_visible = other.m_visible;
    this->m_name = other.m_name;
    this->m_font = other.m_font;
    this->m_topLeft = other.m_topLeft;
    this->m_internal = other.m_internal;
    this->m_xName = other.m_xName;

    *this->m_topPoint = *other.m_topPoint;
    *this->m_bottomPoint = *other.m_bottomPoint;
    *this->m_tagLine = *other.m_tagLine;


    clearItemList();

    for(int i = 0; i < other.m_itemList.count();i++){
        CFeatureTagItem *item = other.m_itemList.at(i);
        switch (item->type()) {
        case CFeatureTagItem::ItemType::Comment: {
            CFeatureTagCommentItem *newItem  = new CFeatureTagCommentItem;
            *newItem = *(static_cast<CFeatureTagCommentItem*>(item));
            m_itemList.append(newItem);
            break;
        }
        case CFeatureTagItem::ItemType::Time: {
            CFeatureTagTimeItem *newItem  = new CFeatureTagTimeItem;
            *newItem = *(static_cast<CFeatureTagTimeItem*>(item));
            m_itemList.append(newItem);
            break;
        }
        case CFeatureTagItem::ItemType::Value: {
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

void CRealFTag::serialize(QJsonObject &json)
{
    json.insert("Name",m_name);
    json.insert("Visible",m_visible);
    json.insert("Font",CMDJsonConvert::toJson(m_font));
    json.insert("Internal",m_internal);
    json.insert("XName",m_xName);
    int count = m_itemList.count();
    QJsonArray array;
    for(int i = 0; i<count;i++)
    {
        QJsonObject listJson;
        CFeatureTagItem *item = m_itemList.at(i);
        item->serialize(listJson);
        array.append(listJson);
    }
    json.insert("ItemList",array);
    QJsonObject tagLineJson;
    m_tagLine->serialize(tagLineJson);
    json.insert("TagLine",tagLineJson);
    QJsonObject topPointJson;
    m_topPoint->serialize(topPointJson);
    json.insert("TopPoint",topPointJson);
    QJsonObject bottomPoint;
    m_bottomPoint->serialize(bottomPoint);
    json.insert("BottomPoint",bottomPoint);

}

void CRealFTag::deserialize(const QJsonObject &json)
{
    clearItemList();
    m_name = json.value("Name").toString();
    m_visible = json.value("Visible").toBool();
    m_font =CMDJsonConvert::toQFont(json.value("Font"));
    m_internal = json.value("Internal").toDouble();
    m_xName = json.value("XName").toString();
    QJsonArray array = json.value("ItemList").toArray();
    int count = array.count();
    for(int i =0; i<count; i++)
    {
        QJsonObject itemJson = array.at(i).toObject();
        int type = itemJson.value("Type").toInt();
        switch ((CFeatureTagItem::ItemType)type) {
        case CFeatureTagItem::ItemType::Time:{
            CFeatureTagTimeItem *timeItem = new CFeatureTagTimeItem;
            timeItem->deserialize(itemJson);
            m_itemList.append(timeItem);
            break;
        }
        case CFeatureTagItem::ItemType::Comment: {
            CFeatureTagCommentItem *commentItem = new CFeatureTagCommentItem;
            commentItem->deserialize(itemJson);
            m_itemList.append(commentItem);
            break;
        }
        case  CFeatureTagItem::ItemType::Value: {
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
    QJsonObject topPointJson = json.value("TopPoint").toObject();
    m_topPoint->deserialize(topPointJson);
    QJsonObject bottomPointJson = json.value("BottomPoint").toObject();
    m_bottomPoint->deserialize(bottomPointJson);
}

QString CRealFTag::name() const
{
    return m_name;
}

void CRealFTag::setName(const QString &name)
{
    m_name = name;
}

QPointF CRealFTag::topLeft() const
{
    return m_topLeft;
}

void CRealFTag::setTopLeft(const QPointF &topLeft)
{
    m_topLeft = topLeft;
}

QFont CRealFTag::font() const
{
    return m_font;
}

void CRealFTag::setFont(const QFont &font)
{
    m_font = font;
    for(int i = 0; i < m_itemList.count();i++){
        CFeatureTagItem *item = m_itemList.at(i);
        item->setFont(font);
    }
}

QList<CFeatureTagItem *> CRealFTag::itemList() const
{
    return m_itemList;
}

void CRealFTag::setItemList(const QList<CFeatureTagItem *> &itemList)
{
    m_itemList = itemList;
}

double CRealFTag::totalHeight()
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

void CRealFTag::setValue(double value)
{
    CFeatureTagItem *it = item(CFeatureTagItem::ItemType::Value);
    if(it){
        it->setValue(value);
    }
}

void CRealFTag::setTime(const QDateTime &time)
{
    CFeatureTagItem *it = item(CFeatureTagItem::ItemType::Time);
    if(it){
        it->setDateTime(time);
    }
}

void CRealFTag::setComment(const QString &commnet)
{
    CFeatureTagItem *it = item(CFeatureTagItem::ItemType::Comment);
    if(it){
       it->setComment(commnet);
    }
}

QString CRealFTag::comment() const
{
    CFeatureTagItem *it = item(CFeatureTagItem::ItemType::Comment);
    if(it){
       return it->comment();
    }
    return QString();
}

void CRealFTag::setFormat(const QString &format)
{
    CFeatureTagItem *it = item(CFeatureTagItem::ItemType::Value);
    if(it){
       it->setFormat(format);
    }
}

QString CRealFTag::format() const
{
    CFeatureTagItem *it = item(CFeatureTagItem::ItemType::Value);
    if(it){
       return it->format();
    }
    return QString();
}

QString CRealFTag::xName() const
{
    return m_xName;
}

void CRealFTag::setXName(const QString &xName)
{
    m_xName = xName;
}

CXAxis *CRealFTag::xAxis() const
{
    return m_xAxis;
}

void CRealFTag::setXAxis(CXAxis *xAxis)
{
    m_xAxis = xAxis;
}

CFeatureTagLine *CRealFTag::tagLine()
{
    return m_tagLine;
}

CPoint *CRealFTag::topPoint()
{
    return m_topPoint;
}

CPoint *CRealFTag::bottomPoint()
{
    return m_bottomPoint;
}

void CRealFTag::calclualte(double areaHeight, double xPoint, const CTagPos &pos)
{

    double startTime = (double)clock();
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

    for(int i = 0; i < m_itemList.count();i++) {
        CFeatureTagItem *item = m_itemList.at(i);
        if(!item->visible())
            continue;

        double itemHeight = item->textHeight();

        switch (hPos.position()) {
        case CTagHPos::Position::Left:
        case CTagHPos::Position::Right:
            switch (vPos.position()) {
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
            switch (vPos.position()) {
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
    double endTime = (double)clock();
//    qDebug()<<"tagCal:"<<(endTime - startTime) /1000;
}

void CRealFTag::draw(QPainter *p)
{
    p->setPen(QColor(Qt::black));
    for(int i = 0; i < m_itemList.count();i++)
    {
        CFeatureTagItem *tagItem = m_itemList.at(i);
        tagItem->drawItem(p);
    }

    m_tagLine->draw(p);
    m_topPoint->draw(p);
    m_bottomPoint->draw(p);
}

