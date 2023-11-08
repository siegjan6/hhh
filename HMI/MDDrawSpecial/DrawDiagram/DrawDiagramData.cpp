#include "DrawDiagramData.h"
//////////////////////////////////////////////////////////////////////////
CDrawDiagramData::CDrawDiagramData()
{
}

CDrawDiagramData::~CDrawDiagramData()
{
}

CDrawDiagramData&  CDrawDiagramData::operator=(const CDrawDiagramData &other)
{
    if(this == &other)
    {
        return *this;
    }
    m_diagram = other.m_diagram;
    return *this;
}
void CDrawDiagramData::serialize(QJsonObject &json)
{
    m_diagram.serialize(json);
}

void CDrawDiagramData::deserialize(const QJsonObject &json)
{
    m_diagram.deserialize(json);
}

//////////////////////////////////////////////////////////////////////////
