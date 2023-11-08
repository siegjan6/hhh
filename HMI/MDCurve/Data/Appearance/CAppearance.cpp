#include "CAppearance.h"
#include "MDJsonConvert.h"
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QJsonObject>
#include <QBrush>

#define MARGIN 70 // 与widget定义相同
CAppearance::CAppearance():
    m_isShowBorder(false),
    m_header(new CHeader)
{
    // initialize the brush color as white which default is black
    m_widgetBrush.setColor(Qt::white);
    m_curveBrush.setColor(Qt::white);
}

CAppearance::~CAppearance()
{
    if(m_header)
    {
        delete m_header;
        m_header = NULL;
    }
}
CHeader *CAppearance::header() const
{
    return m_header;
}

bool CAppearance::isShowBorder() const
{
    return m_isShowBorder;
}

void CAppearance::setIsShowBorder(bool isShowBorder)
{
    m_isShowBorder = isShowBorder;
}


QRectF CAppearance::curveArea() const
{
    return m_curveArea;
}
void CAppearance::setCurveArea(const QRectF &curveArea)
{
    m_curveArea = curveArea;
}
CMDBrushManager CAppearance::curveBrush() const
{
    return m_curveBrush;
}

void CAppearance::setCurveBrush(const CMDBrushManager &curveBrush)
{
    m_curveBrush = curveBrush;
}

QRectF CAppearance::widgetArea() const
{
    return m_widgetArea;
}
void CAppearance::setWidgetArea(const QRectF &widgetArea)
{
    m_widgetArea = widgetArea;
}

CMDBrushManager CAppearance::widgetBrush() const
{
    return m_widgetBrush;
}

void CAppearance::setWidgetBrush(const CMDBrushManager &widgetBrush)
{
    m_widgetBrush = widgetBrush;
}

void CAppearance::draw(QPainter *p,const QRect &rect)
{
    p->save();
    QPainterPath path;
    path.addRect(m_widgetArea);
    m_widgetBrush.generate(m_widgetArea);
    m_widgetBrush.draw(p,m_widgetArea,&path);

    QPainterPath curvePath;
    curvePath.addRect(m_curveArea);
    m_curveBrush.generate(m_curveArea);
    m_curveBrush.draw(p,m_curveArea,&curvePath);

    if(m_isShowBorder)
        drawBorder(p,rect);
    m_header->draw(p);
    p->restore();
}

void CAppearance::drawBorder(QPainter *p, const QRect &rect)
{
    p->save();

//    p->translate(0,-MARGIN/2);
    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(2);
    p->setPen(pen);
    p->drawRect(rect);

    p->restore();
}

void CAppearance::calculate()
{
}

void CAppearance::serialize(QJsonObject &json)
{
    json.insert("WidgetArea",CMDJsonConvert::toJson(m_widgetArea));
    json.insert("CurveArea",CMDJsonConvert::toJson(m_curveArea));
    json.insert("IsShowBorder",m_isShowBorder);
    QJsonObject headerJson;
    m_header->serialize(headerJson);
    json.insert("Header",headerJson);
    QJsonObject widgetJson;
    m_widgetBrush.serialize(widgetJson);
    json.insert("WidgetBrush",widgetJson);
    QJsonObject curveJson;
    m_curveBrush.serialize(curveJson);
    json.insert("CurveBrush",curveJson);
}

void CAppearance::deserialize(const QJsonObject &json)
{
    m_widgetArea =CMDJsonConvert::toQRectF(json.value("WidgetArea"));
    m_curveArea =CMDJsonConvert::toQRectF(json.value("CurveArea"));
    m_isShowBorder = json.value("IsShowBolder").toBool();
    QJsonObject headerJson = json.value("Header").toObject();
    m_header->deserialize(headerJson);
    QJsonObject widgetJson = json.value("WidgetBrush").toObject();
    m_widgetBrush.deserialize(widgetJson);
    QJsonObject curveJson = json.value("CurveBrush").toObject();
    m_curveBrush.deserialize(curveJson);
}

CAppearance &CAppearance::operator=(const CAppearance &other)
{
    if(&other == this)
        return *this;
    this->m_widgetArea = other.m_widgetArea;
    this->m_curveArea = other.m_curveArea;
    this->m_isShowBorder = other.m_isShowBorder;

    *this->m_header = *other.m_header;
    this->m_widgetBrush = other.m_widgetBrush;
    this->m_curveBrush = other.m_curveBrush;
    return *this;
}

void CAppearance::changeLanguage()
{
    m_header->changeLanguage();
}

void CAppearance::collectWords()
{
    m_header->collectWords();
}

void CAppearance::getBrushes(QList<CMDBrushManager *> &list)
{
    list.append(&m_widgetBrush);
    list.append(&m_curveBrush);
}



