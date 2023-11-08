/*!

\author dGFuZ3p3

\section 功能 - 命令队列引擎类，实现通用的命令接收与处理流程，具体功能如下：
、应用场景：接收线程和处理线程是不同的线程，此引擎才有应用的意义；
、持续不断地接收和处理命令，且接收过程不被处理过程所阻塞；
、可从多个线程接收命令，但处理线程只有一个；
、优先命令优先处理：一旦当前命令处理完毕，优先命令会被立即处理；
、对已经处理的命令，可删除或回收，回收的命令可以被再次利用；
、对已经处理的命令，可以延迟删除或回收，例如，延迟 5 秒。

\section 线程特性
、命令的提交是线程安全的，可从任何线程调用，参考 submitCommand 和
    submitPriorCommand；
、命令的处理在引擎的一个内部线程内执行，通过 onProcessCommand 通知
外部进行具体的处理；
、引擎的启动和停止是线程安全的，参考  start() 和 stop()；

\section 用法
、必须从 CMDCommandQueueEngine 派生一个类，并重载 onProcessCommand，
  重载的 onProcessCommand 实现对命令的具体处理；
、交由引擎处理的必须是 CMDCommand 的派生类，且必须通过 new 创建；
、一旦将命令提交给引擎，引擎会在适当的时候将命令删除或回收，外部访问已提交的对象
 是不安全的；
、一旦 onProcessCommand 执行完毕，命令对象就称为“已经处理的命令”，
 根据配置，“已经处理的命令”会被删除或回收，该过程可以【立即】或【延迟】执行，
 【立即】删除：命令对象的内存会被立即释放，绝对不可以被再次使用；
 【立即】回收：命令对象立即进入回收队列，等待被再次使用，对象状态随时可能被改变；
 【延迟】删除：命令对象的内存会被延迟一段时间后释放，延迟期间对象有效且状态不变；
 【延迟】回收：命令对象延迟一段时间后进入回收队列，等待被再次使用，
                         延迟期间对象有效且状态不变；
、如果允许对“已经处理的命令”进行回收，则可以通过 fetchRecycleCommand 获取
  回收命令，重新对其赋值之后可以再次交由引擎处理；如果没有获取到回收命令，则仍然
  必须通过 new 创建新命令；
、调用 stop() 停止引擎时，存在强制终止线程的可能，一旦强制终止线程，则线程持有的锁
  无法释放，极有可能引起“死锁”；因此，建议尽量在整个程序退出时才调用 stop()，减小
  强制终止线程造成的影响。
、避免内存泄漏：如果要使用 CMDCommand 的 setCmdPara 传递指针，则必须
  从 CMDCommand 派生一个类，且必须实现一个虚的析构函数，并在析构函数中释放
  所使用的指针，这样才能确保不发生内存泄漏；
  因为内部在队列长度（如【等待队列】）超过最大值时，会自动删除命令对象，但删除的是
  CMDCommand* 指针，如果不派生类，或者派生类不在析构函数里删除参数指针，内部是无法
  删除外部产生的对象的。

\section 性能
、对命令的接收和处理都是持续不断地进行，处理速度取决于机器性能；
、删除“已经处理的命令”：对每个命令都存在 new 和 delete 的开销；
、回收“已经处理的命令”：如果空闲的回收命令足够多，则没有 new 和 delete 的开销，
                                       但获取回收命令存在锁定回收队列的开销，因此到底哪种策略更好，
                                       只能在具体的应用场景下分析。

\section 修改历史
2015.2 / dGFuZ3p3 / 创建；
2015.2.28 / dGFuZ3p3 / 功能完成并写注释；
2015.3.26 ~ 29 / dGFuZ3p3 / 关键性的修改：
                               、确保引擎的启动和停止是线程安全的；
                               、完善了需要加锁的代码，确保线程安全；
                               、对命令的处理，尽量避免了强制终止线程造成的“死锁”问题；
                               、尽量避免强制终止引擎线程。
2015.6.2/ dGFuZ3p3 / 增加了 startInCurrentThread 接口，外部可指定命令的处理线程；
2015.7.21/ dGFuZ3p3 / 增加了【优先命令队列】，优先命令优先处理，参考 submitPriorCommand；
                                可取消正在等待处理的命令，参考 cancelAllWaiting；
2016.2.23/ dGFuZ3p3 / 增大了【等待队列】的最大长度（高负载时尽量不丢命令）；
                                为【已处理队列】和【回收队列】设置了最大长度，
                                超过此长度，则删除最早进入队列的命令；
                                用户可调整【回收队列】的最大长度以获取最佳性能。
*/

#ifndef MDCOMMANDQUEUEENGINE_H
#define MDCOMMANDQUEUEENGINE_H

#include "MDCommandQueueEngine_Global.h"
#include <QDateTime>

enum class MDModeToHandleProcessedCommand
{
    null_mode  = 0,
    mode_delete  = 1,
    mode_recycle  = 2
};

enum class MDCommandEngineStatus
{
    stopped  = 0,
    starting  = 1,
    started  = 2,
    stopping = 3
};

