/*!
\author aGRw 2015.05.10
\section 弧线数据类
*/
#ifndef DRAWARCDATA_H
#define DRAWARCDATA_H

#include <QList>
#include <QPointF>
#include <QJsonObject>
#include "DrawObjDefine.h"

class CDrawBasicArcData
{
public:
    CDrawBasicArcData();
public:
    qreal m_startAngle;
    qreal m_spanAngle;
    qreal m_startAngleBk;
    qreal m_spanAngleBk;

    //!custom edit
    QList<QPointF> m_customDatas;
    QPointF m_customCenter;
/*!
\section common
*/
public:
    void operator=(const CDrawBasicArcData& other);
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
};

#endif // DRAWARCDATA_H
