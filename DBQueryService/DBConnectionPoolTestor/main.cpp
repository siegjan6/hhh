#include <QCoreApplication>
#include <QDebug>
#include <MDDataSource/MDDataSourceFactory.h>
#include <thread>
#include <assert.h>
#include "MDDBConnCache.h"
#include "DBConnectionTemp.h"

void TestSmoke();
void TestPullAndPush();
void TestDestoryConnection();
void TestNotExistConnection();
void TestMultiThread();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MDDBConnCache::Init();

    //TestSmoke();
    //TestPullAndPush();
    //TestDestoryConnection();
    //TestNotExistConnection();
    TestMultiThread();
    return a.exec();
}

static IMDDataBaseParam * CreateConnParam1()
{
    IMDDataBaseParam * pParam1 = CMDDataSourceFactory::createDatabasePaream();
    pParam1->setdbServeType(DATABASEPARAM::DB_TYPE_MYSQL);
    pParam1->setdbServerIP("127.0.0.1");
    pParam1->setdbServerPort(3306);
    pParam1->setdbName("netscadatestdb");
    pParam1->setdbUser("root");
    pParam1->setdbPassword("123456");

    return pParam1;
}

static IMDDataBaseParam * CreateConnParam2()
{
    IMDDataBaseParam * pParam1 = CMDDataSourceFactory::createDatabasePaream();
    pParam1->setdbServeType(DATABASEPARAM::DB_TYPE_MYSQL);
    pParam1->setdbServerIP("192.168.1.5");
    pParam1->setdbServerPort(3306);
    pParam1->setdbName("netscadatestdb");
    pParam1->setdbUser("root");
    pParam1->setdbPassword("123456");

    return pParam1;
}
void TestSmoke()
{
    MDDBConnCache * pConnCache = MDDBConnCache::GetInst()->GetInst();
    CDBConnectionTemp * pConn = pConnCache->PullConnection(CreateConnParam1());

    assert(pConn);
    pConn->open();

    QSqlQuery * query = pConn->getQuery();
    query->exec("select * from deptno");

    while(query->next())
    {
        qDebug()<<query->value(0).toInt()<<","
                        <<query->value(1).toString();
    }
}

void TestPullAndPush()
{
    IMDDataBaseParam * pDBParam = CreateConnParam1();
    MDDBConnCache * pConnCache = MDDBConnCache::GetInst()->GetInst();
    CDBConnectionTemp * pConn = pConnCache->PullConnection(pDBParam);
   assert(pConn);
    pConn->open();

    pConnCache->PushConnection(pConn);

    pConn = pConnCache->PullConnection(pDBParam);
    assert(pConn);

    QSqlQuery * query = pConn->getQuery();
    query->exec("select * from deptno");

    while(query->next())
    {
        qDebug()<<query->value(0).toInt()<<","
                        <<query->value(1).toString();
    }
}

//创建两条连接 一条标识成删除
void TestDestoryConnection()
{
    IMDDataBaseParam * pDBParam = CreateConnParam1();
    MDDBConnCache * pConnCache = MDDBConnCache::GetInst()->GetInst();

    CDBConnectionTemp * pConn1 = pConnCache->PullConnection(pDBParam);
    assert(pConn1);

    CDBConnectionTemp * pConn2 = pConnCache->PullConnection(pDBParam);
    assert(pConn2);

    assert(pConn1 != pConn2);

    pConnCache->DestoryConnection(pConn1); //用户保证此pConn1不再被使用
    pConnCache->PushConnection(pConn1); //Detory操作无须此步 仅为测试用户的非法操作

    pConnCache->PushConnection(pConn2);
    CDBConnectionTemp * pConn = pConnCache->PullConnection(pDBParam);
    assert(pConn == pConn2);
}

void TestNotExistConnection()
{
    IMDDataBaseParam * pDBParam1 = CreateConnParam1();
    MDDBConnCache * pConnCache = MDDBConnCache::GetInst()->GetInst();
    CDBConnectionTemp * pConn1 = pConnCache->PullConnection(pDBParam1);
    pConnCache->PushConnection(pConn1);

    IMDDataBaseParam * pDBParam2 = CreateConnParam2();
    CDBConnectionTemp * pConn2 = pConnCache->PullConnection(pDBParam2);
    pConn2->open();
    assert(pConn2->isOpen() == false);
}

void TestMultiThread()
{
    std::thread t1(TestSmoke);
    std::thread t2(TestSmoke);
    t1.detach();
    t2.detach();
}

