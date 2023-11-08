/*!
\author aGRw 2015.03.03
\section 成组控件,数据类
*/
#ifndef DRAWGROUPDATA_H
#define DRAWGROUPDATA_H

#include <QList>
#include <QJsonObject>
#include <QJsonValue>
#include "MDCustomProperty.h"

class CMDDrawObj;

class CDrawGroupData
{
public:
    CDrawGroupData();
public: 
    QList<CMDCustomProperty> m_customPropertys;//!自定义属性
    bool m_isVector;
/*!
\section common
*/
public:
    void operator=(const CDrawGroupData& other);
    void serialize(QJsonObject& json);
    void deserialize(QJsonObject& json);
};

#endif // DRAWGROUPDATA_H
