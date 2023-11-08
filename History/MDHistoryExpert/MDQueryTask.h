#ifndef MDQUERYTASK_H
#define MDQUERYTASK_H

#include "IMDDBQueryService.h"
#include "MDDatabaseRecordset.h"

class CMDHistoryExpertImp;
class CMDQueryResultNotify : public IMDDBResultNotify
{
public:
    CMDQueryResultNotify();
    ~CMDQueryResultNotify();
    CMDHistoryExpertImp* m_hisExpert;
public:
    virtual void NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *recordSet);
    CMDDatabaseRecordset m_recordSet;
};

class IMDDBQueryParam;
class IMDDataBaseParam;
class CMDDatabaseRecordset;
class CMDQueryTask
{
public:
    CMDQueryTask(CMDHistoryExpertImp* hisExpert);
    ~CMDQueryTask();

    void setDBPara(IMDDataBaseParam* dbPara);

    IMDDBQueryParam* m_para; //! 查询参数
    void resetResultRecordset();
    QStringList m_fields;
    IMDDataBaseParam* m_dbPara; //! 数据源参数

    CMDQueryResultNotify m_result; //! 接受查询结果的通知类
};

#endif // MDQUERYTASK_H
