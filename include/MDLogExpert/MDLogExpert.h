/*!

\author dGFuZ3p3

\section 功能

日志专家类，实现运行时的日志存储及最近日志查询。

*/

#ifndef MDLOGEXPERT_H
#define MDLOGEXPERT_H

#include <QString>
#include <QDateTime>
#include "MDLogExpert_Global.h"
#include "../../include/MDLogHelper/MDLogMessage.h"

class CMDLogExpertImp;
class CMDHistoryExpert;
class CMDDatabaseRecordset;
class CMDLogContacter;
class MDLOGEXPERTSHARED_EXPORT CMDLogExpert
{
public:
    CMDLogExpert();
    ~CMDLogExpert();
/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, CMDHistoryExpert* historyExpert, CMDLogContacter* contacter);
    void load();
    void unload();

/*!
\section 缓存最近日志到内存；日志归档；获取最近日志
*/
public:
    bool log(CMDLogMessage& msg);
    int maxCount();
    void setMaxCount(int maxCount);
public:
    QDateTime recentUpdateTime();
public:
    bool logRecordSet(CMDDatabaseRecordset* dataRecordset); //! 获取最近日志（内存缓存的日志）

protected:
    CMDLogExpertImp* m_imp;
};

#endif // MDLOGEXPERT_H
