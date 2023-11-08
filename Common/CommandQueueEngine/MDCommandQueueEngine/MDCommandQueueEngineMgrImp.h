/*!

\author dGFuZ3p3

\section 功能

命令引擎管理类的实现类，便于了解引擎的数量、用途等。

*/

#ifndef MDCOMMANDQUEUEENGINEMGRIMP_H
#define MDCOMMANDQUEUEENGINEMGRIMP_H

#include <QList>
#include <QMutex>

class CMDCommandQueueEngine;
class CMDCommandQueueEngineMgrImp
{
public:
    CMDCommandQueueEngineMgrImp();
    void reg(CMDCommandQueueEngine* engine);
    void unreg(CMDCommandQueueEngine* engine);
    void engines(QList<CMDCommandQueueEngine*>& engineList);
    CMDCommandQueueEngine* engineAt(int index);
    int count();
protected:
    QList<CMDCommandQueueEngine*> m_engineList;
    QMutex m_engineListMutex;
};

#endif // MDCOMMANDQUEUEENGINEMGRIMP_H
