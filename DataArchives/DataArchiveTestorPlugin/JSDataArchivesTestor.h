/*!
\author sjc

\section 功能
在JS脚本中引用此类的对象 调用此对象方法  实现测试功能
*/
#ifndef JSDATAARCHIVES_TESTOR_H
#define JSDATAARCHIVES_TESTOR_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QtXml/QDomDocument>
#include <MDDataArchivesEngine.h>
#include <MDDataArchivesNotifyParam.h>
#include <ITestorDataGroupMgr.h>
#include <IMDDataSourceManager.h>

struct CMDDataArchivesNotifyParamWrapper
{
   enum DataArchives::DB_NOTIFY_TYPE type;
    CMDDataArchivesNotifyParam * param;
};
struct JSDBParameters;
class TSQLExecutor;

class JSDataArchivesTestor : public QObject
{
    Q_OBJECT
public:
    explicit JSDataArchivesTestor(QObject *parent = 0);
    void SetParams(IMDDataArchives * pDataArchives, ITestorDataGroupMgr * pTestDataGroupMgr, IMDDataSourceManager * pDSourceMgr);
    Q_INVOKABLE bool selectTestDataCase(QString testDataCaseName);
    Q_INVOKABLE bool start();
    Q_INVOKABLE bool stop();
    Q_INVOKABLE void sleep(int millisecond);
    Q_INVOKABLE bool disconnectDBSource(QString dbSourceName);
    Q_INVOKABLE bool reconnectDBSource(QString dbSourceName);
    Q_INVOKABLE int    queryCount(const QString & dbSourceName, const QString  & sqlStr);
    /*!
     \section 处理一条测试数据
     */
    Q_INVOKABLE bool isTestDataItemOver();
    Q_INVOKABLE void saveTestDataItemAndAdvance();
    Q_INVOKABLE bool checkSaveResult();
signals:

public slots:
private:
    CMDDataArchivesEngine * m_pDataArchivesEngine;
    QDomDocument m_TestDataDomDoc;
    QList<CMDDataArchivesNotifyParamWrapper *> m_RecordWrapperList;
    QMap<QString, JSDBParameters *> m_oldDBParaMap; //原始连接信息：断开数据库连接时 保存初始的连接信息 用于恢复连接
    TSQLExecutor * m_pSqlExecutor;
    IMDDataArchives * m_pDataArchives;
    ITestorDataGroupMgr * m_pTestDataGroupMgr;
    IMDDataSourceManager * m_pDSourceMgr;
};

#endif // JSDATAARCHIVESENGINE_H
