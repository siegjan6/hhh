/*!

\author dGFuZ3p3

\section 功能

报警参数数据类。

*/

#ifndef MDALARMPARA_H
#define MDALARMPARA_H

#include "MDAlarmHelper_Global.h"
#include "MDAlarmConstant.h"
#include <QStringList>
#include <QJsonObject>

class CMDAlarmParaImp;
class MDALARMHELPERSHARED_EXPORT CMDAlarmPara
{
public:
    CMDAlarmPara();
    virtual ~CMDAlarmPara();
   CMDAlarmPara(const CMDAlarmPara& obj);
   CMDAlarmPara& operator = (const CMDAlarmPara& obj);
/*!
\section 属性
*/
public:
   static void enumModes(QStringList& modeList);
   static QString modeToString(MDAlarmMode mode);
   static MDAlarmMode stringToMode(const QString& str);
public:
   MDAlarmMode alarmMode() const;
   void setAlarmMode(MDAlarmMode mode);
public:
   virtual bool copy(const CMDAlarmPara* source);

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
   CMDAlarmParaImp* m_data;
};

#endif // MDALARMPARA_H
