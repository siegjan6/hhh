#include "../../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"
#include "MDCommandQueueEngineImp.h"
#include "MDCommandData.h"
#include "MDCommandQueueEngineMgr.h"
#include "MDCommandQueueEngineMgrImp.h"

/*!
 * \brief 引擎命令数据类
cmdType：类型，派生类自行解释其含义；
                    （32700 - 32767是系统保留的类型，外部不能使用）
timeOnCreated：创建时间，命令被创建的时间，在 CMDCommand 中
                                被赋值（一般情况下，外部不应该修改该值）；
timeOnProcessed：被处理的时间，在 onProcessCommand 之后
                                    被赋值，用于计算延迟时间
                                   （一般情况下，外部不应该修改该值）；
cmdPara：命令参数，外部定义。
 */
CMDCommand::CMDCommand()
{
    m_data = new CMDCommandData();
}

CMDCommand::CMDCommand(void* cmdPara)
{
    m_data = new CMDCommandData();
    if(m_data)
        m_data->m_para = cmdPara;
}

CMDCommand::~CMDCommand()
{
    if(m_data)
        delete m_data;
}

qint16 CMDCommand::cmdType() const
{
    return (m_data ? m_data->m_type : -1);
}

void CMDCommand::setCmdType(qint16 cmdType)
{
    if(m_data)
        m_data->m_type = cmdType;
}

QDateTime CMDCommand::timeOnCreated() const
{
    return (m_data ? m_data->m_timeOnCreated : QDateTime());
}

void CMDCommand::setTimeOnCreated(QDateTime timeOnCreated)
{
    if(m_data)
        m_data->m_timeOnCreated = timeOnCreated;
}

QDateTime CMDCommand::timeOnProcessed() const
{
    return (m_data ? m_data->m_timeOnProcessed : QDateTime());
}

void CMDCommand::setTimeOnProcessed(QDateTime timeOnProcessed)
{
    if(m_data)
        m_data->m_timeOnProcessed = timeOnProcessed;
}

void* CMDCommand::cmdPara() const
{
    return (m_data ? m_data->m_para : NULL);
}

void CMDCommand::setCmdPara(void* cmdPara)
{
    if(m_data)
        m_data->m_para = cmdPara;
}

/*!
 * \brief 命令队列引擎类
 */
CMDCommandQueueEngine::CMDCommandQueueEngine()
{
    m_imp = new CMDCommandQueueEngineImp(this);

    CMDCommandQueueEngineMgr::reg(this);
}

CMDCommandQueueEngine::~CMDCommandQueueEngine()
{
    if(m_imp)
        delete m_imp;

    CMDCommandQueueEngineMgr::unreg(this);
}

/*!
\section 设置引擎注释
强烈建议设置引擎注释，便于通过内部的引擎管理器了解引擎实例的数量和用途；
例如，事件引擎的注释为 "Event.EventEgnige"
*/
QString CMDCommandQueueEngine::comment()
{
    return (m_imp ? m_imp->m_comment : "");
}

void CMDCommandQueueEngine::setComment(const QString& comment)
{
    if(m_imp)
        m_imp->m_comment = comment;
}

/*!
\section 引擎的启动/停止
start：启动一个内部线程，命令的处理代码将在该线程内执行；
startInCurrentThread：不启动额外的线程，命令的处理代码将在
                                  调用 startInCurrentThread 函数的线程内执行。
\note 只能使用 start 和 startInCurrentThread 之一来启动引擎。
*/
bool CMDCommandQueueEngine::start()
{
    return (m_imp ? m_imp->start() : false);
}

bool CMDCommandQueueEngine::startInCurrentThread()
{
    return (m_imp ? m_imp->startInCurrentThread() : false);
}

void CMDCommandQueueEngine::stop(unsigned long timeout)
{
    if(m_imp)
        m_imp->stop(timeout);
}

bool CMDCommandQueueEngine::isRunning()
{
    return (m_imp ? m_imp->isRunning() : false);
}

MDCommandEngineStatus CMDCommandQueueEngine::status()
{
    return (m_imp ? m_imp->status() : MDCommandEngineStatus::stopped);
}

QDateTime CMDCommandQueueEngine::startTime()
{
    return m_imp->m_startTime;
}

/*!
\section 命令提交与处理
必须重载 onProcessCommand 实现对命令的具体处理；
submitCommand：线程安全，可从任何线程调用；
submitPriorCommand：线程安全，可从任何线程调用；
onProcessCommand：在内部线程或外部线程被调用，参考 start 和 startInCurrentThread；
队列长度限制：等待队列的最大长度是 40K = 1024 * 40，
                      若已达此长度，则后续命令被直接丢弃。
*/
bool CMDCommandQueueEngine::submitCommand(CMDCommand* command)
{
    return (m_imp ? m_imp->submitCommand(command) : false);
}

