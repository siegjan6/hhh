#include "MDQueryTask.h"
#include "IMDDatabaseParam.h"
#include "MDDataSourceFactory.h"
#include "MDDBQueryServiceFactory.h"
#include "MDHistoryExpertImp.h"

CMDQueryResultNotify::CMDQueryResultNotify()
{
    m_hisExpert = NULL;
}

CMDQueryResultNotify::~CMDQueryResultNotify()
{
    m_recordSet.clear();
}

void CMDQueryResultNotify::NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *recordSet)
{
    int rowCount = recordSet->rowCount();
    int colCount = recordSet->columnCount();
    for(int row = 0; row < rowCount; row ++)
    {
        int row1 = m_recordSet.addRow();
        for(int col = 0; col < colCount; col ++)
        {
            QVariant data = recordSet->value(row, col);
            m_recordSet.setValue(row1, col, data);
        }
    }

    if(m_hisExpert)
        m_hisExpert->onQueryResult("", 0);
}

CMDQueryTask::CMDQueryTask(CMDHistoryExpertImp* hisExpert)
{
    m_result.m_hisExpert = hisExpert;

    m_dbPara = NULL;//CMDDataSourceFactory::createDatabasePaream();
    m_para = CMDDBQueryServiceFactory::createDBQueryParam();
}

CMDQueryTask::~CMDQueryTask()
{
    if(m_para)
        delete m_para;
    if(m_dbPara)
        delete m_dbPara;

    m_result.m_recordSet.clear();
}

void CMDQueryTask::setDBPara(IMDDataBaseParam* dbPara)
{
    // dbPara 必须是 new 的对象
    m_dbPara = dbPara;
    m_para->setDBParam(dbPara);
}

void CMDQueryTask::resetResultRecordset()
{
    m_result.m_recordSet.clear();
    m_result.m_recordSet.bindFileds(m_fields);
}
