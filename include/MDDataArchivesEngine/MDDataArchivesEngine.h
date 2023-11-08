#ifndef MDDATAARCHIVESENGINE_H
#define MDDATAARCHIVESENGINE_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据归档外部调用接口类。
2.对配置好的数据库的中插入、更新、删除数据。
3.对运行时更改数据库的参数。
4.对所有启用的数据源进行归档。

*/

#include "mddataarchivesengine_global.h"
#include "MDDataArchivesNotifyParam.h"

class CMDDataArchivesCore;
class IMDDataArchives;
class CMDCommandQueueEngine;


class MDDATAARCHIVESENGINESHARED_EXPORT CMDDataArchivesEngine
{
public:
    CMDDataArchivesEngine();
    ~CMDDataArchivesEngine();

    /*!
    \section 数据归档指针即项目加载时读取的数据归档所有的数据
    */
public:
    void setDataArchives(IMDDataArchives *dataArchives);

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

    //! 同步执行自定义sql
    int exec(const QString &strDataSrcName, const QString& sqlcmd);
    QString getLastError(const QString& datasourceName);

private:
    CMDDataArchivesCore *m_mdDataArchivesCore; //! 数据归档内部实现封装类
};

#endif // MDDATAARCHIVESENGINE_H
