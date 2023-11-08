#include "MDDataArchivesCore.h"
#include "MDDatabaseService.h"
#include "IMDDataArchives.h"
#include "IMDDataSource.h"
#include "IMDDataGroup.h"
#include "IMDDataSourceManager.h"
#include "IMDDataGroupManager.h"
#include <QMutex>
#include <QThread>

CMDDataArchivesCore::CMDDataArchivesCore()
{
    m_isRunning      = false;  //! 服务运行标识
    m_iSettingFlag   = false;  //! 运行参数设置标识
    m_mdDataArchives = NULL;   //! 数据归档

    m_pDataSrcServerList.clear();
    m_mapSrcGrpTab.clear();
    m_pModifyDataSrcList.clear();
}

CMDDataArchivesCore::~CMDDataArchivesCore()
{
    removeAllModifyDataSource();
    removeAllService();
}

void CMDDataArchivesCore::load(const QString &projectPath)
{
    if (m_mdDataArchives)
    {
        m_mdDataArchives->load(projectPath);
    }
}

bool CMDDataArchivesCore::start()
{
    bool success = false;
    if (NULL == m_mdDataArchives)
    {
        return success;
    }

    if(m_isRunning)
    {
        return success;
    }
    // 数据管理模块
    IMDDataSourceManager *pDBManager = m_mdDataArchives->dataSourceManager();
    if(NULL == pDBManager)
    {
        return success;
    }

    IMDDataSource *pDataSrc = NULL;
    int count = pDBManager->dataSourceCount();
    // 枚举数据源
    for (int i = 0; i < count; i++)
    {
        pDataSrc = pDBManager->getDataSource(i);
        if(NULL == pDataSrc)
        {
            continue;
        }

        if(pDataSrc->isEnable())
        {
            m_Mutex.lock();
            // 启动一个数据源服务
            CMDDBSourceService *pDBService = new CMDDBSourceService;
            pDBService->setComment("DBSourceService_" + pDataSrc->name());
            pDBService->setDBSource(pDataSrc);
            pDBService->setDataArchives(m_mdDataArchives);
            // 启动服务
            pDBService->startService();
            m_pDataSrcServerList.append(pDBService);

            MDLOG_INFO_TO_LOGGER(QString("engine---dataarchivecore start datasource %1").arg(pDataSrc->name()));
            m_Mutex.unlock();

//            QThread::msleep(1000);
        }
    }

    success = true;
    m_isRunning = true;
    return success;
}

bool CMDDataArchivesCore::stop()
{
    m_Mutex.lock();

    CMDDBSourceService *pDBServer = NULL;

    while(!m_pDataSrcServerList.isEmpty())
    {
        pDBServer = m_pDataSrcServerList.at(0);
        if(NULL == pDBServer)
        {
            continue;
        }

        m_pDataSrcServerList.removeAt(0);
        MDLOG_INFO_TO_LOGGER(QString("engine---dataarchivecore stop datasource %1").arg(pDBServer->dataSource()->name()));
        pDBServer->stopService();
        delete pDBServer;
        pDBServer = NULL;
    }

    m_pDataSrcServerList.clear();
    m_Mutex.unlock();
    return true;
}

void CMDDataArchivesCore::runTimeLoad()
{
    if (NULL == m_mdDataArchives)
    {
        return;
    }

    IMDDataGroupManager *pGrpMana = m_mdDataArchives->dataGroupManager();
    IMDDataSourceManager *pSrcMana = m_mdDataArchives->dataSourceManager();

    if ((NULL == pGrpMana) || (NULL == pSrcMana))
    {
        return;
    }

    IMDDataSource *pDataSrc     = NULL;
    IMDDataGroup *pHisGrp       = NULL;
    QString strGrpName          = "";

    IMDDataGroup *pAlmGrp = pGrpMana->getAlarmDataGroup();
    IMDDataGroup *pLogGrp = pGrpMana->getLogDataGroup();

    QList<IMDDataTable*> tabList;
    tabList.clear();

    QMap<QString, QList<IMDDataTable*> > mapTabList;
    for (int i = 0; i < pSrcMana->dataSourceCount(); ++i)
    {
        pDataSrc = pSrcMana->getDataSource(i);
        if (NULL == pDataSrc)
        {
            continue;
        }

        strGrpName = pAlmGrp->name();
        tabList.clear();
        pDataSrc->getDataTableByGroupName(strGrpName, tabList);
        mapTabList.insert(strGrpName, tabList);
        m_mapSrcGrpTab.insert(pDataSrc->name(), mapTabList);

        strGrpName = pLogGrp->name();
        tabList.clear();
        pDataSrc->getDataTableByGroupName(strGrpName, tabList);
        mapTabList.insert(strGrpName, tabList);
        m_mapSrcGrpTab.insert(pDataSrc->name(), mapTabList);

        for (int j = 0; j < pGrpMana->hisDataGroupCount(); ++j)
        {
            pHisGrp = pGrpMana->getHisDataGroup(i);
            if (NULL == pHisGrp)
            {
                continue;
            }

            strGrpName = pHisGrp->name();
            tabList.clear();
            pDataSrc->getDataTableByGroupName(strGrpName, tabList);
            mapTabList.insert(strGrpName, tabList);
            m_mapSrcGrpTab.insert(pDataSrc->name(), mapTabList);
        }
    }
}

