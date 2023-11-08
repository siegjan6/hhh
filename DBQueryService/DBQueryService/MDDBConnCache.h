#ifndef MDDB_CONN_CACHE_2016_09_21_H
#define MDDB_CONN_CACHE_2016_09_21_H
#include <list>
#include <QString>
#include <QMutex>
#include <MDDataArchivesInterface/IMDDatabaseParam.h>

class CDBConnectionTemp;//!使用CDBConnectionTemp逐步替换掉CMDDBConnect 待全部替换掉之后 在将CDBConnectionTemp抽出，形成动态库。

class MDDBConnCache{
public:
    typedef std::list<CDBConnectionTemp *> ConnList;
public:
    static void Init(); //使用此Cache前必须被调用 且只能调用一次
    static MDDBConnCache * GetInst();//获取数据库连接池对象
    static void DestroyCache();//只能被调用一次
public:
    CDBConnectionTemp * PullConnection(IMDDataBaseParam * pDBConnParam);//!根据连接参数获得连接 不存在则创建
    void PushConnection(CDBConnectionTemp * conn);//!将数据库连接放回到缓冲容器中
    void DestoryConnection(CDBConnectionTemp *conn); //!标记成删除
private:
    MDDBConnCache();
    ~MDDBConnCache();
private:
    ConnList m_UnusedConnList;//!未使用的连接队列
    ConnList m_UsedConnList;//!已使用的连接队列
    QMutex m_Lock;//线程锁
    static MDDBConnCache * m_pConnCache;
};

#endif  /*_CONNECTION_POOL_H */
