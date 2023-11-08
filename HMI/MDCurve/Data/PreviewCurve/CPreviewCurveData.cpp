#include "CPreviewCurveData.h"
#include <QJsonObject>

CPreviewCurveData::CPreviewCurveData()
{
    m_version = 1;
    m_isPreviewVisible = true;

    m_previewBackBrush.init(QColor(192,192,192),QColor(255,255,255),270);
    QColor color = QColor(192,192,192);
    color.setAlpha(100);
    m_previewHightLightBrush.init(color);

    m_mainBisectrixColor = QColor(120,120,120);
    m_subBisectrixColor = QColor(120,120,120);
    m_startBackColor = QColor(154,154,154);
    m_startScaleColor = QColor(0,0,0);
    m_startLineMarkColor= QColor(0,0,0);

    m_endBackColor = QColor(154,154,154);
    m_endScaleColor= QColor(0,0,0);
    m_endLineMarkColor= QColor(0,0,0);

    m_mainBisectrix = 10;
    m_subBisectrix = 4;
    m_subBisectrixheight = 5;

    /*!
    \section 滚动条
    */
    m_rollBackBrush.init(QColor(192,192,192),QColor(180,180,180),270);
    m_rollMoveBackBrush.init(QColor(128,128,128),QColor(255,255,255),90);
    m_rollStartBackBrush.init(QColor(200,200,200),QColor(180,180,180),270);
    m_rollEndBackBrush.init(QColor(200,200,200),QColor(180,180,180),270);
    m_rollScaleColor = QColor(0,0,0);
    m_rollStartArrowsColor = QColor(128,128,128);
    m_rollEndArrowsColor = QColor(128,128,128);

    /*!
    \section 曲线与标签
    */
    m_isCurveVisible = true;
    m_boundedCurve = "";
}

CPreviewCurveData::~CPreviewCurveData()
{
}

bool CPreviewCurveData::isPreviewVisible() const
{
    return m_isPreviewVisible;
}

void CPreviewCurveData::setPreviewVisible(bool visible)
{
    m_isPreviewVisible = visible;
}

void CPreviewCurveData::setPreviewBackBrush(const CMDBrushManager &previewBackBrush)
{
    m_previewBackBrush = previewBackBrush;
}

CMDBrushManager CPreviewCurveData::previewBackBrush() const
{
    return m_previewBackBrush;
}

void CPreviewCurveData::setPreviewHightLightBrush(const CMDBrushManager &previewHightLightBrush)
{
    m_previewHightLightBrush = previewHightLightBrush;
}

CMDBrushManager CPreviewCurveData::previewHightLightBrush() const
{
    return m_previewHightLightBrush;
}

void CPreviewCurveData::setMainBisectrixColor(const QColor &mainBisectrixColor)
{
    m_mainBisectrixColor = mainBisectrixColor;
}

QColor CPreviewCurveData::mainBisectrixColor() const
{
    return m_mainBisectrixColor;
}

void CPreviewCurveData::setSubBisectrixColor(const QColor &subBisectrixColor)
{
    m_subBisectrixColor = subBisectrixColor;
}

QColor CPreviewCurveData::subBisectrixColor() const
{
    return m_subBisectrixColor;
}

void CPreviewCurveData::setStartBackColor(const QColor &startBackColor)
{
    m_startBackColor = startBackColor;
}

QColor CPreviewCurveData::startBackColor() const
{
    return m_startBackColor;
}

void CPreviewCurveData::setStartScaleColor(const QColor &startScaleColor)
{
    m_startScaleColor = startScaleColor;
}

QColor CPreviewCurveData::startScaleColor() const
{
    return m_startScaleColor;
}

void CPreviewCurveData::setStartLineMarkColor(const QColor &startLineMarkColor)
{
    m_startLineMarkColor = startLineMarkColor;
}

QColor CPreviewCurveData::startLineMarkColor() const
{
    return m_startLineMarkColor;
}

void CPreviewCurveData::setEndBackColor(const QColor &endBackColor)
{
    m_endBackColor = endBackColor;
}

QColor CPreviewCurveData::endBackColor() const
{
    return m_endBackColor;
}

void CPreviewCurveData::setEndScaleColor(const QColor &endScaleColor)
{
    m_endScaleColor = endScaleColor;
}

QColor CPreviewCurveData::endScaleColor() const
{
    return m_endScaleColor;
}

void CPreviewCurveData::setEndLineMarkColor(const QColor &endLineMarkColor)
{
    m_endLineMarkColor = endLineMarkColor;
}

QColor CPreviewCurveData::endLineMarkColor() const
{
    return m_endLineMarkColor;
}

void CPreviewCurveData::setMainBisectrix(const int &mainCount)
{
    m_mainBisectrix = mainCount;
}

int CPreviewCurveData::mainBisectrix() const
{
    return m_mainBisectrix;
}

void CPreviewCurveData::setSubBisectrix(const int &subCount)
{
    m_subBisectrix = subCount;
}