class CMDCommandData;
/*!
 * \brief 引擎命令数据类
cmdType：类型，派生类自行解释其含义；
                    （32700 - 32767是系统保留的类型，外部不能使用）
// cmdType
// 0 - RawAlarmEvent
// 1 - confirmDirect
// 2 - buildConditionsFromCloud
// 3 - acknowledgeDirect
// 4 - suppress
// 5 - shelve
// 6 - add comment

timeOnCreated：创建时间，命令被创建的时间，在 CMDCommand 中
                                被赋值（一般情况下，外部不应该修改该值）；
timeOnProcessed：被处理的时间，在 onProcessCommand 之后
                                    被赋值，用于计算延迟时间
                                   （一般情况下，外部不应该修改该值）；
cmdPara：命令参数，外部定义。
 */
class MDCOMMANDQUEUEENGINESHARED_EXPORT CMDCommand
{
public:
    CMDCommand(void* cmdPara);
    CMDCommand();
    virtual ~CMDCommand();
public:
    qint16 cmdType() const;
    void setCmdType(qint16 cmdType);
public:
    QDateTime timeOnCreated() const;
    void setTimeOnCreated(QDateTime timeOnCreated);
public:
    QDateTime timeOnProcessed() const;
    void setTimeOnProcessed(QDateTime timeOnProcessed);
public:
    void* cmdPara() const;
    void setCmdPara(void* cmdPara);
protected:
    CMDCommandData* m_data;
};

class CMDCommandQueueEngineImp;
/*!
 * \brief 命令队列引擎类
 */
class MDCOMMANDQUEUEENGINESHARED_EXPORT CMDCommandQueueEngine
{
public:
    CMDCommandQueueEngine();
     virtual ~CMDCommandQueueEngine();

/*!
\section 设置引擎注释
强烈建议设置引擎注释，便于通过内部的引擎管理器了解引擎实例的数量和用途；
例如，事件引擎的注释为 "Event.EventEgnige"
*/
public:
    QString comment();
    void setComment(const QString& comment);    //! 设置注释，表明引擎的用途

/*!
\section 引擎的启动/停止
start：启动一个内部线程，命令的处理代码将在该线程内执行；
startInCurrentThread：不启动额外的线程，命令的处理代码将在
                                  调用 startInCurrentThread 函数的线程内执行。
\note 只能使用 start 和 startInCurrentThread 之一来启动引擎。
*/
public:
    bool start();
    bool startInCurrentThread();
    void stop(unsigned long timeout = 0);
    bool isRunning();
public:
    MDCommandEngineStatus status();
public:
    QDateTime startTime(); //! 最近一次的启动时间

/*!
\section 命令提交与处理
必须重载 onProcessCommand 实现对命令的具体处理；
submitCommand：线程安全，可从任何线程调用；
submitPriorCommand：线程安全，可从任何线程调用；
onProcessCommand：在内部线程或外部线程被调用，参考 start 和 startInCurrentThread；
队列长度限制：【等待队列】和【优先等待队列】的最大长度是 40K = 1024 * 40，
                      若已达此长度，则后续命令被直接丢弃。
*/
public:
    bool submitCommand(CMDCommand* command);            //! 提交命令到【普通等待队列】
    bool submitPriorCommand(CMDCommand* command);   //! 提交命令到【优先等待队列】
public:
    qint64 totalSubmit(); //! 尝试提交的普通命令总数，每次启动后都重置为 0
    qint64 totalSubmitHistory(); //! 尝试提交的普通命令历史总数，一直累加
public:
    void cancelAllWaiting(); //! 取消正在等待处理的命令
public:
    virtual void onProcessCommand(CMDCommand* command);

/*!
\section 管理【等待处理的命令】
*/
public:
    int sizeOfWaitingQueue();
    int sizeOfProcessingQueue();
public:
    int sizeOfPriorWaitingQueue();

/*!
\section 管理【已经处理的命令】（缺省：处理后的命令立即进入回收队列）
*/
public:
    qint64 totalProcessed(); //! 已处理的命令总数，每次启动后都重置为 0
    qint64 totalProcessedHistory(); //! 已处理的命令历史总数，一直累加
public:
    int sizeOfProcessedQueue();
    int sizeOfRecycleQueue();
    CMDCommand* fetchRecycleCommand(); //! 获取一个已经回收的命令
public:
    /*!
     调整回收队列的最大容量，在处理高吞吐量的任务时，
     可以调整此参数获得最佳性能。
     */
    qint32 maxRecycleQueueLength(); //! 回收队列最大长度 [缺省：1024]
    void setMaxRecycleQueueLength(qint32 maxRecycle);
public:
    bool isRecycleEnabled(); //! 允许回收吗？ [缺省：回收]
    MDModeToHandleProcessedCommand modeToHandleProcessed();
    void setModeToHandleProcessed(MDModeToHandleProcessedCommand mode);
public:
    bool isDelayHandleProcessed(); //! 是否延迟？ [缺省：不延迟，立即处理]
    void setDelayHandleProcessed(bool flag);

    int delayTimeLength(); //! 延迟时间 [缺省：5秒]
    void setDelayTimeLength(int timeLength); //! 设置延迟的毫秒数

protected:
    CMDCommandQueueEngineImp* m_imp;
};

#endif // MDCOMMANDQUEUEENGINE_H
