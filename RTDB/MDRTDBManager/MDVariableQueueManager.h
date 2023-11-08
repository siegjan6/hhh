/*!
\author enli 2015.01.08
\section 目前没使用
*/
#ifndef CMDVARIABLEQUEUEMANAGER_H
#define CMDVARIABLEQUEUEMANAGER_H

#include <QList>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include "IMDVariable.h"
class CMDVariableQueueManager
{
public:
    CMDVariableQueueManager();
    ~CMDVariableQueueManager();
//变量变化处理
public:
    //添加变化变量
    void add(IMDVariable* changedVariable);
    //移除变化变量
    void remove(IMDVariable* changedVariable);
    //获取并移除变化变量
    IMDVariable* removeAt(int index);
    //删除所有变化变量
    void removeAll();
    //获取变化变量
    IMDVariable* getAt(int index);
    //获取变化变量数量
    int getCount();
private:
    QList<IMDVariable*> m_variableList;//变化变量链表
    QMutex m_variableListMutex;
};

#endif // CMDVARIABLEQUEUEMANAGER_H
