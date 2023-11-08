/*!

\author dGFuZ3p3

\section 功能

动作数据基类，具体的动作由此类派生。

*/

#ifndef MDACTIONPROPERTY_CONFIG_H
#define MDACTIONPROPERTY_CONFIG_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include "MDActionConstant.h"
#include "../../include/MDActionIDHelper/MDActionID.h"
#include "MDActionHelper_Global.h"

class CMDActionPropertyData_Config;
class CMDActionSPList;
class MDACTIONHELPERSHARED_EXPORT CMDActionProperty_Config
{
public:
    CMDActionProperty_Config();
    CMDActionProperty_Config(MDActionType actionType);
     virtual ~CMDActionProperty_Config();
    CMDActionProperty_Config(const CMDActionProperty_Config& obj);
    CMDActionProperty_Config& operator = (const CMDActionProperty_Config& obj);
/*!
\section 属性
*/
public:
    QString name() const;
    void setName(const QString& name);

    QString comment() const;
    void setComment(const QString& comment);

    bool isEnable() const;
    void setEnable(bool enable);
public:
    CMDActionID& actionID() const;
    void setActionID(const CMDActionID& actionID);

    bool isEnableLog() const;
    void setEnableLog(bool enable);
public:
    bool isEnableDelay() const;
    void setEnableDelay(bool enable);

    qint32 delayLength() const;
    void setDelayLength(qint32 delayLength);

    bool isEnableExecExp() const;
    void setEnableExecExp(bool enable);

    QString execExp() const;
    void setExecExp(QString exp);

public:
    CMDActionSPList* spList();
    bool setSPList(const CMDActionSPList* spList);
public:
    virtual bool copy(const CMDActionProperty_Config* source);

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
    CMDActionPropertyData_Config* m_data;
};

#endif // MDACTIONPROPERTY_CONFIG_H
