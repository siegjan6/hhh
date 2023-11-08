/*!

\author dGFuZ3p3

\section 功能

动作ID数据类的实现类。

*/

#ifndef MDACTIONIDDATA_H
#define MDACTIONIDDATA_H

#include <QString>
#include <QVariant>
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "../../include/MDActionHelper/MDActionConstant.h"
#include <QJsonObject>
#include <QDataStream>

class CMDActionIDData
{
public:
    CMDActionIDData();
    virtual ~CMDActionIDData();
   CMDActionIDData(const CMDActionIDData& obj);
   CMDActionIDData& operator = (const CMDActionIDData& obj);
/*!
\section 属性
*/
public:
    static MDObjectType actionToObjType(MDActionType actionType);
public:
    MDObjectType m_objType ;// MDObjectType::null_obj;
    MDActionType m_type ;// MDActionType::null_action;
public:
    QString m_objName;
    QString m_objComment;
    QString m_customAction;
    QString m_customObjectName;
    QString m_customComment;
public:
    QVariant m_srcState;
    QVariant m_destState;
public:
    bool copy(const CMDActionIDData* source);

/*!
\section 序列化
*/
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream);
public:
    void deserialize(QDataStream& stream);
    void deserializeOnVersion_1(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    quint8 m_version ;// 0;
};

#endif // MDACTIONIDDATA_H
