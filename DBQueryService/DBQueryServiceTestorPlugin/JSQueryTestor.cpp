#include "JSQueryTestor.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <MDDBQueryServiceFactory.h>
#include <IMDDataSourceManager.h>
#include <IMDDataSource.h>
#include <MDDatabaseRecordset.h>
#include <QDebug>
#include <qalgorithms.h>
#include <IMDDatabaseParam.h>
#include <QMessageBox>
#include <QThread>

static bool ParseJsonToIMDDBQueryParams(const QJsonObject & json, IMDDBQueryService * pDBQueryService, QList<IMDDBQueryParam *> & pQueryParamsOut);

void JSQueryTestorDBResultNofity::NotityDBQueryResult(const QString & strQueryID, CMDDatabaseRecordset * pRecordset)
{
    //TODO:ASSERT pRecordset
    int row = pRecordset->rowCount();
    int col = pRecordset->columnCount();

    for(int i=0;i<row; ++i)
    {
        qDebug()<<i<<" row";

        for(int j=0;j<col; ++j)
        {
            qDebug()<<pRecordset->value(i, j);
        }
    }
}

JSQueryTestor::JSQueryTestor(QObject *parent)
    :QObject(parent),
      m_pDBQueryService(0),
      m_pIMDDataArchives(0),
      m_pTestDataGroupMgr(0)
{
    m_pTestorDBResultNofity = new JSQueryTestorDBResultNofity;
    m_pDBQueryService = CMDDBQueryServiceFactory::createDBQueryService();
    m_pDBQueryService->start();
}

void JSQueryTestor::setParams(IMDDataArchives * pDataArchives, ITestorDataGroupMgr * pTestDataGroupMgr)
{
    m_pIMDDataArchives = pDataArchives;
    m_pTestDataGroupMgr = pTestDataGroupMgr;
}

bool JSQueryTestor::selectTestQueryCase(QString testQueryCaseName)
{
    //qDeleteAll(m_QueryParams);

    for(int i=0; i<m_QueryParams.size();++i)
    {
        delete m_QueryParams.at(i);
    }
    m_QueryParams.clear();

    QString fileContentOut;
    if(!m_pTestDataGroupMgr->GetCase(testQueryCaseName, fileContentOut))
    {
        QMessageBox::critical(0, "Test Data Error", "cann't open test query file");
        return false;
    }

    QByteArray jsonData = fileContentOut.toUtf8();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));


    if(!ParseJsonToIMDDBQueryParams(jsonDoc.object(), m_pDBQueryService, m_QueryParams))
    {
        QMessageBox::critical(0, "Json Error", "cann't parse test query file");
        return false;
    }

    return true;
}

/*
bool JSQueryTestor::start()
{
...???
    return true;
}

bool JSQueryTestor::stop()
{

    if(m_pDBQueryService)
        m_pDBQueryService->stop();

    delete m_pDBQueryService;
    m_pDBQueryService = 0;

    return true;
}
*/

void JSQueryTestor::sleep(int millisecond)
{
    QThread::msleep(millisecond);
}

bool JSQueryTestor::query()
{
    int count = m_QueryParams.size();
    if(count<1)
        return false;

    if(count == 1) //日后有需求时再扩展吧
    {
        if(!ConfigQueryParams(m_QueryParams, DB_QUERY::Where_Condi_Mode))
            return false;

        IMDDBQueryParam * queryParam = m_QueryParams.at(0);
        return m_pDBQueryService->query(queryParam,  m_pIMDDataArchives);
    }

    //count >1，认为是union操作.日后有需求时再扩展吧
    if(!ConfigQueryParams(m_QueryParams, DB_QUERY::Where_Condi_Mode))
        return false;

    return m_pDBQueryService->queryUnion(m_QueryParams, m_pIMDDataArchives);
}

