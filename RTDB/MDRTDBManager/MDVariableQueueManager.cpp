#include "MDVariableQueueManager.h"

CMDVariableQueueManager::CMDVariableQueueManager()
{

}

CMDVariableQueueManager::~CMDVariableQueueManager()
{
    removeAll();
}

//移除所有变化变量
void CMDVariableQueueManager::removeAll()
{
    QMutexLocker locker(&m_variableListMutex);
    for(int i=0;i<m_variableList.count();i++)
    {
        delete m_variableList[i];
    }
    m_variableList.clear();
}
//添加变化变量
void CMDVariableQueueManager::add(IMDVariable* variable)
{
    if(NULL != variable)
    {
        QMutexLocker locker(&m_variableListMutex);
        m_variableList.append(variable);
    }
}
//移除变化变量
void CMDVariableQueueManager::remove(IMDVariable* variable)
{
    if(NULL != variable)
    {
        QMutexLocker locker(&m_variableListMutex);
        m_variableList.removeOne(variable);
    }
}
//获取并移除变化变量
IMDVariable*  CMDVariableQueueManager::removeAt(int index)
{
    IMDVariable* variable = NULL;
    QMutexLocker locker(&m_variableListMutex);
    if (index < m_variableList.count() && index>=0)
    {
        variable = m_variableList[index];
        m_variableList.removeAt(index);
    }
    return variable;
}
//获取变化变量
IMDVariable* CMDVariableQueueManager::getAt(int index)
{
    IMDVariable* variable = NULL;
    QMutexLocker locker(&m_variableListMutex);
    if (index < m_variableList.count() && index>=0)
    {
        variable = m_variableList[index];
    }
    return variable;
}
//获取变化变量数量
int CMDVariableQueueManager::getCount()
{
    int iCount = 0;
    QMutexLocker locker(&m_variableListMutex);
    iCount = m_variableList.count();
    return iCount;
} 
