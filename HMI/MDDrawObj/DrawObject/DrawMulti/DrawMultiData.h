/*!
\author aGRw 2016.08.17
\section 多组控件,数据类
*/
#ifndef DRAWMULTIDATA_H
#define DRAWMULTIDATA_H

#include <QList>
#include <QJsonObject>

class CMDDrawObj;

class CDrawMultiData
{
public:
    CDrawMultiData();
public:
    QList<CMDDrawObj*> m_objList;    
    //临时处理序列化问题，以后要删除
    qint32 m_baseVersion;
/*!
\section common
*/
public:
    void operator=(const CDrawMultiData& other);
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWMULTIDATA_H
