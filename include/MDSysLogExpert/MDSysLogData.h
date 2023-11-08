/*!

\author dGFuZ3p3

\section 功能

日志记录数据类，代表了一条日志记录的信息

\section 修改历史

2015.4 / dGFuZ3p3 / 创建；
2015.4.9 / dGFuZ3p3 / 功能完成并写注释。

*/

#ifndef MDSYSLOGDATA_H
#define MDSYSLOGDATA_H

#include "MDSysLogExpert_Global.h"
#include <QString>
#include <QDateTime>

class CMDSysLogDataImp;
class MDSYSLOGEXPERTSHARED_EXPORT CMDSysLogData
{
    friend class CMDSysLoggerImp;
public:
    CMDSysLogData();
    ~CMDSysLogData();
public:
    QDateTime timeStamp() const; //! 时间戳，日志产生时的时间点
    QString timeStamp_ISO8601() const; //! QLatin1String("yyyy-MM-dd hh:mm:ss.zzz")
    //! 级别，分为 3 级：信息（128）；警告（150）；错误（182）
    int level() const;
    QString source() const; //! 来源，自定义
    QString message() const; //! 消息，自定义

protected:
    CMDSysLogDataImp* m_imp;
};

#endif // MDSYSLOGDATA_H
