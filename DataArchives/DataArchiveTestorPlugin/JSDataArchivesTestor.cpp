#include "JSDataArchivesTestor.h"
#include "TSQLExecutor.h"
#include "JSDBParameters.h"
#include <IMDDataItem.h>
#include <MDDataGroupFactory.h>
#include <IMDDataSourceManager.h>
#include <IMDDataSource.h>
#include <IMDDatabaseParam.h>
#include <IMDDataArchives.h>
#include <QMessageBox>
#include <QDateTime>
#include <QThread>

enum JSDATA_PARAM
{
    JSDATA_PARAM_DatabaseInvalid             = -1,
    JSDATA_PARAM_DatabaseType                = 1,
    JSDATA_PARAM_DatabaseHost                = 2,
    JSDATA_PARAM_DatabasePort                = 3,
    JSDATA_PARAM_DatabaseName                = 4,
    JSDATA_PARAM_DatabaseUser                = 5,
    JSDATA_PARAM_DatabasePassword            = 6
};

static const char * DBTypeConvert(DATABASEPARAM::DATABASE_TYPE type);

JSDataArchivesTestor::JSDataArchivesTestor(QObject *parent) :
    QObject(parent),
    m_pDataArchivesEngine(0),
    m_pDataArchives(0),
    m_pTestDataGroupMgr(0),
    m_pDSourceMgr(0)
{
    m_pSqlExecutor = new TSQLExecutor;
}

void JSDataArchivesTestor::SetParams(IMDDataArchives * pDataArchives, ITestorDataGroupMgr * pTestDataGroupMgr, IMDDataSourceManager * pDSourceMgr)
{
    m_pDataArchives = pDataArchives;
    m_pTestDataGroupMgr = pTestDataGroupMgr;
    m_pDSourceMgr = pDSourceMgr;
}

//!选择测试数据用例 并转化成可添加到归档引擎的多条记录
bool JSDataArchivesTestor::selectTestDataCase(QString testDataCaseName)
{
    QString fileContentOut;
    if(!m_pTestDataGroupMgr->GetCase(testDataCaseName, fileContentOut))
    {
        QMessageBox::critical(0, "Test Data Error", "cann't open test data file");
        return false;
    }

   if(! m_TestDataDomDoc.setContent(fileContentOut))
   {
        QMessageBox::critical(0, "XML Error", "cann't parse test data file");
        return false;
   }

   //解析成CDataArchivesNotifyParam对象
    QDomElement root = m_TestDataDomDoc.documentElement();
    QDomElement table =  root.firstChildElement();

    QString itemTypeStr;
    static QString itemTypeString("string");
    static QString itemTypeDateTime("dateTime");
    static QString itemTypeInt("int");
    static QString itemTypeDouble("double");

    while(!table.isNull())
    {
        QDomElement record = table.firstChildElement();
        if(table.attribute("name").isEmpty())
        {
            //TODO:LOG
            continue;
        }


        while(!record.isNull())
        {
            QDomElement item = record.firstChildElement();

            //如果record没有item 则不创建CDataArchivesNotifyParam对象
            CMDDataArchivesNotifyParam * pRecord = 0;
            if(!item.isNull())
             {
                pRecord = new CMDDataArchivesNotifyParam;
                pRecord->setDataGroupName(table.attribute("name")); //设置数据库表名
            }

            while(!item.isNull())
            {
               itemTypeStr = item.attribute("type");
               //把默认记录时间字段增加到第一行的位置
               IMDDataItem *dataItem = CMDDataGroupFactory::createDataItem();

               dataItem->setName(item.attribute("name"));
               //pHisDataGrp->addItem(item);


               if(itemTypeStr.compare(itemTypeString, Qt::CaseInsensitive) == 0)
               {
                   QVariant var(item.text());
                    dataItem->setDataType(MDDataType::String);
                    dataItem->setData(var);
               }else if(itemTypeStr.compare(itemTypeDateTime, Qt::CaseInsensitive) == 0)
               {
                   //2016/07/01 08:30:29.120   yyyy/MM/dd hh:mm:ss.z
                   QDateTime dateTime = QDateTime::fromString(item.text(), "yyyy/MM/dd hh:mm:ss.z");
                   QVariant var(dateTime);
                    dataItem->setDataType(MDDataType::DateTime);
                    dataItem->setData(var);
               }else if(itemTypeStr.compare(itemTypeInt, Qt::CaseInsensitive) == 0)
               {
                   QVariant var(item.text().toInt());
                    dataItem->setDataType(MDDataType::Int32);
                    dataItem->setData(var);
               }else if(itemTypeStr.compare(itemTypeDouble, Qt::CaseInsensitive) == 0)
               {
                   QVariant var(item.text().toDouble());
                    dataItem->setDataType(MDDataType::Double);
                    dataItem->setData(var);
               }else
               {
                   //TODO:log exit
               }

               pRecord->addItem(dataItem);

               item = item.nextSiblingElement();
            }//end item

            if(pRecord)
            {
                CMDDataArchivesNotifyParamWrapper * wrapper = new CMDDataArchivesNotifyParamWrapper;

                wrapper->param = pRecord;

                if(record.attribute("action").compare("add", Qt::CaseInsensitive) == 0)
                    wrapper->type = DataArchives::DB_NOTIFY_ADD;
                else if(record.attribute("action").compare("update", Qt::CaseInsensitive) == 0)
                    wrapper->type = DataArchives::DB_NOTIFY_UPDATE;
                else if(record.attribute("action").compare("delete"), Qt::CaseInsensitive == 0)
                    wrapper->type = DataArchives::DB_NOTIFY_DELETE;
                else
                    wrapper->type = DataArchives::DB_NOTIFY_ALM_OFF; //TODO:Temp

                m_RecordWrapperList.append(wrapper);
            }

            record = record.nextSiblingElement();
        }//end record
        table = table.nextSiblingElement();
    }//end table

    return true;
}

