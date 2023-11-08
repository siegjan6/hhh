/*!

\author dGFuZ3p3

\section 功能

日志消息数据类。

*/

#ifndef MDLOGMESSAGE_H
#define MDLOGMESSAGE_H

#include "MDLogHelper_Global.h"
#include  "../../include/MDCommonHelper/MDObject.h"
#include  "../../include/MDSecurityPolicyHelper/MDActionSPSession.h"

enum class MDLogType
{
    info        = 0,
    warn      = 1,
    error      = 2,
    success  = 3
};

class CMDLogMessageData;
class MDLOGHELPERSHARED_EXPORT CMDLogMessage
{
public:
    CMDLogMessage();
    virtual ~CMDLogMessage();
    CMDLogMessage(const CMDLogMessage& obj);
    CMDLogMessage& operator = (const CMDLogMessage& obj);
public:
    virtual bool copy(const CMDLogMessage* source);
public:
    QString msg() const;
    void setMsg(const QString& msg);

    QDateTime dt() const;
    void setDT(const QDateTime& dt);

    MDLogType type() const;
    void setType(MDLogType type);

    QString currentUser() const;
    void setCurrentUser(const QString& currentUser);

    QString source() const;
    void setSource(const QString& source);

    CMDObject* object() const;
    void setObject(const CMDObject* object);

    CMDActionSPSession* spSession() const;
    void setSpSession(const CMDActionSPSession* spSession);

protected:
    CMDLogMessageData* m_data;
};

#endif // MDLOGMESSAGE_H
