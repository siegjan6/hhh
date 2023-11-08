#include "CTagPosition.h"
#include <QJsonObject>
#define MAX 100
#define MIN 1
// CTagVPos

void CTagVPos::setOffset(int offset)
{
    if(offset > MAX)
        offset = MAX;
    else if(offset  < MIN)
        offset = MIN;

    switch (m_position) {
    case Position::Bottom:
        m_bottomOffset = offset;
        break;
    case Position::Top:
        m_topOffset = offset;
        break;
    case Position::Middle:
        break;
    default:
        break;
    }
}

int CTagVPos::offset() const
{
    switch (m_position) {
    case Position::Bottom:
      return  m_bottomOffset;
    case Position::Top:
      return  m_topOffset;
    case Position::Middle:
        return MIDDLE;
    default:
        return MIDDLE;
    }
}
CTagVPos::Position CTagVPos::position() const
{
    return m_position;
}

void CTagVPos::setPosition(const Position &position)
{
    m_position = position;
}

void CTagVPos::serialize(QJsonObject &json)
{
    json.insert("TopOffSet",m_topOffset);
    json.insert("BottomOffset",m_bottomOffset);
    json.insert("Position",(int)m_position);
}

void CTagVPos::deserialize(const QJsonObject &json)
{
    m_topOffset = json.value("TopOffSet").toInt();
    m_bottomOffset = json.value("BottomOffset").toInt();
    int position = json.value("Position").toInt();
    m_position = (Position)position;
}



void CTagHPos::setOffset(int offset)
{
    if(offset > MAX)
        offset = MAX;
    else if(offset  < MIN)
        offset = MIN;

    switch (m_position) {
    case Position::Left:
        m_leftOffset = offset;
        break;
    case Position::Right:
        m_rightOffset = offset; // 将右侧设为负值
        break;
    case Position::Middle:
        break;
    default:
        break;
    }
}

int CTagHPos::offset() const
{
    switch (m_position) {
    case Position::Left:
      return  m_leftOffset;
    case Position::Right:
      return  -m_rightOffset;
    case Position::Middle:
        return MIDDLE;
    default:
        return MIDDLE;
    }
}

CTagHPos::Position CTagHPos::position() const
{
    return m_position;
}

void CTagHPos::setPosition(const Position &position)
{
    m_position = position;
}

void CTagHPos::serialize(QJsonObject &json)
{
    json.insert("LeftOffSet",m_leftOffset);
    json.insert("RightOffset",m_rightOffset);
    json.insert("Position",(int)m_position);
}

void CTagHPos::deserialize(const QJsonObject &json)
{
    m_leftOffset = json.value("LeftOffSet").toInt();
    m_rightOffset = json.value("RightOffset").toInt();
    int position = json.value("Position").toInt();
    m_position = (Position)position;
}

CTagPos::CTagPos(const CTagPos &other)
{
    this->m_hPos = other.m_hPos;
    this->m_vPos = other.m_vPos;
}

CTagHPos CTagPos::hPos() const
{
    return m_hPos;
}

void CTagPos::setHPos(const CTagHPos &hPos)
{
    m_hPos = hPos;
}
CTagVPos CTagPos::vPos() const
{
    return m_vPos;
}

void CTagPos::setVPos(const CTagVPos &vPos)
{
    m_vPos = vPos;
}

CTagPos &CTagPos::operator =(const CTagPos &other)
{
    this->m_hPos = other.m_hPos;
    this->m_vPos = other.m_vPos;
    return *this;
}

void CTagPos::serialize(QJsonObject &json)
{
    QJsonObject hJson;
    m_hPos.serialize(hJson);
    json.insert("HPos",hJson);
    QJsonObject vJson;
    m_vPos.serialize(vJson);
    json.insert("VPos",vJson);
}

void CTagPos::deserialize(const QJsonObject &json)
{
    QJsonObject hJson = json.value("HPos").toObject();
    m_hPos.deserialize(hJson);
    QJsonObject vJson = json.value("VPos").toObject();
    m_vPos.deserialize(vJson);
}
