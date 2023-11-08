/*!

\author dGFuZ3p3

\section 功能

日志消息数据类的实现类。

*/

#ifndef MDLOGMESSAGEDATA_H
#define MDLOGMESSAGEDATA_H

#include  <QString>
#include  "MDObject.h"
#include  "MDActionSPSession.h"
#include  "MDLogMessage.h"

class CMDLogMessageData
{
public:
    CMDLogMessageData();
    virtual ~CMDLogMessageData();
   CMDLogMessageData(const CMDLogMessageData& obj);
   CMDLogMessageData& operator = (const CMDLogMessageData& obj);
public:
    bool copy(const CMDLogMessageData* source);
public:
    QString m_msg;
    QDateTime m_dt ;// QDateTime::currentDateTime();
    MDLogType m_type ;// MDLogType::info;
    QString m_source;
    QString m_currentUser;

    CMDObject m_object;
    CMDActionSPSession m_spSession;
};
#endif // MDLOGMESSAGEDATA_H
