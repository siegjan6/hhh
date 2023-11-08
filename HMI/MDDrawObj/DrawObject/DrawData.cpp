#include "DrawData.h"
#include "MDJsonConvert.h"
//////////////////////////////////////////////////////////////////////////
CDataObj::CDataObj()
{
}
void CDataObj::serialize(QJsonObject &json)
{
    json.insert("Rect", CMDJsonConvert::toJson(m_rect));
}
void CDataObj::deserialize(const QJsonObject &json)
{
    m_rect = CMDJsonConvert::toQRectF(json.value("Rect"));
}
bool CDataObj::equal(CDataObj *other)
{
    return m_rect == other->m_rect;
}
//////////////////////////////////////////////////////////////////////////
CDataVector::CDataVector()
{
    m_rotateAngle = 0;
    m_shear = 0;
    m_rotatePointPos;
    m_rotatePoint = QPointF(0.5, 0.5);
    m_xScale = 1;
    m_yScale = 1;
    m_scalePoint = QPointF(0.5, 0.5);
    m_isFlipX = false;
    m_isFlipY = false;
}
void CDataVector::serialize(QJsonObject &json)
{
    json.insert("RotateAngle", m_rotateAngle);
    json.insert("Shear", m_shear);
    json.insert("RotatePoint", CMDJsonConvert::toJson(m_rotatePoint));
    json.insert("ScalePoint", CMDJsonConvert::toJson(m_scalePoint));
    json.insert("IsFlipX", m_isFlipX);
    json.insert("IsFlipY", m_isFlipY);
}
void CDataVector::deserialize(const QJsonObject &json)
{
    m_rotateAngle = json.value("RotateAngle").toDouble();
    m_shear = json.value("Shear").toDouble();
    m_rotatePoint = CMDJsonConvert::toQPointF(json.value("RotatePoint"));
    m_scalePoint = CMDJsonConvert::toQPointF(json.value("ScalePoint"));
    m_isFlipX = json.value("IsFlipX").toBool();
    m_isFlipY = json.value("IsFlipY").toBool();
}
bool CDataVector::equal(CDataVector *other)
{
    bool b = (m_rotateAngle == other->m_rotateAngle
            && m_shear == other->m_shear
            && m_rotatePoint == other->m_rotatePoint
            && m_xScale == other->m_xScale
            && m_yScale == other->m_yScale
            && m_scalePoint == other->m_scalePoint
            && m_isFlipX == other->m_isFlipX
            && m_isFlipY == other->m_isFlipY
            );

    return b;
}
//////////////////////////////////////////////////////////////////////////
CCalcData::CCalcData()
{
    m_state = ControlState::None;
}
void CCalcData::setState(ControlState value)
{
    m_state = value;

    switch (value)
    {
    case ControlState::XScale:
    case ControlState::YScale:
        m_fixRate = m_dataVector.scalePoint();
        break;
    case ControlState::Shear:
    case ControlState::Center:
    case ControlState::MultiRotate:
        m_fixRate = QPointF(0.5, 0.5);
        break;
    case ControlState::BoundMove:
        m_fixRate = QPointF(0, 0);
        break;
    default:
        break;
    }
    m_offset = QPointF(0, 0);
}
//////////////////////////////////////////////////////////////////////////

