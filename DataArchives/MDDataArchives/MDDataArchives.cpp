#include "MDDataArchives.h"
#include "IMDDataGroupManager.h"
#include "IMDDataSourceManager.h"
#include "IMDDataSource.h"
#include "IMDDataTable.h"
#include "IMDDataField.h"
#include "MDDataGroupFactory.h"
#include "MDDataSourceFactory.h"
#include <QFile>
#include <QDir>

CMDDataArchives::CMDDataArchives()
{
    m_mdDataGroupManager = CMDDataGroupFactory::createDataGroupManager();
    m_mdDataSourceManager = CMDDataSourceFactory::createDataSourceManager();
}

CMDDataArchives::~CMDDataArchives()
{
//    unLoad();
    if (m_mdDataGroupManager)
    {
        delete m_mdDataGroupManager;
        m_mdDataGroupManager = NULL;
    }

    if (m_mdDataSourceManager)
    {
        delete m_mdDataSourceManager;
        m_mdDataSourceManager = NULL;
    }
}

/*!
\section 加载数据源管理器与数据组管理器
*/
void CMDDataArchives::load(const QString &projectPath)
{
    if (NULL == m_mdDataGroupManager)
    {
        m_mdDataGroupManager = CMDDataGroupFactory::createDataGroupManager();
    }

    if (NULL == m_mdDataSourceManager)
    {
        m_mdDataSourceManager = CMDDataSourceFactory::createDataSourceManager();
    }

    QString dataGroupFile = "";
    QString dataSourceFile = "";
    QString tmpPath = projectPath;
    if(!(tmpPath.right(1) == "/"))
    {
        tmpPath += "/";
    }
    m_projectPath = tmpPath;

    calDataGrpAndDataSrcPath(tmpPath, dataGroupFile, dataSourceFile);
    /////////////////////////////////////////////////////////////////////////////////////
    // 老版本升级临时代码 打开.dat -> 保存为.json
    /////////////////////////////////////////////////////////////////////////////////////
//    QString dataGroupFile_upgrade = dataGroupFile.replace(".json",".dat");
//    QString dataSourceFile_upgrade = dataSourceFile.replace(".json",".dat");
//    m_mdDataGroupManager->load(dataGroupFile_upgrade);
//    m_mdDataSourceManager->load(dataSourceFile_upgrade);
    /////////////////////////////////////////////////////////////////////////////////////

    m_mdDataGroupManager->load_json(dataGroupFile);
    m_mdDataSourceManager->load_json(dataSourceFile);
    m_mdDataSourceManager->setTempDatabasePath(tmpPath + "TempDatabase/");
}

/*!
\section 存储数据源管理器与数据组管理器
*/
void CMDDataArchives::save(const QString &projectPath)
{
    QString dataGroupFile = "";
    QString dataSourceFile = "";
    calDataGrpAndDataSrcPath(projectPath, dataGroupFile, dataSourceFile);
    m_mdDataGroupManager->save_json(dataGroupFile);
    m_mdDataSourceManager->save_json(dataSourceFile);
}

QString CMDDataArchives::projectPath()
{
    return m_projectPath;
}

void CMDDataArchives::unLoad()
{
    if (m_mdDataGroupManager)
    {
        delete m_mdDataGroupManager;
        m_mdDataGroupManager = NULL;
    }

    if (m_mdDataSourceManager)
    {
        delete m_mdDataSourceManager;
        m_mdDataSourceManager = NULL;
    }
}

/*!
    查找所有数据源名。
\param 无。
\returns QStringList。
\note 无。
*/
QStringList CMDDataArchives::allSource()
{
    QStringList allSrcNameList;
    allSrcNameList.clear();

    if (NULL == m_mdDataSourceManager)
    {
        return allSrcNameList;
    }

    IMDDataSource *pDataSrc = NULL;
    for (int i = 0; i < m_mdDataSourceManager->dataSourceCount(); ++i)
    {
        pDataSrc = m_mdDataSourceManager->getDataSource(i);
        if (NULL == m_mdDataSourceManager)
        {
            continue;
        }

        allSrcNameList.append(pDataSrc->name());
    }

    return allSrcNameList;
}

/*!
    查找指定数据源中的所有表名。
\param sourceName 数据源名。
\returns QStringList。
\note 无。
*/
QStringList CMDDataArchives::tablesOfSource(const QString &sourceName)
{
    QStringList allTabNameList;
    allTabNameList.clear();

    if (NULL == m_mdDataSourceManager)
    {
        return allTabNameList;
    }

    IMDDataSource *pDataSrc = m_mdDataSourceManager->getDataSource(sourceName);
    if (NULL == pDataSrc)
    {
        return allTabNameList;
    }

    IMDDataTable *pDataTab = NULL;
    for (int i = 0; i < pDataSrc->dataTableCount(); ++i)
    {
        pDataTab = pDataSrc->getDataTable(i);
        if (NULL == pDataTab)
        {
            continue;
        }

        allTabNameList.append(pDataTab->name());
    }

    return allTabNameList;
}

/*!
    查找指定数据源中某个表的所有字段名。
\param sourceName 数据源名。
\param tableName 表名。
\returns QStringList。
\note 无。
*/

QStringList CMDDataArchives::fieldsOfTable(const QString &sourceName, const QString &tableName)
{
    QStringList allFiledNameList;
    allFiledNameList.clear();

    if (NULL == m_mdDataSourceManager)
    {
        return allFiledNameList;
    }

    IMDDataSource *pDataSrc = m_mdDataSourceManager->getDataSource(sourceName);
    if (NULL == pDataSrc)
    {
        return allFiledNameList;
    }

    IMDDataTable *pDataTab = pDataSrc->getDataTable(tableName);
    if (NULL == pDataTab)
    {
        return allFiledNameList;
    }

    IMDDataField *pDataField = NULL;
    for (int i = 0; i < pDataTab->dataFieldCount(); ++i)
    {
        pDataField = pDataTab->getDataField(i);
        if (NULL == pDataField)
        {
            continue;
        }

        allFiledNameList.append(pDataField->name());
    }

    return allFiledNameList;
}

/*!
    计算数据组与数据组的序列化文件路径
\param projectPath 项目序列化路径。
\param dataGrpPath 数据组序列化路径。
\param dataSrcPath 数据源序列化路径。
\returns 无。
\note 无。
*/
void CMDDataArchives::calDataGrpAndDataSrcPath(const QString &projectPath, QString &dataGrpPath, QString &dataSrcPath)
{
    QString strDataArchivesPath = projectPath + "DataArchives/";
    QString strTmpDatabasePath = projectPath + "TempDatabase/";
    //dataGrpPath = strDataArchivesPath + "dataGroup.dat";
    dataGrpPath = strDataArchivesPath + "dataGroup.json";
    //dataSrcPath = strDataArchivesPath + "dataSource.dat";
    dataSrcPath = strDataArchivesPath + "dataSource.json";

    QDir dir;
    if (!dir.exists(strDataArchivesPath))
    {
        dir.mkdir(strDataArchivesPath);
    }

    if (!dir.exists(strTmpDatabasePath))
    {
        dir.mkdir(strTmpDatabasePath);
    }
}
