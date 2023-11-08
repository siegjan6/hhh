/*!

\author dGFuZ3p3

\section 功能

报警条件数据类。

*/

#ifndef MDALARMCONDITION_H
#define MDALARMCONDITION_H

#include "MDAlarmHelper_Global.h"
#include "MDAlarmLevel.h"
#include "../../include/MDEventHelper/MDEventHelper.h"
#include "../../include/MDSecurityPolicyHelper/MDActionSP.h"

class CMDAlarmConditionImp;
class MDALARMHELPERSHARED_EXPORT CMDAlarmCondition
{
public:
    CMDAlarmCondition();
    virtual ~CMDAlarmCondition();
   CMDAlarmCondition(const CMDAlarmCondition& obj);
   CMDAlarmCondition& operator = (const CMDAlarmCondition& obj);
/*!
\section 属性
*/
public:
   QString name() const;
   void setName(const QString& name);

   QString message() const;
   void setMessage(const QString& message);

   QString comment() const;
   void setComment(const QString& comment);

   QString levelName() const;
   void setLevelName(const QString& levelName);

   QString expression() const;
   void setExpression(const QString& expression);

   QString bindVar();
   void setBindVar(const QString& bindVar);

   QString customId() const;
   void setCustomId(const QString& comment);

   int serverity() const;
   void setServerity(const int& serverity);

   bool delayEnabled() const;
   void setDelayEnabled(const bool& delayEnabled);

   QString delayConditionExp();
   void setDelayConditionExp(const QString& delayConditionExp);

   QString delayTimeExp();
   void setDelayTimeExp(const QString& delayTimeExp);

   bool latched() const;
   void setLatched(const bool& latched);

   bool acknowledgeRequired() const;
   void setAcknowledgeRequired(const bool& acknowledgeRequired);

   bool enabled() const;
   void setEnabled(const bool& enabled);

   QString enableAlarmExp();
   void setEnableAlarmExp(const QString& enableAlarmExp);

   enum LimitAlarmMode
   {
       NONE,
       HHL,
       HL,
       LL,
       LLL
   };
   LimitAlarmMode currentLimitAlarmMode() const;
   void setCurrentLimitAlarmMode(const LimitAlarmMode& currentLimitAlarmMode);

private:
   LimitAlarmMode m_currentLimitAlarmMode;

public:
   CMDActionSPList* spList();
   bool setSPList(const CMDActionSPList* spList);
public:
   virtual bool copy(const CMDAlarmCondition* source);

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

   QString toJsonString();
protected:
   CMDAlarmConditionImp* m_data;

   QString m_message;
};

#endif // MDALARMCONDITION_H
