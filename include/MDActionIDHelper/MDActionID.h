/*!

\author dGFuZ3p3

\section 功能

动作ID数据类，该类用于标识一个具体的动作。

*/

#ifndef MDACTIONID_H
#define MDACTIONID_H

#include <QString>
#include <QVariant>
#include "MDActionIDHelper_Global.h"
#include "../../include/MDCommonHelper/MDCommonConstant.h"
#include "../../include/MDActionHelper/MDActionConstant.h"

class CMDActionIDData;
class MDACTIONIDHELPERSHARED_EXPORT CMDActionID
{
public:
    CMDActionID();
    CMDActionID(MDActionType type, const QString& name);
    virtual ~CMDActionID();
   CMDActionID(const CMDActionID& obj);
   CMDActionID& operator = (const CMDActionID& obj);
/*!
\section 属性
*/
public:
    static QString typeToString(MDActionType actionType);
    static MDObjectType actionToObjType(MDActionType actionType);
    static CMDActionID& theDefault();
public:
    MDActionType type() const;
    void setType(MDActionType type);

    MDObjectType objType() const;
public:
    QString objName() const;
    void setObjName(const QString& name);

    QString customAction() const;
    void setCustomAction(const QString& action);

    QString customObjectName() const;
    void setCustomObjectName(const QString& name);

    QString customComment() const;
    void setCustomComment(const QString& comment);
public:
    QVariant srcState() const;
    void setSrcState(const QVariant& state);

    QVariant destState() const;
    void setDestState(const QVariant& state);
public:
    QString getLogMsg();

public:
    virtual bool copy(const CMDActionID* source);

/*!
\section 序列化
*/
public:
    virtual bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    virtual void deserialize(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    virtual void serialize(QJsonObject& json);
    virtual void deserialize(const QJsonObject& json);
protected:
    CMDActionIDData* m_data;
};

#endif // MDACTIONID_H