int CPreviewCurveData::subBisectrix() const
{
    return m_subBisectrix;
}

void CPreviewCurveData::setSubBisectrixHeight(const int &height)
{
    m_subBisectrixheight = height;
}

int CPreviewCurveData::subBisectrixHeight() const
{
    return m_subBisectrixheight;
}

void CPreviewCurveData::setRollBackBrush(const CMDBrushManager &rollBackBrush)
{
    m_rollBackBrush = rollBackBrush;
}

CMDBrushManager CPreviewCurveData::rollBackBrush() const
{
    return m_rollBackBrush;
}

void CPreviewCurveData::setRollMoveBackBrush(const CMDBrushManager &rollMoveBackBrush)
{
    m_rollMoveBackBrush = rollMoveBackBrush;
}

CMDBrushManager CPreviewCurveData::rollMoveBackBrush() const
{
    return m_rollMoveBackBrush;
}

void CPreviewCurveData::setRollStartBackBrush(const CMDBrushManager &rollStartBackBrush)
{
    m_rollStartBackBrush = rollStartBackBrush;
}

CMDBrushManager CPreviewCurveData::rollStartBackBrush() const
{
    return m_rollStartBackBrush;
}

void CPreviewCurveData::setRollEndBackBrush(const CMDBrushManager &rollEndBackBrush)
{
    m_rollEndBackBrush = rollEndBackBrush;
}

CMDBrushManager CPreviewCurveData::rollEndBackBrush() const
{
    return m_rollEndBackBrush;
}

void CPreviewCurveData::setRollScaleColor(const QColor &rollScaleColor)
{
    m_rollScaleColor = rollScaleColor;
}

QColor CPreviewCurveData::rollScaleColor() const
{
    return m_rollScaleColor;
}

void CPreviewCurveData::setRollStartArrowsColor(const QColor &rollStartArrowsColor)
{
    m_rollStartArrowsColor = rollStartArrowsColor;
}

QColor CPreviewCurveData::rollStartArrowsColor() const
{
    return m_rollStartArrowsColor;
}

void CPreviewCurveData::setRollEndArrowsColor(const QColor &rollEndArrowsColor)
{
    m_rollEndArrowsColor = rollEndArrowsColor;
}

QColor CPreviewCurveData::rollEndArrowsColor() const
{
    return m_rollEndArrowsColor;
}

void CPreviewCurveData::setCurveVisible(bool visible)
{
   m_isCurveVisible = visible;
}

bool CPreviewCurveData::isCurveVisible() const
{
    return m_isCurveVisible;
}

void CPreviewCurveData::setBoundedCurve(const QString &string)
{
    m_boundedCurve = string;
}

QString CPreviewCurveData::boundedCurve() const
{
    return m_boundedCurve;
}

CPreviewCurveData &CPreviewCurveData::operator =(const CPreviewCurveData &other)
{
    if(&other == this)
        return *this;
    m_version = other.m_version;
    m_isPreviewVisible = other.m_isPreviewVisible;

    m_previewBackBrush = other.m_previewBackBrush;
    m_previewHightLightBrush = other.m_previewHightLightBrush;
    m_mainBisectrixColor = other.m_mainBisectrixColor;
    m_subBisectrixColor = other.m_subBisectrixColor;
    m_startBackColor = other.m_startBackColor;
    m_startScaleColor = other.m_startScaleColor;
    m_startLineMarkColor= other.m_startLineMarkColor;

    m_endBackColor = other.m_endBackColor;
    m_endScaleColor= other.m_endScaleColor;
    m_endLineMarkColor= other.m_endLineMarkColor;
    m_mainBisectrix = other.m_mainBisectrix;
    m_subBisectrix = other.m_subBisectrix;
    m_subBisectrixheight = other.m_subBisectrixheight;

    m_rollBackBrush = other.m_rollBackBrush;
    m_rollMoveBackBrush = other.m_rollMoveBackBrush;
    m_rollStartBackBrush = other.m_rollStartBackBrush;
    m_rollEndBackBrush = other.m_rollEndBackBrush;
    m_rollScaleColor = other.m_rollScaleColor;
    m_rollStartArrowsColor = other.m_rollStartArrowsColor;
    m_rollEndArrowsColor = other.m_rollEndArrowsColor;

    m_isCurveVisible = other.m_isCurveVisible;
    m_boundedCurve = other.m_boundedCurve;

    return *this;
}