bool JSDataArchivesTestor::start()
{
    m_pDataArchivesEngine = new CMDDataArchivesEngine();
    m_pDataArchivesEngine->setDataArchives(m_pDataArchives);
    m_pDataArchivesEngine->start();
    return true;
}

bool JSDataArchivesTestor::stop()
{

    if(m_pDataArchivesEngine)
        m_pDataArchivesEngine->stop();

    delete m_pDataArchivesEngine;
    m_pDataArchivesEngine = 0;

    CMDDataArchivesNotifyParamWrapper * wrapper = 0;
    while(!m_RecordWrapperList.isEmpty())
    {
        wrapper = m_RecordWrapperList.takeFirst();
        delete wrapper->param;
        delete wrapper;
    }

    return true;
}

void JSDataArchivesTestor::sleep(int millisecond)
{
    QThread::msleep(millisecond);
}

bool JSDataArchivesTestor::disconnectDBSource(QString dbSourceName)
{
    //获得数据源的配置信息:IMDDataSource 数据源连接信息   表信息
    IMDDataSourceManager * pDSourceMgr = m_pDataArchives->dataSourceManager();
    IMDDataSource * pDSource = pDSourceMgr->getDataSource(dbSourceName);
    if(!pDSource)
        return false;

    QMap<QString, JSDBParameters *>::iterator ite = m_oldDBParaMap.find(dbSourceName);
   //备份原始连接信息
    if(ite == m_oldDBParaMap.end()) //说明没有此元素
    {
        JSDBParameters *pDBParam = new JSDBParameters;
        m_oldDBParaMap.insert(dbSourceName, pDBParam);

        IMDDataBaseParam * pMDDBParam = pDSource->dbParam();

        pDBParam->dbTypeStr = DBTypeConvert(pMDDBParam->dbServeType());
        pDBParam->hostStr = pMDDBParam->dbServerIP();
        pDBParam->portStr = QString("%1").arg(pMDDBParam->dbServerPort());
        pDBParam->databaseName = pMDDBParam->dbName();
        pDBParam->databaseUser = pMDDBParam->dbUser();
        pDBParam->databasePassword = pMDDBParam->dbPassword();
    }

    //将dbSourceName代表的数据库连接设置一个无效连接参数 使之断开连接
    m_pDataArchivesEngine->startSetting();

    IMDDataBaseParam * pMDDBParam = pDSource->dbParam();
    m_pDataArchivesEngine->setDatabaseParam(dbSourceName, JSDATA_PARAM_DatabaseType, "MYSQL");
    m_pDataArchivesEngine->setDatabaseParam(dbSourceName, JSDATA_PARAM_DatabaseHost, "127.0.0.1");
    m_pDataArchivesEngine->setDatabaseParam(dbSourceName, JSDATA_PARAM_DatabasePort, "65539"); //设置一个非法端口
    m_pDataArchivesEngine->setDatabaseParam(dbSourceName, JSDATA_PARAM_DatabaseName, pMDDBParam->dbName());
    m_pDataArchivesEngine->setDatabaseParam(dbSourceName, JSDATA_PARAM_DatabaseUser, "wwee");
    m_pDataArchivesEngine->setDatabaseParam(dbSourceName, JSDATA_PARAM_DatabasePassword, "111111");

    m_pDataArchivesEngine->endSetting();

    return true;
}

