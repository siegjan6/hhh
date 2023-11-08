#ifndef MDDB_CONN_CACHE_2016_09_21_H
#define MDDB_CONN_CACHE_2016_09_21_H
#include <list>
#include <QString>
#include <QMutex>
#include "DatabaseParam.h"
#include <MDDBConnect/MDDBConnect.h>

namespace DBQE
{
//TODO:必须抽出.放在MDDBConnect模块中
class MDDBConnCache{
public:
    typedef std::list<MDDBConnection *> ConnList;
public:
    static void Init(); //使用此Cache前必须被调用 且只能调用一次
    static MDDBConnCache * GetInst();//获取数据库连接池对象
    static void DestroyCache();//只能被调用一次
public:
    MDDBConnection * PullConnection(CDataBaseParam * pDBConnParam);//!根据连接参数获得连接 不存在则创建
    void PushConnection(MDDBConnection * conn);//!将数据库连接放回到缓冲容器中
    void DestoryConnection(MDDBConnection *conn); //!标记成删除
private:
    MDDBConnCache();
    ~MDDBConnCache();
private:
    ConnList m_UnusedConnList;//!未使用的连接队列
    ConnList m_UsedConnList;//!已使用的连接队列
    QMutex m_Lock;//线程锁
    static MDDBConnCache * m_pConnCache;
};
}

#endif  /*_CONNECTION_POOL_H */
