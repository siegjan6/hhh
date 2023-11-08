#ifndef MDDATAARCHIVESCORE_H
#define MDDATAARCHIVESCORE_H

/*!

\author bHVvY2hhbw==

\section 功能
1.CMDDataArchivesEngine类的内部实现类。
2.对配置好的数据库的中插入、更新、删除数据。
3.对运行时更改数据库的参数。
4.对所有启用的数据源进行归档。

*/

#include <QList>
#include <QMap>
#include <QMutex>
#include "MDDataArchivesEngine.h"
#include "IMDDataTable.h"
#include "MDSysLogExpert.h"
#define LOG_SOURCE "DataArchives"

class CMDDBSourceService;
class IMDDataArchives;
class CMDDataArchivesNotifyParam;
class CMDMsgManager;
class IMDDataSource;

typedef QMap<QString, QMap<QString, QList<IMDDataTable*> > > MapSrcGrpTableList;

class CMDDataArchivesCore
{
public:
    CMDDataArchivesCore();
    ~CMDDataArchivesCore();

    /*!
    \section 运行时修改的参数类型
    */
    enum Settting_Param_type
    {
        DatabaseInvalid             = -1,
        DatabaseType                = 1,
        DatabaseHost                = 2,
        DatabasePort                = 3,
        DatabaseName                = 4,
        DatabaseUser                = 5,
        DatabasePassword            = 6
    };

    /*!
    \section 数据归档指针即项目加载时读取的数据归档所有的数据
    */
public:    
    void setDataArchives(IMDDataArchives *dataArchives){m_mdDataArchives = dataArchives;}

    /*!
    \section 每一个数据源是一个线程，对每一个线程的停止与启动
    */
public:
    bool start();//! 启动服务
    bool stop();//! 停止服务
    void load(const QString &projectPath);
    void runTimeLoad();//! 运行时加载

    /*!
    \section 运行时修改数据库对数，首先调用 startSetting接口。
    然后调用setDatabaseParam接口设置相应修改的参数。
    最后对所有的参数设置完成后，调用endSetting。
    */
public:
   //! 设置数据库参数接口，先调用start，然后调用设置接口，最后结束设置
    void startSetting();
    void endSetting();
    void setDatabaseParam(const QString &strDataSrcName, int type, const QString &strParam);

    /*!
    \section 异步处理数据库，对数据库插入、更新、删除操作
    */
public:
    //!  异步触发数据归档
    bool asyncCallDataArchives(DataArchives::DB_NOTIFY_TYPE nMSGType, CMDDataArchivesNotifyParam *pParam);

    //!  同步执行自定义sql
    int exec(const QString &strDataSrcName, const QString& sqlcmd);
    QString getLastError(const QString& datasourceName);

    /*!
    \section 查找数据源、删除所有数据源、删除所有服务
    */
private:
    IMDDataSource *findDataSource(const QString &strSrcName);
    void removeAllModifyDataSource();
    void removeAllService();

public:
    bool m_isRunning                                    ;  //! 服务运行标识
    bool m_iSettingFlag                                 ;  //! 运行参数设置标识
    IMDDataArchives *m_mdDataArchives                   ;   //! 数据归档
    QMutex m_Mutex;                                               //! 线程锁
    QList<IMDDataSource*> m_pModifyDataSrcList;                   //! 运行参数修改的数据源链表
    QList<CMDDBSourceService*> m_pDataSrcServerList;              //! 数据源服务链表
    MapSrcGrpTableList m_mapSrcGrpTab;                            //! 所有数据源相关对应的表的链表
};

#endif // MDDATAARCHIVESCORE_H