bool JSQueryTestor::ConfigQueryParams(QList<IMDDBQueryParam *> & queryParamsOut, DB_QUERY::DB_QUERY_MODE queryMode)
{
    if(queryParamsOut.size()<1)
        return false;

    IMDDBQueryParam * pQueryParam = 0;
    for(int i=0; i<queryParamsOut.size();++i)
    {
        pQueryParam = queryParamsOut.at(i);
        pQueryParam->setQueryMode(queryMode);
        //没有几个数据源
        IMDDataSource * pDataSource = m_pIMDDataArchives->dataSourceManager()->getDataSource(pQueryParam->dataSourceName());
        if(!pDataSource)
            return false;

        IMDDataBaseParam * pDataBaseParam = pDataSource->dbParam();
        if(!pDataBaseParam)
            return false;

        pQueryParam->setDBParam(pDataBaseParam);//delete pQueryParam时,其析构函数会delete pDataBaseParam.所以要clone.
        pQueryParam->setQueryResultNotify(m_pTestorDBResultNofity);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static IMDDBQueryParam * ReadSelectJsonObj(const QJsonObject & selectJsonObj,  IMDDBQueryService * pDBQueryService)
{
    QJsonArray fieldArr = selectJsonObj["fields"].toArray();
    //fields == * ?
    if(fieldArr.size()<1)
        return 0;

    IMDDBQueryParam * pQueryParm = CMDDBQueryServiceFactory::createDBQueryParam(); //不成功 要释放此结构

    //<填入查询字段>
    QList<QString> fieldList;
    for(int i=0; i<fieldArr.size(); i++)
    {
        fieldList.append(fieldArr.at(i).toString());
    }

    pQueryParm->setFieldList(fieldList);
    //</填入查询字段>

    //目前只支持单表 setQuerytable
    QJsonArray tbArr = selectJsonObj["tables"].toArray();
    //qDebug()<<"select tables = ";
    if(tbArr.size()<1)
    {
        delete pQueryParm;
        return 0;
    }

    pQueryParm->setQuerytable(tbArr.at(0).toString());

   QString whereStr = selectJsonObj["where_str"].toString();
   if(!whereStr.isEmpty())
   {
       pQueryParm->setQueryWhereConditionstr(whereStr);
   }

   //必须要有数据源字段
    QString dataSrcStr = selectJsonObj["datasource"].toString();
    if(dataSrcStr.isEmpty())
    {
        delete pQueryParm;
        return 0;
    }

    pQueryParm->setDataSourceName(dataSrcStr);

   return pQueryParm;
}

static bool ParseJsonToIMDDBQueryParams(const QJsonObject & json, IMDDBQueryService * pDBQueryService, QList<IMDDBQueryParam *> & pQueryParamsOut)
{
    QString kindStr = json["kind"].toString();
    if(kindStr.compare("select", Qt::CaseInsensitive) == 0)
    {
        QJsonArray selectArr = json["select"].toArray();
        //int selecArrSize = selectArr.size();
        int count = selectArr.count();
        if(count<1)
            return false;

        QJsonObject selectJsonObj = selectArr.at(0).toObject();
        IMDDBQueryParam * pQueryParam = ReadSelectJsonObj(selectJsonObj, pDBQueryService);
        if(pQueryParam)
        {
            pQueryParamsOut.append(pQueryParam);
            return true;
        }
        return false;
      }//end if

    if(kindStr.compare("union", Qt::CaseInsensitive) == 0)
    {
        QJsonArray selectArr = json["select"].toArray();

        for(int i=0; i<selectArr.count(); ++i)
        {
            QJsonObject selectJsonObj = selectArr.at(i).toObject();
            IMDDBQueryParam * pQueryParam = ReadSelectJsonObj(selectJsonObj, pDBQueryService);
            if(pQueryParam)
                pQueryParamsOut.append(pQueryParam);
        }

        if(pQueryParamsOut.size()<2)
        {
            qDeleteAll(pQueryParamsOut);
            pQueryParamsOut.clear();
            return false;
        }

        return true;
    }//end if:union

    return false;
}