bool CMDCommandQueueEngine::submitPriorCommand(CMDCommand* command)
{
    return (m_imp ? m_imp->submitPriorCommand(command) : false);
}

void CMDCommandQueueEngine::cancelAllWaiting()
{
    if(m_imp)
        m_imp->cancelAllWaiting();
}

void CMDCommandQueueEngine::onProcessCommand(CMDCommand* command)
{
}

qint64 CMDCommandQueueEngine::totalSubmit()
{
    return m_imp->m_totalSubmit;
}

qint64 CMDCommandQueueEngine::totalSubmitHistory()
{
    return m_imp->m_totalSubmitHistory;
}

/*!
\section 管理【等待处理的命令】
*/
int CMDCommandQueueEngine::sizeOfWaitingQueue()
{
    return (m_imp ? m_imp->m_processingQueueManager.sizeOfWaitingQueue() : 0);
}

int CMDCommandQueueEngine::sizeOfProcessingQueue()
{
    return (m_imp ? m_imp->m_processingQueueManager.sizeOfProcessingQueue() : 0);
}

int CMDCommandQueueEngine::sizeOfPriorWaitingQueue()
{
    return (m_imp ? m_imp->m_processingQueueManager.sizeOfPriorWaitingQueue() : 0);
}

/*!
\section 管理【已经处理的命令】
缺省管理方式：延迟 5 秒后删除命令
*/
qint64 CMDCommandQueueEngine::totalProcessed()
{
    return (m_imp ? m_imp->m_processedQueueManager.totalProcessed() : 0);
}

qint64 CMDCommandQueueEngine::totalProcessedHistory()
{
    return (m_imp ? m_imp->m_processedQueueManager.totalProcessedHistory() : 0);
}

int CMDCommandQueueEngine::sizeOfProcessedQueue()
{
    return (m_imp ? m_imp->m_processedQueueManager.sizeOfProcessedQueue() : 0);
}

int CMDCommandQueueEngine::sizeOfRecycleQueue()
{
    return (m_imp ? m_imp->m_processedQueueManager.sizeOfRecycleQueue() : 0);
}

MDModeToHandleProcessedCommand CMDCommandQueueEngine::modeToHandleProcessed()
{
    return (m_imp ? m_imp->m_processedQueueManager.modeToHandleProcessed() : MDModeToHandleProcessedCommand::null_mode);
}

void CMDCommandQueueEngine::setModeToHandleProcessed(MDModeToHandleProcessedCommand mode)
{
    if(m_imp)
        m_imp->m_processedQueueManager.setModeToHandleProcessed(mode);
}

bool CMDCommandQueueEngine::isDelayHandleProcessed()
{
    return (m_imp ? m_imp->m_processedQueueManager.isDelayHandleProcessed() : false);
}

void CMDCommandQueueEngine::setDelayHandleProcessed(bool flag)
{
    if(m_imp)
        m_imp->m_processedQueueManager.setDelayHandleProcessed(flag);
}

int CMDCommandQueueEngine::delayTimeLength()
{
    return (m_imp ? m_imp->m_processedQueueManager.delayTimeLength() : 5000);
}

void CMDCommandQueueEngine::setDelayTimeLength(int timeLength)
{
    if(m_imp)
        m_imp->m_processedQueueManager.setDelayTimeLength(timeLength);
}

bool CMDCommandQueueEngine::isRecycleEnabled()
{
    return (m_imp ?
                (MDModeToHandleProcessedCommand::mode_recycle == m_imp->m_processedQueueManager.modeToHandleProcessed()) :
                false);
}

CMDCommand* CMDCommandQueueEngine::fetchRecycleCommand()
{
    return (m_imp ? m_imp->m_processedQueueManager.fetchRecycleCommand() : NULL);
}

/*!
 调整回收队列的最大容量，在处理高吞吐量的任务时，
 可以调整此参数获得最佳性能。
 */
qint32 CMDCommandQueueEngine::maxRecycleQueueLength()
{
    return m_imp->m_processedQueueManager.maxRecycleQueueLength();
}

void CMDCommandQueueEngine::setMaxRecycleQueueLength(qint32 maxRecycle)
{
    m_imp->m_processedQueueManager.setMaxRecycleQueueLength(maxRecycle);
}
