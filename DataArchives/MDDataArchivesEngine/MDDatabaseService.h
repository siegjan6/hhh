#ifndef MDDATABASESERVICE_H
#define MDDATABASESERVICE_H

/*!

\author bHVvY2hhbw==

\section 功能
1.CMDCommandQueueEngine类的派生类。
2.每一个数据源即一个线程，也是一个服务。
3.通过onProcessCommand接口回调进行线程命令处理。
4.处理组织好的sql语句即对数据库进行sql操作。

*/

#include "MDDBConnect.h"
#include "MDDataArchivesNotifyParam.h"
#include "../../include/interface/Common/IMDDataType.h"
#include "MDCommandQueueEngine/MDCommandQueueEngine.h"
#include "MDSysLogExpert.h"
#include "IMDDatabaseParam.h"
#include <list>
#include <QMap>

using namespace std;


//==============================================================================

//#define LOG_SOURCE "DataArchives"

#define THREAD_QUIT_SLEEP_MS                            1000
//#define SQL_EXEC_SUM                                    200000

//==============================================================================

class QSqlQuery;
class CMDMsgManager;
class IMDDataSource;
class IMDDataArchives;
class CMDNotifyParamCMD;
class IMDDataBaseParam;
class IMDDataTable;
//class QTimer;
class TempDBWorker;
class TimerThread;
class MDDBConnection;

/*!
\section 控制命令类型，线程通过命令控制
*/
namespace DatabaseEngine {

    // 指令ID
    enum DB_CMD_TYPE
    {
        DB_CMD_INVALID              = -1,
        DB_CMD_START                = 1,                        //! 开始
        DB_CMD_STOP					= 2,                      //! 停止
        DB_CMD_OPERATE              = 3,                    //! 更新、插入、删除
        DB_CMD_MODIFY_DBPARAM		= 4,			//! 数据库参数修改指令
        DB_CMD_CHECK_DATABASE		= 5,			//! 数据库检查改指令
        DB_CMD_CHECK_CONNECTOIN = 6          //! 数据库连接检查命令 c2pj 216.07.28
    };

}

//===============================================================================
/*!
 * \brief 数据源服务 执行连接数据库、创建数据库、增删改命令
 */
class CMDDBSourceService : public CMDCommandQueueEngine
{
public:
    CMDDBSourceService();
    virtual ~CMDDBSourceService();
    /*!
    \section 回调函数，要在线程中处理，
             必须在这个回调函数内部完成，
             才能保证相应的处理是在当前类创建的线程内处理
    */
protected:
    virtual void onProcessCommand(CMDCommand* command);

    /*!
    \section 设置/获取属性
    */
public:    
    inline IMDDataSource *dataSource(){return m_pDBSource;}
    inline void setDBSource(IMDDataSource *pDbSource){m_pDBSource = pDbSource;} //! 设置/获取数据源

    inline void setDataArchives(IMDDataArchives *dataArchives){m_mdDataArchives = dataArchives;}
    inline IMDDataArchives *dataArchives(){return m_mdDataArchives;} //! 设置/获取数据归档

    /*!
    \section 对外调用接口，启用停止服务，异步通知处理数据库，执行sql语句，通知修改数据库参数
    */
public:
    bool startService(); //! 启动数据库服务
    bool stopService(); //! 停止数据库服务

    //!  异步触发数据归档
    bool asyncCallDataArchives(DataArchives::DB_NOTIFY_TYPE nMSGType, CMDDataArchivesNotifyParam *pParam);

    /*!
    \section 执行SQL
    */
    //去掉缓存预编译语句机制 原因见《缓存预编译语句导致的BUG.doc》
    //bool sqlExecute(const QString &strSQLPrepare, QList<QVariant> &varValueList);
    //!只执行插入SQL 若执行失败 内部会断开数据库连接
    bool executeInsertSQL(const QString &strSQLCmd);
    //!只执行非插入SQL 若执行失败
    bool executeCommonSQL(const QString &strSQLCmd);
    QString getLastError();

    //! 通知修改数据库参数 内部会释放pDataSrc
    bool notifyModifyDatabaseParam(IMDDataSource *pDataSrc);

    MDDBConnection *currdb(); //! 获取当前工作的数据库链接 //TODO:删除此API 破坏和封装性

    //! 根据指定通知的修改项类型以及值，将数据转换成sql语句字符串
    QString variantDataToString(MDDataType type, const QVariant &data);

    //!临时数据库接口
    void SaveSQLToTempDB(const QString & strSQLCmd);
    //!设置数据库连接状态 bStatus true为连接成功 false为连接失败
    void setDBConnectionStatus(bool bStatus);
    bool dbConnectionStatus();
    /*!
    \section 指针内存释放、服务命令控制、sql语句执行
    */
private:
    void release();

    //命令相关操作
    bool doStart();
    bool doStop();
    bool doCtrlServiceCmd(DatabaseEngine::DB_CMD_TYPE type);         
private:
    IMDDataSource * m_pDBSource           ;      //! 数据源 保存了数据源的配置信息 表信息
    IMDDataArchives * m_mdDataArchives    ;      //! 数据归档
    MDDBConnection * m_pWorkDBConnection    ;      //! 工作数据库链接
    TempDBWorker * m_pTempDBWorker;
    TimerThread * m_pTimerThread;
    volatile bool m_bDBConnectionStatus; //数据库连接状态 bStatus true为连接成功 false为连接失败
};

#endif // MDDATABASESERVICE_H
