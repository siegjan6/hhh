#include "MDDataArchivesEngine.h"
#include "MDDataArchivesCore.h"

CMDDataArchivesEngine::CMDDataArchivesEngine()
{
    m_mdDataArchivesCore = new CMDDataArchivesCore();
}

CMDDataArchivesEngine::~CMDDataArchivesEngine()
{
    stop();
    if (m_mdDataArchivesCore)
    {
        delete m_mdDataArchivesCore;
        m_mdDataArchivesCore = NULL;
    }
}

void CMDDataArchivesEngine::setDataArchives(IMDDataArchives *dataArchives)
{
    if (m_mdDataArchivesCore)
    {
        m_mdDataArchivesCore->setDataArchives(dataArchives);
    }
}

bool CMDDataArchivesEngine::start()
{
    if (m_mdDataArchivesCore)
    {
        return m_mdDataArchivesCore->start();
    }

    return false;
}

bool CMDDataArchivesEngine::stop()
{
    if (m_mdDataArchivesCore)
    {
        return m_mdDataArchivesCore->stop();
    }

    return false;
}

void CMDDataArchivesEngine::load(const QString &projectPath)
{
    if (m_mdDataArchivesCore)
    {
        m_mdDataArchivesCore->load(projectPath);
    }
}


void CMDDataArchivesEngine::runTimeLoad()
{
    if (m_mdDataArchivesCore)
    {
        m_mdDataArchivesCore->runTimeLoad();
    }
}

void CMDDataArchivesEngine::startSetting()
{
    if (m_mdDataArchivesCore)
    {
        m_mdDataArchivesCore->startSetting();
    }
}

void CMDDataArchivesEngine::endSetting()
{
    if (m_mdDataArchivesCore)
    {
        m_mdDataArchivesCore->endSetting();
    }
}

void CMDDataArchivesEngine::setDatabaseParam(const QString &strDataSrcName, int type, const QString &strParam)
{
    if (m_mdDataArchivesCore)
    {
        m_mdDataArchivesCore->setDatabaseParam(strDataSrcName, type, strParam);
    }
}

bool CMDDataArchivesEngine::asyncCallDataArchives(DataArchives::DB_NOTIFY_TYPE nMSGType, CMDDataArchivesNotifyParam *pParam)
{
    if (m_mdDataArchivesCore)
    {
        return m_mdDataArchivesCore->asyncCallDataArchives(nMSGType, pParam);
    }

    return false;
}

int CMDDataArchivesEngine::exec(const QString &strDataSrcName, const QString& sqlcmd)
{
    if (m_mdDataArchivesCore)
    {
        return m_mdDataArchivesCore->exec(strDataSrcName, sqlcmd);
    }

    return -1;
}

QString CMDDataArchivesEngine::getLastError(const QString& datasourceName)
{
    if (m_mdDataArchivesCore)
    {
        return m_mdDataArchivesCore->getLastError(datasourceName);
    }

    return "";
}
