#include "MDDBConnCache.h"
#include <algorithm>
#include <QMutexLocker>
#include "DBConnectionTemp.h"
//查找连接时 可能存在多个相同连接参数的连接 返回第一个连接参数匹配的连接

struct PredictOp
{
    IMDDataBaseParam * pParam;
    bool operator()(CDBConnectionTemp * pConn)
    {
        if (!pConn)
            return false;

        if(pConn->compareConnectionParam(pParam))
                return true;

        return false;
    }
    //D:\MindSCADA\code\include\MDDataArchivesInterface\IMDDatabaseParam.h

};

//这个比较函数用于从已使用队列中移除连接 相等条件为指针相同
//相等条件不是参数相等 因为存在多个参数相同的连接.导致移错对象.
struct PredictOp2
{
    bool operator()(CDBConnectionTemp * p)
    {
        return p == pCon;
    }
    CDBConnectionTemp * pCon;
};

MDDBConnCache * MDDBConnCache::m_pConnCache = 0;

void MDDBConnCache::Init()
{
    if(!m_pConnCache)
        m_pConnCache = new MDDBConnCache;

}

MDDBConnCache * MDDBConnCache::GetInst()
{
    return m_pConnCache;
}

void MDDBConnCache::DestroyCache()
{
    delete m_pConnCache;
    m_pConnCache = 0;
}

//前提假设:一个连接要么存在于未使用队列 要么存在于以使用队列
//前提假设:一个连接只能被Pull一次
//前提假设:一个连接只能被Push一次
//成功获得一个连接的情况:1.存在此连接 且未被标记成删除 2.新建连接 且连接数据库成功
//被标记成删除的连接 只在PullConnection中删除


//在连接池中获得一个连接
CDBConnectionTemp * MDDBConnCache::PullConnection(IMDDataBaseParam * pDBConnParam)
{
    if(!pDBConnParam)
        return 0;

    CDBConnectionTemp * pCon = 0;
    QMutexLocker locker(&m_Lock);

    //先在未使用的连接队列里找连接
    //1.找到:将连接移动到已使用队列中 之后返回连接
    //2.没找到:新建一条连接
    PredictOp op = {pDBConnParam};

    while(true)
    {
        ConnList::iterator ite =  std::find_if(m_UnusedConnList.begin(), m_UnusedConnList.end(), op);

        if (ite != m_UnusedConnList.end())
        {
            pCon = (*ite);
            if(pCon->getExtendFlag() == 1) //被标记成删除
            {
                m_UnusedConnList.erase(ite);
                pCon->close();
                delete pCon; //因在未使用队列中 说明未被使用 所以操作安全
                continue ;
            }

            m_UnusedConnList.erase(ite);
            m_UsedConnList.push_back(pCon);
            return pCon;
          }
          else
            break ; //不存在

    }

    //新建一条连接
    //成功:放入已使用连接队列 返回连接
    //失败:返回空
    pCon = new CDBConnectionTemp;
    pCon->setParameters(pDBConnParam);

    //如果连接的IP错误 此函数可能会阻塞几十秒.如果将此操作移到外面.
    //if(!pCon->open())
    //{
        //delete pCon;
        //return 0;
    //}

    m_UsedConnList.push_back(pCon);
    return pCon;
    //TODO:
    //检验未使用连接队列中的连接，未使用的时长是否超限
    //超限:从未使用连接队列中移出连接，并销毁。
}

//回收数据库连接 放到未使用队列中 以备再用
//前提假设:一个连接只能被Push一次
 void MDDBConnCache::PushConnection(CDBConnectionTemp  * conn)
 {
    if(!conn)
        return ;

    QMutexLocker locker(&m_Lock);

    //从已使用队列中移出
    PredictOp2 op2={conn};
    ConnList::iterator ite =  std::find_if(m_UsedConnList.begin(), m_UsedConnList.end(), op2);
    if(ite == m_UsedConnList.end()) //不可能出现此种情况
    {
        //TODO:log
        return ;
    }
    m_UsedConnList.erase(ite);

    //添加到未使用队列 供下次使用
    ite = std::find_if(m_UnusedConnList.begin(), m_UnusedConnList.end(), op2);
    //保证:一个连接只能被Push一次
    if(ite != m_UnusedConnList.end()) //此对象存在于未使用队列中 不可能出现此种情况
    {
        //TODO:log
        return ;
    }

    m_UnusedConnList.push_back(conn);
 }

//保证Cache只能在内部new
MDDBConnCache::MDDBConnCache()
{}

MDDBConnCache::~MDDBConnCache()
{
    //调用者保证没有其它线程使用Cache提供的服务
    m_Lock.lock();
    ConnList::iterator ite = m_UnusedConnList.begin();
    for( ; ite != m_UnusedConnList.end(); ite++)
    {
        (*ite)->close();
        delete (*ite);
    }

    m_UnusedConnList.clear();

    ite = m_UsedConnList.begin();
    for( ; ite != m_UsedConnList.end(); ite++)
    {
        (*ite)->close();
        delete (*ite);
    }

    m_UsedConnList.clear();

    m_Lock.unlock();
}

void MDDBConnCache::DestoryConnection(CDBConnectionTemp * pConn)
{
    if(pConn)
     {
        pConn->setExtendFlag(1);
        //保证已标记的删除的连接在未使用队列中
        //保证实现前提假设:一个连接只能被Push一次
        PushConnection(pConn);
    }
}
