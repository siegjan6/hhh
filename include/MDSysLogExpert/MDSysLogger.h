/*!

\author dGFuZ3p3

\section 功能 - 日志记录器类，实现日志记录，具体功能如下：
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
、通过 log 从内存链表获取的日志记录指针，不应该长期保留。

\section 性能

、日志记录也有开销，特别是记录到文件时，可能会显著降低系统的处理性能；
    可以通过 enableOutput 开关来测试日志对系统处理性能的影响。

\section 修改历史

2015.4 / dGFuZ3p3 / 创建；
2015.4.9 / dGFuZ3p3 / 功能完成并写注释；
2015.4.12 / dGFuZ3p3 / 增加了“引擎记录”模式；
2015.7.15 / dGFuZ3p3 / 可以访问 logger 的参数，参考 CMDSysLogger::para 。
2016.3.29 / dGFuZ3p3 / 增加了日志输出开关。
*/

#ifndef MDSYSLOGGER_H
#define MDSYSLOGGER_H

#include "MDSysLogExpert_Global.h"
#include "MDSysLogData.h"
#include <QString>

class CMDSysLoggerParaImp;
/*!
 * \brief 日志记录器参数类
 * 被 CMDSysLogExpert::init 和 CMDSysLogExpert::createLogger 使用
 */
class MDSYSLOGEXPERTSHARED_EXPORT CMDSysLoggerPara
{
public:
    CMDSysLoggerPara();
    CMDSysLoggerPara(const CMDSysLoggerPara& obj);
    ~CMDSysLoggerPara();
    CMDSysLoggerPara& operator=(const CMDSysLoggerPara& obj);
public:
    enum LogEngineType
    {
        Default = 0,           //! 缺省引擎
        Independant = 1,  //! 独立引擎
        NoEngine = 2       //! 不使用引擎
    };
public:
    //! 记录器的名称（缺省：空）
    QString name() const;
    void setName(const QString& name);

    //! 对此记录器的描述（缺省：空）
    QString comment() const;
    void setComment(const QString& comment);
public:
    //! 是否允许向内存链表添加日志记录（缺省：禁止）
    bool isEnableListAppender() const;
    void enableListAppender(bool enable);

    //! 内存链表最大长度（缺省：2048）
    int maxCountInList() const;
    void setMaxCountInList(int maxCount);
public:
    //! 是否允许向文本文件添加日志记录（缺省：允许）
    bool isEnableFileAppender() const;
    void enableFileAppender(bool enable);

    //! 文件存储主目录（缺省：应用程序路径下的子目录 logs）
    QString fileDir() const;
    void setFileDir(const QString& fileDir);

    //! 文件存储子目录（缺省：空，即文件直接存储在主目录下）
    QString subFileDir() const;
    void setSubFileDir(const QString& subFileDir);

    //! 主文件名（后缀固定为 .txt）（缺省：logs）
    QString fileMainName() const;
    void setFileMainName(const QString& fileDir);

    //! 最大文件数，只保留最近的指定个数的文件（缺省：3）
    int maxBackupFileIndex() const;
    void setMaxBackupFileIndex(int maxIndex);

    //! 最大文件字节大小（缺省：1*1024*1024）
    //! 一旦到达此大小，则文件将被改名，例如，从 logs.txt 改为 logs_1.txt，
    //! 系统另外创建一个名为 logs.txt 的文件，记录后续日志。
    qint64 maxFileSize() const;
    void setMaxFileSize(qint64 maxFileSize);

public:
    //! 日志引擎类型（缺省：不使用引擎）
    //! 缺省不使用引擎，则每条日志记录的过程都是在调用线程内执行，
    //! 如果记录频繁，则可能显著影响调用线程
    LogEngineType logEngineType() const;
    void setLogEngineType(LogEngineType type);
public:
    //! 是否允许输出日志，如果 = false，则不会向任何 Appender 输出日志；
    //! 运行时可通过此开关来禁止/允许所有日志输出（缺省：允许）
    bool isEnableOutput() const;
    void enableOutput(bool enable);
protected:
    CMDSysLoggerParaImp* m_imp;
};

class CMDSysLoggerImp;
class CMDSysLogData;
/*!
 * \brief 日志记录器类
*/
class CMDCommandQueueEngine;
class MDSYSLOGEXPERTSHARED_EXPORT CMDSysLogger
{
    friend class CMDSysLogExpertImp;
public:
    CMDSysLogger();
    ~CMDSysLogger();
public:
    CMDSysLoggerPara& para() const;
public:
    void logInfo(const QString& msg, const QString& source = "");
    void logWarn(const QString& msg, const QString& source = "");
    void logError(const QString& msg, const QString& source = "");
public:
    int logCount();
    CMDSysLogData* log(int iIndex);
protected:
    CMDSysLoggerImp* m_imp;
};

#endif // MDSYSLOGGER_H
