#include "CTagManager.h"
#include "CPoint.h"
#include "CFeatureTagLine.h"
#include "MDJsonConvert.h"
#include <QJsonObject>
#include <QJsonArray>

CTagManager::CTagManager():
    m_visible(false),
    m_interval(5),
    m_isStudioMode(false),
    m_referenceAxis(0),
    m_tagLine(new CFeatureTagLine),
    m_topPoint(new CPoint),
    m_bottomPoint(new CPoint),
    m_isCalculated(false),
    m_xs(NULL)
{
    m_topPoint->setVisible(true);
    m_bottomPoint->setVisible(true);
}

CTagManager::~CTagManager()
{
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

bool CTagManager::visible() const
{
    return m_visible;
}

void CTagManager::setVisible(bool visible)
{
    m_visible = visible;
}

QString CTagManager::comment() const
{
    return m_comment;
}

void CTagManager::setComment(const QString &comment)
{
    m_comment = comment;
}

qint32 CTagManager::interval() const
{
    return m_interval;
}

void CTagManager::setInterval(const qint32 &interval)
{
    m_interval = interval;
}

QFont CTagManager::font() const
{
    return m_font;
}

void CTagManager::setFont(const QFont &font)
{
    m_font = font;
}

CDataSource CTagManager::dataSource() const
{
    return m_dataSource;
}

void CTagManager::setDataSource(const CDataSource &dataSource)
{
    m_dataSource = dataSource;
}

QStringList CTagManager::fields() const
{
    return m_dataSource.fields();
}

void CTagManager::setFields(const QStringList &fields)
{
    m_dataSource.setFields(fields);
}

bool CTagManager::isStudioMode() const
{
    return m_isStudioMode;
}

void CTagManager::setIsStudioMode(bool isStudioMode)
{
    m_isStudioMode = isStudioMode;
}

QString CTagManager::reAxisName() const
{
    return m_reAxisName;
}

void CTagManager::setReAxisName(const QString &reAxisName)
{
    m_reAxisName = reAxisName;
}

CXAxis *CTagManager::referenceAxis() const
{
    return m_referenceAxis;
}

void CTagManager::setReferenceAxis(CXAxis *referenceAxis)
{
    m_referenceAxis = referenceAxis;
}

CXAxisManager *CTagManager::xs() const
{
    return m_xs;
}

void CTagManager::setXs(CXAxisManager *xs)
{
    m_xs = xs;
}
CFeatureTagLine *CTagManager::tagLine() const
{
    return m_tagLine;
}

CPoint *CTagManager::topPoint() const
{
    return m_topPoint;
}

CPoint *CTagManager::bottomPoint() const
{
    return m_bottomPoint;
}

bool CTagManager::isCalculated() const
{
    return m_isCalculated;
}

void CTagManager::setIsCalculated(bool isCalculated)
{
    m_isCalculated = isCalculated;
}

void CTagManager::serialize(QJsonObject &json)
{
    json.insert("Visible",m_visible);
    json.insert("Comment",m_comment);
    json.insert("Interval",m_interval);
    json.insert("Font",CMDJsonConvert::toJson(m_font));
    json.insert("ReAxisName",m_reAxisName);
    QJsonObject dataJson;
    m_dataSource.serialize(dataJson);
    json.insert("DataSource",dataJson);
    QJsonObject topJson;
    m_topPoint->serialize(topJson);
    json.insert("TopPoint",topJson);
    QJsonObject bottomJson;
    m_bottomPoint->serialize(bottomJson);
    json.insert("BottomPoint",bottomJson);
}

void CTagManager::deserialize(const QJsonObject &json)
{
    m_visible = json.value("Visible").toBool();
    m_comment = json.value("Comment").toString();
    m_interval = json.value("Interval").toInt();
    m_font =CMDJsonConvert::toQFont(json.value("Font"));
    m_reAxisName = json.value("ReAxisName").toString();
    QJsonObject dataJson = json.value("DataSource").toObject();
    m_dataSource.deserialize(dataJson);
    QJsonObject topJson = json.value("TopPoint").toObject();
    m_topPoint->deserialize(topJson);
    QJsonObject bottomJson = json.value("BottomPoint").toObject();
    m_bottomPoint->deserialize(bottomJson);

}

CTagManager &CTagManager::operator =(const CTagManager &other)
{
    if(this == &other)
        return *this;
    this->m_visible = other.m_visible;
    this->m_comment = other.m_comment;
    this->m_interval = other.m_interval;
    this->m_font = other.m_font;
    this->m_dataSource = other.m_dataSource;
    this->m_reAxisName = other.m_reAxisName;

    *this->m_topPoint = *other.m_topPoint;
    *this->m_bottomPoint = *other.m_bottomPoint;
    return *this;
}
CCurveDataManager &CTagManager::rawDataList()
{
    return m_rawDataList;
}

void CTagManager::setRawDataList(const CCurveDataManager &rawDataList)
{
    m_rawDataList = rawDataList;
}

void CTagManager::changeLanguage()
{

}

void CTagManager::collectWords()
{

}

QRectF CTagManager::drawArea() const
{
    return m_drawArea;
}

void CTagManager::setDrawArea(const QRectF &drawArea)
{
    m_drawArea = drawArea;
}

