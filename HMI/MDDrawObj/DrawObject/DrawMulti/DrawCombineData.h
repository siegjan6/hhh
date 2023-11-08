/*!
\author aGRw 1016.08.09
\section 联合控件,数据类
*/
#ifndef DRAWCOMBINEDATA_H
#define DRAWCOMBINEDATA_H

#include <QList>
#include <Qt>
#include <QJsonObject>
#include <QJsonValue>

class CMDDrawObj;

class CDrawCombineData
{
public:
    CDrawCombineData();
public:
    //连接线
    bool m_connectLine;
    //填充模式
    Qt::FillRule m_fillMode;
/*!
\section common
*/
public:
    void operator=(const CDrawCombineData& other);
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWCOMBINEDATA_H