void CPreviewCurveData::serialize(QJsonObject &json)
{
    json.insert("IsPreviewVisible",m_isPreviewVisible);
    json.insert("MainBisectrixColor",(double)m_mainBisectrixColor.rgba());
    json.insert("SubBisectrixColor",(double)m_subBisectrixColor.rgba());
    json.insert("StartBackColor",(double)m_startBackColor.rgba());
    json.insert("StartScaleColor",(double)m_startScaleColor.rgba());
    json.insert("StartLineMarkColor",(double)m_startLineMarkColor.rgba());
    json.insert("EndBackColor",(double)m_endBackColor.rgba());
    json.insert("EndScaleColor",(double)m_endScaleColor.rgba());
    json.insert("EndLineMarkColor",(double)m_endLineMarkColor.rgba());
    json.insert("MainBisectrix",m_mainBisectrix);
    json.insert("SubBisectrix",m_subBisectrix);
    json.insert("SubBisectrixheight",m_subBisectrixheight);

    json.insert("RollScaleColor",(double)m_rollScaleColor.rgba());
    json.insert("RollStartArrowsColor",(double)m_rollStartArrowsColor.rgba());
    json.insert("RollEndArrowsColor",(double)m_rollEndArrowsColor.rgba());
    json.insert("IsCurveVisible",m_isCurveVisible);
    json.insert("BoundedCurve",m_boundedCurve);

    QJsonObject previewBackBrush;
    m_previewBackBrush.serialize(previewBackBrush);
    json.insert("PreviewBackBrush",previewBackBrush);
    QJsonObject previewHightLightBrush;
    m_previewHightLightBrush.serialize(previewHightLightBrush);
    json.insert("PreviewHightLightBrush",previewHightLightBrush);
    QJsonObject rollBackBrush;
    m_rollBackBrush.serialize(rollBackBrush);
    json.insert("RollBackBrush",rollBackBrush);
    QJsonObject rollMoveBackBrush;
    m_rollMoveBackBrush.serialize(rollMoveBackBrush);
    json.insert("RollMoveBackBrush",rollMoveBackBrush);
    QJsonObject rollStartBackBrush;
    m_rollStartBackBrush.serialize(rollStartBackBrush);
    json.insert("RollStartBackBrush",rollStartBackBrush);
    QJsonObject rollEndBackBrush;
    m_rollEndBackBrush.serialize(rollEndBackBrush);
    json.insert("RollEndBackBrush",rollEndBackBrush);
}


void CPreviewCurveData::deserialize(const QJsonObject &json)
{
    m_isPreviewVisible = json.value("IsPreviewVisible").toBool();
    m_mainBisectrixColor = QColor::fromRgba(json.value("MainBisectrixColor").toDouble());
    m_subBisectrixColor = QColor::fromRgba(json.value("SubBisectrixColor").toDouble());
    m_startBackColor = QColor::fromRgba(json.value("StartBackColor").toDouble());
    m_startScaleColor = QColor::fromRgba(json.value("StartScaleColor").toDouble());
    m_startLineMarkColor = QColor::fromRgba(json.value("StartLineMarkColor").toDouble());
    m_endBackColor = QColor::fromRgba(json.value("EndBackColor").toDouble());
    m_endScaleColor = QColor::fromRgba(json.value("EndScaleColor").toDouble());
    m_endLineMarkColor = QColor::fromRgba(json.value("EndLineMarkColor").toDouble());
    m_mainBisectrix = json.value("MainBisectrix").toInt();
    m_subBisectrix = json.value("SubBisectrix").toInt();
    m_subBisectrixheight = json.value("SubBisectrixheight").toInt();

    m_rollScaleColor = QColor::fromRgba(json.value("RollScaleColor").toDouble());
    m_rollStartArrowsColor = QColor::fromRgba(json.value("RollStartArrowsColor").toDouble());
    m_rollEndArrowsColor = QColor::fromRgba(json.value("RollEndArrowsColor").toDouble());
    m_isCurveVisible = json.value("IsCurveVisible").toBool();
    m_boundedCurve = json.value("BoundedCurve").toString();

    QJsonObject previewBackBrush = json.value("PreviewBackBrush").toObject();
    m_previewBackBrush.deserialize(previewBackBrush);
    QJsonObject previewHightLightBrush = json.value("PreviewHightLightBrush").toObject();
    m_previewHightLightBrush.deserialize(previewHightLightBrush);
    QJsonObject rollBackBrush = json.value("RollBackBrush").toObject();
    m_rollBackBrush.deserialize(rollBackBrush);
    QJsonObject rollMoveBackBrush = json.value("RollMoveBackBrush").toObject();
    m_rollMoveBackBrush.deserialize(rollMoveBackBrush);
    QJsonObject rollStartBackBrush = json.value("RollStartBackBrush").toObject();
    m_rollStartBackBrush.deserialize(rollStartBackBrush);
    QJsonObject rollEndBackBrush = json.value("RollEndBackBrush").toObject();
    m_rollEndBackBrush.deserialize(rollEndBackBrush);
}

void CPreviewCurveData::getBrushes(QList<CMDBrushManager *> &list)
{
    list.append(&m_previewBackBrush);
    list.append(&m_previewHightLightBrush);
    list.append(&m_rollBackBrush);
    list.append(&m_rollMoveBackBrush);
    list.append(&m_rollStartBackBrush);
    list.append(&m_rollEndBackBrush);
}
