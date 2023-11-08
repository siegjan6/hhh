/*!

\author dGFuZ3p3

\section 功能 - 日志记录器类的实现类，实现日志记录，具体功能如下：
 、记录日志到内存链表和文本文件（.txt）；
 、查询内存链表，获取最近的日志信息；
 、可以配置为“直接记录”或“引擎记录”模式：
     直接记录：logXX 函数的调用将立即在链表或文件中生成日志信息；
     引擎记录：logXX 函数的调用会将日志提交给一个引擎，该引擎将在专门的
                      线程中处理日志，该线程会在链表或文件中生成日志信息；
     二者的区别在于“引擎记录”将提交和处理分开，实现了快速提交，
     而相对耗时的处理过程不影响调用线程。

\section 线程特性

、此类的函数都是线程安全的。

\section 用法

、必须通过 CMDSysLogExpert 创建，参考 MDSysLogExpert.h；
、通过 log 从内存链表获取的日志记录指针，不应该长期保留，保留时间不应超过 3 分钟；
原因是内部有垃圾处理机制，凡是超出链表最大长度的日志记录将被转交给垃圾处理器，
垃圾处理器至少保留 3 分钟，然后将其删除 （delete）。

\section 修改历史

2015.4 / dGFuZ3p3 / 创建；
2015.4.9 / dGFuZ3p3 / 功能完成并写注释；
2015.4.12 / dGFuZ3p3 / 增加了“引擎记录”模式。

*/

#ifndef MDSYSLOGGERIMP_H
#define MDSYSLOGGERIMP_H

#include "log4qt/logger.h"
#include "MDSysLogData.h"
#include "MDSysLogger.h"

#include "log4qt_for_MindSCADA/layout_MindSCADA.h"
#include "log4qt_for_MindSCADA/rollingfileappender_MindSCADA.h"
#include "log4qt_for_MindSCADA/listappender_MindSCADA.h"

#include <QString>
/*!
 * \brief 日志记录器参数类的实现类
 */
class CMDSysLoggerParaImp
{
public:
    CMDSysLoggerParaImp();
    CMDSysLoggerParaImp(const CMDSysLoggerParaImp& obj);
    CMDSysLoggerParaImp& operator=(const CMDSysLoggerParaImp& obj);
public:
    QString m_name; //! 记录器的名称
    QString m_comment; //! 对此记录器的描述
public:
    bool m_enableListAppender; //! 是否允许向内存链表添加日志记录
    int m_maxCountInList; //! 内存链表最大长度
public:
    bool m_enableFileAppender; //! 是否允许向文本文件添加日志记录
    QString m_fileDir; //! 文件存储主目录（缺省：应用程序路径下的子目录 logs）
    QString m_subFileDir; //! 文件存储子目录（缺省为空，即文件直接存储在主目录下）
    QString m_fileMainName; //! 主文件名（后缀固定为 .txt）
    int m_maxBackupFileIndex; //! 最大文件数，只保留最近的指定个数的文件
    //! 最大文件字节大小（缺省：1*1024*1024）
    //! 一旦到达此大小，则文件将被改名，例如，从 logs.txt 改为 logs_1.txt，
    //! 系统另外创建一个名为 nslog.txt 的文件，记录后续日志。
    qint64 m_maxFileSize;
public:
    static QString buildDefaultFileDir();
public:
    //! 日志引擎类型（缺省：不使用引擎）
    //! 缺省不使用引擎，则每条日志记录的过程都是在调用线程内执行，
    //! 如果记录频繁，则可能显著影响调用线程
    CMDSysLoggerPara::LogEngineType m_engineType;
public:
    bool m_enableOutput; //! 是否允许输出日志，如果 = false，则不会向任何 Appender 输出日志；
                                         //! 运行时可通过此开关来禁止/允许所有日志输出
};

class CMDSysLogEngine;
class CMDSysLoggerPara;
/*!
 * \brief 日志记录器类的实现类
*/
class CMDSysLoggerImp
{
    friend class CMDSysLogExpertImp;
public:
    CMDSysLoggerImp();
    ~CMDSysLoggerImp();
/*!
\section 初始化与释放
*/
public:
    bool init(Log4Qt::Logger* logger, const CMDSysLoggerPara& para);
    void release();
    void initEngine(const CMDSysLoggerPara& para);
    void releaseEngine();
    static CMDSysLogData* helper_newLogData(const Log4Qt::LoggingEvent& logging, CMDSysLogData* recycleLog);
    static Log4Qt::MindSCADALayout* newLayout();
public:
    QString makeSureFileDirExist(const QString& fileDir);
    bool helper_makeSureFileDirExist(const QString& fileDir);

/*!
\section 记录器参数
*/
public:
    CMDSysLoggerPara m_para;

/*!
\section 记录到日志；访问最近的日志
*/
public:
    void logInfo(const QString& msg, const QString& source = "");
    void logWarn(const QString& msg, const QString& source = "");
    void logError(const QString& msg, const QString& source = "");

    void processLog(Log4Qt::Level level, const QString& msg, const QString& source = "");
public:
    int logCount();
    CMDSysLogData* log(int index);
protected:
    Log4Qt::Logger* m_Log4Qt_Logger;
    //! 记录了内存链表的指针，快速访问最近日志记录
    //! （不需要每次都按名字查找追加器）
    Log4Qt::MindSCADAListAppender* m_listAppender;

protected:
    CMDSysLogEngine* m_logEngine;
};

#endif // MDSYSLOGGERIMP_H
