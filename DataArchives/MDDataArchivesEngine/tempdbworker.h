#ifndef TEMPDBWORKER_2016_07_18_H
#define TEMPDBWORKER_2016_07_18_H

#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QStringList>
#include <QSqlDatabase>

class IMDDataBaseParam;
class QSqlQuery;
class MDDBConnection;

class TempDBWorker : public QThread
{
    Q_OBJECT
public:
    explicit TempDBWorker(QObject *parent = 0);
    virtual ~TempDBWorker();
    //!此函数只能调用一次 tempDBName为临时数据库名称 调用者保证不能重名
    bool Start(const QString & tempDBName, const QString& projectPath);
    void Stop();
    void ModifyConnParam(IMDDataBaseParam * pDBConnParam);
    void SaveData(const QString & sqlStr);
protected:
    virtual void run() Q_DECL_OVERRIDE;
private:
    bool InitTempDB(const QString & tempDBName);
    void SetWorkDBConnInfoNolock();
    void SaveDataToTempDB();
    bool IsEmptyInTempDB();
    bool ImportToWorkDB();
private:
    volatile bool m_Stop;
    bool m_Init;
    QMutex m_Lock;
    QMutex m_StopLock;
    QWaitCondition m_StopConditon;
    //volatile bool m_SleepActive;
    bool m_WorkDBConnStatus;//只在一个线程中调用
    QSqlDatabase m_TempDBConnection;
    QSqlQuery * m_pTempDBInsertQuery;
    QSqlQuery * m_pTempDBBackupQuery;
    QSqlQuery * m_pTempDBCommonQuery;
    MDDBConnection * m_pWorkDBConnection;      //! 工作数据库链接
    //QSqlQuery * m_pWorkDBQuery;
    QStringList m_SQLStringList;
    QList<IMDDataBaseParam *> m_ConnList;
    QString m_TempDBName;//临时数据库名称 同时也是工作数据库名称
    QString m_projectPath;//项目路径
};

#endif // TEMPDBWORKER_H
