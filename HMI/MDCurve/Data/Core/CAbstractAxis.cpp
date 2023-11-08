#include "CAbstractAxis.h"
#include "CCurveObject.h"
#include <QString>
#include <QJsonObject>
#include "MDMultiLanguage.h"
#include "MDJsonConvert.h"

CAbstractAxis::CAbstractAxis():
    m_comment(QObject::tr("[t]")),
    m_valuePrecision(2),
    m_axisVisible(true),
    m_axisLineVisible(false),
    m_fontColor(Qt::black),
    m_endScale(1),
    m_startScale(0),
    m_axisLine(new CBisectrixLine)
{
    m_axisLine->setLineStyle(Qt::SolidLine);
}

CAbstractAxis::~CAbstractAxis()
{
    if(m_axisLine) {
        delete m_axisLine;
        m_axisLine = NULL;
    }
}

void CAbstractAxis::setName(const QString &name)
{
    if(m_name != name)
        m_name = name;
    setComment(name);
}

QString CAbstractAxis::name() const
{
    return m_name;
}

void CAbstractAxis::setComment(const QString &comment)
{
    m_comment = comment;
}
QString CAbstractAxis::comment() const
{
//    return m_comment;
    return m_translateComment.isEmpty()? m_comment:m_translateComment;
}

int CAbstractAxis::valuePrecision() const
{
    return m_valuePrecision;
}

void CAbstractAxis::setValuePrecision(int valuePrecision)
{
    enum { Min =0, Max = 5 };
    if(valuePrecision < Min )
        m_valuePrecision = Min;
    else if( valuePrecision > Max)
        m_valuePrecision = Max;
    else
        m_valuePrecision = valuePrecision;
}

void  CAbstractAxis::setAxisVisible(bool isShow)
{
    m_axisVisible = isShow;
}

bool  CAbstractAxis::axisVisible() const
{
    return m_axisVisible;
}

bool CAbstractAxis::axisLineVisible() const
{
    return m_axisLineVisible;
}

void CAbstractAxis::setAxisLineVisible(bool isShowAxisLine)
{
    m_axisLineVisible = isShowAxisLine;
}

QFont CAbstractAxis::font() const
{
    return m_font;
}

void CAbstractAxis::setFont(const QFont &font)
{
    m_font = font;
}
QColor CAbstractAxis::fontColor() const
{
    return m_fontColor;
}

void CAbstractAxis::setFontColor(const QColor &fontColor)
{
    m_fontColor = fontColor;
}

void CAbstractAxis::setScale(double start, double end)
{
    if(start < end && start >= 0){
        m_startScale = start;
        m_endScale = end;
    }
}
double CAbstractAxis::startScale() const
{
    return m_startScale;
}

void CAbstractAxis::setStartScale(double startScale)
{
    m_startScale = startScale;
}

double CAbstractAxis::endScale() const
{
    return m_endScale;
}

void CAbstractAxis::setEndScale(double endScale)
{
    m_endScale = endScale;
}


CBisectrixLine *CAbstractAxis::axisLine() const
{
    return m_axisLine;
}
/*!
    \section   曲线相关
*/
void CAbstractAxis::addCurve(CCurveObject *curve)
{
    m_curves.append(curve);
}

void CAbstractAxis::removeCurve(CCurveObject *curve)
{
    m_curves.removeOne(curve);
}

QList<CCurveObject *> CAbstractAxis::curves() const
{
    return m_curves;
}

void CAbstractAxis::setCurves(const QList<CCurveObject *> &curves)
{
    m_curves = curves;
}

bool CAbstractAxis::isBoundCurve() const
{
    return !m_curves.isEmpty();
}

bool CAbstractAxis::allBindCurvesInvisible() const
{
    if(!m_curves.isEmpty()){
//        QSetIterator<CCurveObject *> it(m_curves);
//        while(it.hasNext()){
//            if(it.next()->curveVisible())
//                return false;
//        }
        for(int i = 0; i <m_curves.count(); i++)
        {
            if(m_curves.at(i)->curveVisible())
                return false;
        }
    }
    return true;

}

/*!
    \section 序列化
*/
void CAbstractAxis::serialize(QJsonObject &json)
{
    json.insert("Name",m_name);
    json.insert("Comment",m_comment);
    json.insert("Font",CMDJsonConvert::toJson(m_font));
    json.insert("FontColor",(double)m_fontColor.rgba());
    json.insert("AxisVisible",m_axisLineVisible);
    json.insert("ValuePrecision",m_valuePrecision);
    json.insert("AxisLineVisible",m_axisLineVisible);
    QJsonObject axisJson;
    m_axisLine->serialize(axisJson);
    json.insert("AxisLine",axisJson);

}

void CAbstractAxis::deserialize(const QJsonObject &json)
{
    m_name = json.value("Name").toString();
    m_comment = json.value("Comment").toString();
    m_font = CMDJsonConvert::toQFont(json.value("Font"));
    m_fontColor = QColor::fromRgba(json.value("FontColor").toDouble());
    m_axisLineVisible = json.value("AxisVisible").toBool();
    m_valuePrecision = json.value("ValuePrecision").toInt();
    m_axisLineVisible = json.value("AxisLineVisible").toBool();
    QJsonObject axisJson = json.value("AxisLine").toObject();
    m_axisLine->deserialize(axisJson);
}

CAbstractAxis &CAbstractAxis::operator =(const CAbstractAxis &other)
{
    this->m_name  = other.m_name;
    this->m_comment = other.m_comment;
    this->m_font = other.m_font;
    this->m_fontColor = other.m_fontColor;
    this->m_axisVisible = other.m_axisVisible;
    this->m_axisLineVisible = other.m_axisLineVisible;
    this->m_valuePrecision = other.m_valuePrecision;
    *this->m_axisLine = *other.m_axisLine;
    return *this;
}

void CAbstractAxis::changeLanguage()
{
    m_translateComment = CMDMultiLanguage::instance()->value(m_comment,false);
}

void CAbstractAxis::collectWords()
{
    CMDMultiLanguage::instance()->addWord(m_comment,false);
}

