#include "MDCommandQueueEngineThread.h"
#include "MDCommandQueueEngineImp.h"

CMDCommandQueueEngineThread::CMDCommandQueueEngineThread(QObject *parent) :
    QThread(parent)
{
    m_engine = NULL;
}

CMDCommandQueueEngineThread::~CMDCommandQueueEngineThread()
{
}

void CMDCommandQueueEngineThread::run()
{
    if(m_engine)
        m_engine->new_Handler();

    // QScopedPointer<CNotificationHandler> handlerSP(m_handler);

    exec();
}
