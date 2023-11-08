#include "MDHistoryExpertImp.h"
#include "MDAction.h"
#include "MDHistoryContacter.h"
#include <QDebug>
#include "MDDataArchivesEngine.h"
#include "MDDataGroupFactory.h"
#include "MDDataArchivesInterface/IMDDataGroupManager.h"
#include "MDDataArchivesInterface/IMDDataArchives.h"
#include "MDDataArchivesInterface/IMDDataGroup.h"
#include "MDDataArchivesInterface/IMDDataItem.h"
#include "IMDDBQueryService.h"
#include "MDDBQueryServiceFactory.h"
#include "IMDDataSourceManager.h"
#include "IMDDataSource.h"
#include "MDRTDBInterfaces/IMDVariable.h"
#include "MDRTDBManagerFactory.h"

#include "MDDBQueryEngineFactory.h"

CMDHistoryExpertImp::CMDHistoryExpertImp()
{
    m_pid = "";
    m_contacter = NULL;
    m_dataArchivesEngine = NULL;
    m_dataArchivesConfiger = NULL;
    m_dbQueryService = NULL;
    m_dbQueryEngine = NULL;
}

/*!
\section 初始化与释放
*/
void CMDHistoryExpertImp::setParaBeforeLoad(const QString& pid, IMDDataArchives *dataArchivesConfiger, CMDHistoryContacter* contacter)
{
    m_pid = pid;
    m_contacter = contacter;
    m_dataArchivesConfiger  = dataArchivesConfiger;
}

void CMDHistoryExpertImp::load()
{
    if(!m_dataArchivesEngine)
    {
        m_dataArchivesEngine = new CMDDataArchivesEngine;
        m_dataArchivesEngine->setDataArchives(m_dataArchivesConfiger);
        m_dataArchivesEngine->start();
    }

//    if(!m_dbQueryService)
//    {
//        m_dbQueryService = CMDDBQueryServiceFactory::createDBQueryService();
//        m_dbQueryService->start();
//    }

    if(!m_dbQueryEngine)
    {
        m_dbQueryEngine = CMDDBQueryEngineFactory::createDBQueryEngine();
        m_dbQueryEngine->load(m_pid + "/DBQuery");
    }

    m_qTaskMgr.init(this);
}

void CMDHistoryExpertImp::unload()
{
    m_qTaskMgr.release();

    if(m_dataArchivesEngine)
    {
        m_dataArchivesEngine->stop();
        delete m_dataArchivesEngine;
        m_dataArchivesEngine = NULL;
    }

    if(m_dbQueryEngine)
    {
        m_dbQueryEngine->unload();
        delete m_dbQueryEngine;
        m_dbQueryEngine = NULL;
    }

//    if(m_dbQueryService)
//    {
//        m_dbQueryService->stop();
//        delete m_dbQueryService;
//        m_dbQueryService = NULL;
//    }
}

/*!
\section 执行保存历史组动作
*/
qint8 CMDHistoryExpertImp::executeAction(CMDAction* action)
{
    if(MDActionType::historyGroup_Save == action->configProperty()->actionID().type())
    {
        saveHisGroup(action->configProperty()->actionID().objName());
    }

    return -1;
}

bool CMDHistoryExpertImp::saveHisGroup(const QString& name)
{
    if(!m_contacter)
        return false;
    if(!m_dataArchivesConfiger || !m_dataArchivesEngine)
        return false;

    IMDDataGroup* hisGroup = m_dataArchivesConfiger->dataGroupManager()->getHisDataGroup(name);
    if(!hisGroup)
        return false;

    CMDDataArchivesNotifyParam* notify = new CMDDataArchivesNotifyParam;
    QDateTime currentDT = QDateTime::currentDateTime();
    notify->setDataGroupName(name);
    notify->setRecordDateTime(currentDT);
    for(int i = 0; i < hisGroup->itemCount(); i++)
    {
        IMDDataItem* groupItem = hisGroup->getItem(i);
        QString varName = groupItem->name();
        QVariant value = initItemValue(groupItem, currentDT);
        IMDDataItem* varItem = CMDDataGroupFactory::createDataItem();
        varItem->setName(varName);
        varItem->setData(value);
        varItem->setDataType(groupItem->dataType());
        varItem->setDescription(varName);
        notify->addItem(varItem);
    }

    return m_dataArchivesEngine->asyncCallDataArchives(DataArchives::DB_NOTIFY_ADD ,notify);
}

QVariant CMDHistoryExpertImp::initItemValue(IMDDataItem* groupItem, const QDateTime& dt)
{
    QVariant value = groupItem->data();
    QString itemName = groupItem->name();
    if(0 == itemName.compare("RecordTime", Qt::CaseInsensitive))
    {
        value = QVariant(dt);
    }
    else if(0 == itemName.compare("RecordTimeMS", Qt::CaseInsensitive))
    {
        value = QVariant(dt.time().msec());
    }
    else
    {
        IMDVariable* var = CMDRTDBManagerFactory::createVariable(); // 准备一个空白变量
        var->setName(itemName);
        if(m_contacter->variable(itemName, var))
            value = var->currentValue();
        delete var;
    };

    return value;
}

/*!
\section 查询归档数据
*/
IMDDBQueryEngine* CMDHistoryExpertImp::queryEngine()
{
    return m_dbQueryEngine;
}

bool CMDHistoryExpertImp::queryArchiveData(IMDDBQueryParam* queryPara)
{
    if(!m_dbQueryService) // 若查询服务不存在，查询肯定失败
        return false;

    return m_dbQueryService->query(queryPara, m_dataArchivesConfiger);
}

IMDDataSource* CMDHistoryExpertImp::dataSource(const QString& dsName)
{
    if(!m_dataArchivesConfiger)
        return nullptr;

    return m_dataArchivesConfiger->dataSourceManager()->getDataSource(dsName);
}

void CMDHistoryExpertImp::onQueryResult(const QString& queryID, int result)
{
    if(!m_contacter)
        return;

    m_contacter->onQueryResult(queryID, result);
}
