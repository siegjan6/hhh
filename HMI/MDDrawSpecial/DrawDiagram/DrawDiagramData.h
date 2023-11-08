#ifndef DRAWGDIAGRAMDATA_H
#define DRAWGDIAGRAMDATA_H

#include "MDDiagram.h"

//报表控件,数据类
class CDrawDiagramData
{
public:
    CDrawDiagramData();
    ~CDrawDiagramData();
public:
    CMDDiagram m_diagram;
//////////////////////////////////////////////////////////////////////////
//common
public:
    CDrawDiagramData&  operator=(const CDrawDiagramData& other);

public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

};

#endif // DRAWGDIAGRAMDATA_H