bool JSDataArchivesTestor::reconnectDBSource(QString dbSourceName)
{
    IMDDataSourceManager * pDSourceMgr = m_pDataArchives->dataSourceManager();
    IMDDataSource * pDSource = pDSourceMgr->getDataSource(dbSourceName);
    if(!pDSource)
        return false;

    QMap<QString, JSDBParameters *>::iterator ite = m_oldDBParaMap.find(dbSourceName);
    if(ite == m_oldDBParaMap.end()) //说明此元素不存在
        return false; //TODO:LOG

    JSDBParameters * pDBParam = ite.value();

    m_pDataArchivesEngine->startSetting();

    //将dbSourceName代表的数据库连接设置一个无效连接参数 使之断开连接
    m_pDataArchivesEngine->setDatabaseParam(dbSourceName, JSDATA_PARAM_DatabaseType, pDBParam->dbTypeStr);
    m_pDataArchivesEngine->setDatabaseParam(dbSourceName, JSDATA_PARAM_DatabaseHost, pDBParam->hostStr);
    m_pDataArchivesEngine->setDatabaseParam(dbSourceName, JSDATA_PARAM_DatabasePort, pDBParam->portStr);
    m_pDataArchivesEngine->setDatabaseParam(dbSourceName, JSDATA_PARAM_DatabaseName, pDBParam->databaseName);
    m_pDataArchivesEngine->setDatabaseParam(dbSourceName, JSDATA_PARAM_DatabaseUser, pDBParam->databaseUser);
    m_pDataArchivesEngine->setDatabaseParam(dbSourceName, JSDATA_PARAM_DatabasePassword, pDBParam->databasePassword);

    m_pDataArchivesEngine->endSetting();
    return true;
}

int JSDataArchivesTestor::queryCount(const QString & dbSourceName, const QString & sqlStr)
{
    QMap<QString, JSDBParameters *>::iterator ite = m_oldDBParaMap.find(dbSourceName);

    //备份原始连接信息
    JSDBParameters *pDBParam = 0;
    if(ite == m_oldDBParaMap.end()) //说明没有此元素
    {
        pDBParam = new JSDBParameters;
        m_oldDBParaMap.insert(dbSourceName, pDBParam);

        IMDDataSourceManager * pDSourceMgr = m_pDSourceMgr;
        IMDDataSource * pDSource = pDSourceMgr->getDataSource(dbSourceName);
        if(!pDSource)
            return -1;

        IMDDataBaseParam * pMDDBParam = pDSource->dbParam();

        pDBParam->dbTypeStr = DBTypeConvert(pMDDBParam->dbServeType());
        pDBParam->hostStr = pMDDBParam->dbServerIP();
        pDBParam->portStr = QString("%1").arg(pMDDBParam->dbServerPort());
        pDBParam->databaseName = pMDDBParam->dbName();
        pDBParam->databaseUser = pMDDBParam->dbUser();
        pDBParam->databasePassword = pMDDBParam->dbPassword();
    }
    else
        pDBParam = ite.value();

    return m_pSqlExecutor->queryCount(dbSourceName, pDBParam, sqlStr);
}

bool JSDataArchivesTestor::isTestDataItemOver()
{
    return m_RecordWrapperList.isEmpty();
}

void JSDataArchivesTestor::saveTestDataItemAndAdvance()
{
    CMDDataArchivesNotifyParamWrapper * wrapper = 0;
    if(!m_RecordWrapperList.isEmpty())
    {
        wrapper = m_RecordWrapperList.takeFirst();
        m_pDataArchivesEngine->asyncCallDataArchives(wrapper->type, wrapper->param);
    }
}

bool JSDataArchivesTestor::checkSaveResult()
{
    return true;
}

static const char * DBTypeConvert(DATABASEPARAM::DATABASE_TYPE type)
{
    switch(type)
    {
    case DATABASEPARAM::DB_TYPE_MYSQL:
        return "MYSQL";
    case  DATABASEPARAM::DB_TYPE_ORACLE:
        return "ORACLE";
    case DATABASEPARAM::DB_TYPE_KINGBASE:
        return "KINGBASE";
    case DATABASEPARAM::DB_TYPE_DB2:
        return "DB2";
    case DATABASEPARAM::DB_TYPE_SQLLITE:
        return "SQLITE";
    case DATABASEPARAM::DB_TYPE_SQL:
        return "SQLSERVER";
    default:
        return "INVALID";
    }
    return "INVALID";
}
