/*!
\author sjc

\section 功能
在JS脚本中引用此类的对象 调用此对象方法  实现测试功能
*/
#ifndef JS_QUERY_TESTOR_2016_09_06_H
#define JS_QUERY_TESTOR_2016_09_06_H

#include <QObject>
#include <QString>
//#include <QList>
//#include <QMap>
#include <QtXml/QDomDocument>
#include <IMDDBQueryService.h>
#include <ITestorDataGroupMgr.h>

class IMDDBQueryService;
class IMDDBQueryParam;
class IMDDataArchives;

class JSQueryTestorDBResultNofity : public IMDDBResultNotify
{
public:
    void NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *);
};

class JSQueryTestor : public QObject
{
    Q_OBJECT
public:
    explicit JSQueryTestor(QObject *parent = 0);
    void setParams(IMDDataArchives * pDataArchives, ITestorDataGroupMgr * pTestDataGroupMgr);
    Q_INVOKABLE bool selectTestQueryCase(QString testQueryCaseName);
    //Q_INVOKABLE bool start();
    //Q_INVOKABLE bool stop();
    Q_INVOKABLE void sleep(int millisecond);
    Q_INVOKABLE bool query();
signals:

public slots:
private:
    bool ConfigQueryParams(QList<IMDDBQueryParam *> & queryParamsOut, DB_QUERY::DB_QUERY_MODE queryMode);
private:
    IMDDBQueryService * m_pDBQueryService;
    IMDDataArchives * m_pIMDDataArchives;
    ITestorDataGroupMgr * m_pTestDataGroupMgr;
    QList<IMDDBQueryParam *> m_QueryParams;
    JSQueryTestorDBResultNofity * m_pTestorDBResultNofity;
    //CMDDataArchivesEngine * m_pDataArchivesEngine;
    //QDomDocument m_TestQueryDomDoc;
    //QList<CMDDataArchivesNotifyParamWrapper *> m_RecordWrapperList;
    //QMap<QString, JSDBParameters *> m_oldDBParaMap; //原始连接信息：断开数据库连接时 保存初始的连接信息 用于恢复连接
    //TSQLExecutor * m_pSqlExecutor;
    //IMDDataArchives * m_pDataArchives;
    //ITestorDataGroupMgr * m_pTestDataGroupMgr;
    //IMDDataSourceManager * m_pDSourceMgr;
};

#endif // JSDATAARCHIVESENGINE_H
