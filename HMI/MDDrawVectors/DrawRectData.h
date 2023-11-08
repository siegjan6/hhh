/*!
\author aGRw 2015.03.04
\section 矩形框,数据类
*/
#ifndef DRAWRECTDATA_H
#define DRAWRECTDATA_H

#include <QList>
#include <QPointF>
#include <QPixmap>
#include <QJsonObject>


class CDrawRectData
{
public:
    CDrawRectData();
public:
    qreal m_xRoundBk;
    qreal m_yRoundBk;
    qreal m_roundLimit;
    qreal m_xRound;
    qreal m_yRound;
    //!custom edit
    QList<QPointF> m_customDatas;
    QPointF m_customCenter;
/*!
\section common
*/
public:
    void operator=(const CDrawRectData& other);
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
    void equal();
};

#endif // DRAWRECTDATA_H