void CMDDataArchivesCore::startSetting()
{
    if (m_iSettingFlag)
    {
        return;
    }

    m_pModifyDataSrcList.clear();
    m_iSettingFlag = true;
}

void CMDDataArchivesCore::endSetting()
{
    if (!m_iSettingFlag)
    {
        return;
    }

    CMDDBSourceService *pDBServer   = NULL;
    IMDDataSource *pOldDataSrc      = NULL;
    IMDDataSource *pNewDataSrc      = NULL;

    int count = m_pDataSrcServerList.size();
    for (int i = 0; i < count; ++i)
    {
        pDBServer = m_pDataSrcServerList.at(i);
        if (NULL == pDBServer)
        {
            continue;
        }

        pOldDataSrc = pDBServer->dataSource();
        if (NULL == pOldDataSrc)
        {
            continue;
        }

        pNewDataSrc = findDataSource(pOldDataSrc->name());
        if (NULL == pNewDataSrc)
        {
            continue;
        }

        pDBServer->notifyModifyDatabaseParam(pNewDataSrc->clone());
        MDLOG_INFO_TO_LOGGER(QString("engine---Modify runtime database param old data source name %1, new data source name %2...").arg(pOldDataSrc->name()).arg(pNewDataSrc->name()));
    }

    removeAllModifyDataSource();
    m_iSettingFlag = false;
}

void CMDDataArchivesCore::setDatabaseParam(const QString &strDataSrcName, int type, const QString &strParam)
{
    if (NULL == m_mdDataArchives)
    {
        return;
    }

    IMDDataSourceManager *pDataMana = m_mdDataArchives->dataSourceManager();
    if (NULL == pDataMana)
    {
        return;
    }

    IMDDataSource *pDataSrcClone = findDataSource(strDataSrcName);
    IMDDataSource *pDataSrc = NULL;
    if (NULL == pDataSrcClone)
    {
        pDataSrc = pDataMana->getDataSource(strDataSrcName);
        if (NULL == pDataSrc)
        {
            return;
        }

        pDataSrcClone = pDataSrc->clone();
        if (NULL == pDataSrcClone)
        {
            return;
        }
    }

    IMDDataBaseParam *pDBParam = pDataSrcClone->dbParam();
    if (NULL == pDBParam)
    {
        return;
    }

    DATABASEPARAM::DATABASE_TYPE nType = DATABASEPARAM::DB_TYPE_INVALID;
    switch ((CMDDataArchivesCore::Settting_Param_type)type)
    {
    case CMDDataArchivesCore::DatabaseType:
        if (0 == strParam.compare(QString("MYSQL"), Qt::CaseInsensitive))
        {
            nType = DATABASEPARAM::DB_TYPE_MYSQL;
        }
        else if (0 == strParam.compare(QString("ORACLE"), Qt::CaseInsensitive))
        {
            nType = DATABASEPARAM::DB_TYPE_ORACLE;
        }
        else if (0 == strParam.compare(QString("KINGBASE"), Qt::CaseInsensitive))
        {
            nType = DATABASEPARAM::DB_TYPE_KINGBASE;
        }
        else if (0 == strParam.compare(QString("DB2"), Qt::CaseInsensitive))
        {
            nType = DATABASEPARAM::DB_TYPE_DB2;
        }
        else if (0 == strParam.compare(QString("SQLITE"), Qt::CaseInsensitive))
        {
            nType = DATABASEPARAM::DB_TYPE_SQLLITE;
        }
        else if (0 == strParam.compare(QString("SQLSERVER"), Qt::CaseInsensitive))
        {
            nType = DATABASEPARAM::DB_TYPE_SQL;
        }
        pDBParam->setdbServeType(nType);
        break;
    case CMDDataArchivesCore::DatabaseHost:
        pDBParam->setdbServerIP(strParam);
        break;
    case CMDDataArchivesCore::DatabasePort:
        pDBParam->setdbServerPort(strParam.toInt());
        break;
    case CMDDataArchivesCore::DatabaseName:
        pDBParam->setdbName(strParam);
        break;
    case CMDDataArchivesCore::DatabaseUser:
        pDBParam->setdbUser(strParam);
        break;
    case CMDDataArchivesCore::DatabasePassword:
        pDBParam->setdbPassword(strParam);
        break;
    default:
        break;
    }

    if (!findDataSource(strDataSrcName))
    {
        m_pModifyDataSrcList.append(pDataSrcClone);
    }
}

