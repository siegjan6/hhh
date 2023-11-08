#include "MDStdTimer.h"
#include "MDProjectExpertImp.h"

CMDStdTimer::CMDStdTimer(QObject *parent) :
    QObject(parent)
{
    m_projectExpert = NULL;
}

void CMDStdTimer::start(int msec)
{
    connect(&m_stdTimer, SIGNAL(timeout()), this, SLOT(doTimeout()));
    m_stdTimer.start(msec);
}

void CMDStdTimer::stop()
{
    m_stdTimer.stop();
}

void CMDStdTimer::doTimeout()
{
    if(m_projectExpert)
        m_projectExpert->onTimeout_FromStdTimer();
}