bool CMDDataArchivesCore::asyncCallDataArchives(DataArchives::DB_NOTIFY_TYPE nMSGType, CMDDataArchivesNotifyParam *pParam)
{
    m_Mutex.lock();
    CMDDBSourceService *pDBServer = NULL;
    int count = m_pDataSrcServerList.size();
    for (int i = 0; i < count; ++i)
    {
        pDBServer = m_pDataSrcServerList.at(i);
        if (NULL == pDBServer)
        {
            continue;
        }

        pDBServer->asyncCallDataArchives(nMSGType, pParam);
    }

    m_Mutex.unlock();
    return true;
}

int CMDDataArchivesCore::exec(const QString &strDataSrcName, const QString& sqlcmd)
{
    int ret = -1;
    m_Mutex.lock();
    CMDDBSourceService *pDBServer = NULL;
    int count = m_pDataSrcServerList.size();
    for (int i = 0; i < count; ++i)
    {
        pDBServer = m_pDataSrcServerList.at(i);
        if (pDBServer)
        {
            if(pDBServer->dataSource()->name() == strDataSrcName) {
                ret = pDBServer->executeCommonSQL(sqlcmd) ? 0 : -1;
                //pDBServer->asyncCallDataArchives(DataArchives::DB_NOTIFY_TYPE::DB_NOTIFY_SQL, pParam);
            }
        }
    }
    m_Mutex.unlock();
    return ret;
}

QString CMDDataArchivesCore::getLastError(const QString& datasourceName)
{
    QString ret = "";
    m_Mutex.lock();
    CMDDBSourceService *pDBServer = NULL;
    int count = m_pDataSrcServerList.size();
    for (int i = 0; i < count; ++i)
    {
        pDBServer = m_pDataSrcServerList.at(i);
        if (pDBServer)
        {
            if(pDBServer->dataSource()->name() == datasourceName)
                ret = pDBServer->getLastError();
        }
    }
    m_Mutex.unlock();
    return ret;
}

IMDDataSource *CMDDataArchivesCore::findDataSource(const QString &strSrcName)
{
    if (m_pModifyDataSrcList.isEmpty())
    {
        return NULL;
    }

    IMDDataSource *pDataSrc = NULL;
    int count = m_pModifyDataSrcList.count();
    for (int i = 0; i < count; ++i)
    {
        pDataSrc = m_pModifyDataSrcList.at(i);
        if (NULL == pDataSrc)
        {
            continue;
        }

        if (pDataSrc->name() == strSrcName)
        {
            return pDataSrc;
        }
    }

    return NULL;
}

void CMDDataArchivesCore::removeAllModifyDataSource()
{
    if (!m_pModifyDataSrcList.isEmpty())
    {
        qDeleteAll(m_pModifyDataSrcList.begin(), m_pModifyDataSrcList.end());
        m_pModifyDataSrcList.clear();
    }
}

void CMDDataArchivesCore::removeAllService()
{
    if (!m_pDataSrcServerList.isEmpty())
    {
        qDeleteAll(m_pDataSrcServerList.begin(), m_pDataSrcServerList.end());
        m_pDataSrcServerList.clear();
    }
}
