#include <MDDBQueryEngineFactory.h>
#include <MDSQLDataBaseProxy/MDDatabaseRecordset.h>
#include <QDebug>
#include <assert.h>

static IMDDBQueryEngine * pEngine = 0;

class Case1Notify : public IMDDBRecordSetNotify
{
public:
    virtual void NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *);
};

void Case1Notify::NotityDBQueryResult(const QString & strQueryID, CMDDatabaseRecordset * pRecordset)
{
    //能查出值就OK
    int row = pRecordset->rowCount();
    int col = pRecordset->columnCount();

    for(int i=0;i<row; ++i)
    {
        qDebug()<<i<<" row";

        for(int j=0;j<col; ++j)
        {
            qDebug()<<pRecordset->value(i, j);
        }
    }
}

class Case2Notify : public IMDDBRecordSetNotify
{
public:
    virtual void NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *);
};

void Case2Notify::NotityDBQueryResult(const QString & strQueryID, CMDDatabaseRecordset * pRecordset)
{
    int row = pRecordset->rowCount();
    int col = pRecordset->columnCount();

    assert(1 == row); //只有一条数据

    for(int i=0;i<row; ++i)
    {
        qDebug()<<i<<" row";

        for(int j=0;j<col; ++j)
        {
            if(1==j)
            {
                QString name = pRecordset->value(i, j).toString() ;
                assert(name == "KING");
            }
        }//end for 2
    }//end for1
}

class Case3Notify : public IMDDBRecordSetNotify
{
public:
    virtual void NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *);
};

void Case3Notify::NotityDBQueryResult(const QString & strQueryID, CMDDatabaseRecordset * pRecordset)
{
    int row = pRecordset->rowCount();
    int col = pRecordset->columnCount();

    assert(2 == row && 2 == col); //只有两条数据

    for(int i=0;i<row; ++i)
    {
        QString name = pRecordset->value(i, 0).toString() ;
        assert(name == "ALLEN" || name == "TURNER");
    }
}

class Case4Notify : public IMDDBRecordSetNotify
{
public:
    virtual void NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *);
};

void Case4Notify::NotityDBQueryResult(const QString & strQueryID, CMDDatabaseRecordset * pRecordset)
{
    int row = pRecordset->rowCount();
    int col = pRecordset->columnCount();

    assert(9 == row && 3 == col); //只有两条数据
}

class Case5Notify : public IMDDBRecordSetNotify
{
public:
    virtual void NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *);
};

void Case5Notify::NotityDBQueryResult(const QString & strQueryID, CMDDatabaseRecordset * pRecordset)
{
    int row = pRecordset->rowCount();
    int col = pRecordset->columnCount();

    assert(8 == row && 2 == col); //只有两条数据

    for(int i=0;i<row; ++i)
    {
        qDebug()<<i<<" row";

        for(int j=0;j<col; ++j)
        {
            qDebug()<<pRecordset->value(i, j);
        }
    }
}

class Case6Notify : public IMDDBRecordSetNotify
{
public:
    virtual void NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *);
};

void Case6Notify::NotityDBQueryResult(const QString & strQueryID, CMDDatabaseRecordset * pRecordset)
{
    int row = pRecordset->rowCount();
    int col = pRecordset->columnCount();

    assert(15230 == row);

    for(int i=0;i<row&&i<20; ++i)
    {
        qDebug()<<i<<" row";

        for(int j=0;j<col; ++j)
        {
            qDebug()<<pRecordset->value(i, j);
        }
    }
}

class Case9Notify : public IMDDBRecordSetNotify
{
public:
    virtual void NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *);
};

void Case9Notify::NotityDBQueryResult(const QString & strQueryID, CMDDatabaseRecordset * pRecordset)
{
    //能查出值就OK
    int row = pRecordset->rowCount();
    int col = pRecordset->columnCount();

    for(int i=0;i<row; ++i)
    {
        qDebug()<<i<<" row";

        for(int j=0;j<col; ++j)
        {
            qDebug()<<pRecordset->value(i, j);
        }
    }
}

void TestCase1()
{
    Case1Notify * pNotify = new Case1Notify;
    pEngine = CMDDBQueryEngineFactory::createDBQueryEngine();
    bool b = pEngine->load("D:/NetSCADA6/code/DBQuery/TestCase/case1");
    assert(b);
    QString queryID;
    b = pEngine->queryView("t_employee", pNotify, queryID);
    assert(b);
}

void TestCase2()
{
    Case2Notify * pNotify = new Case2Notify;
    pEngine = CMDDBQueryEngineFactory::createDBQueryEngine();
    bool b = pEngine->load("D:/NetSCADA6/code/DBQuery/TestCase/case2");
    assert(b);
    QString queryID;
    b = pEngine->queryView("t_employee", pNotify, queryID);
    assert(b);
}

void TestCase3()
{
    //SELECT * FROM mytestdb.t_employee where job='SALESMAN' and sal>1300;
    Case3Notify * pNotify = new Case3Notify;
    pEngine = CMDDBQueryEngineFactory::createDBQueryEngine();
    bool b = pEngine->load("D:/NetSCADA6/code/DBQuery/TestCase/case3");
    assert(b);
    QString queryID;
    b = pEngine->queryView("t_employee", pNotify, queryID);
    assert(b);
}

void TestCase4()
{
    //SELECT t_employee.empno as id, t_employee.ename as name, deptno.dname as department
    //from mytestdb.t_employee, mytestdb.deptno where t_employee.deptno = deptno.deptno;

    Case4Notify * pNotify = new Case4Notify;
    pEngine = CMDDBQueryEngineFactory::createDBQueryEngine();
    bool b = pEngine->load("D:/NetSCADA6/code/DBQuery/TestCase/case4");
    assert(b);
    QString queryID;
    b = pEngine->queryView("t_employee", pNotify, queryID);
    assert(b);
}

void TestCase5()
{
    //SELECT * FROM mytestdb.t_employee where job='SALESMAN' and sal>1300;
    Case5Notify * pNotify = new Case5Notify;
    pEngine = CMDDBQueryEngineFactory::createDBQueryEngine();
    bool b = pEngine->load("D:/NetSCADA6/code/DBQuery/TestCase/case5");
    assert(b);
    QString queryID;
    b = pEngine->queryView("t_employee", pNotify, queryID);
    assert(b);
}

void TestCase6()
{
    //SELECT RecordTime, var_v1 FROM netscadatestdb.hisgroup where RecordTime>'2016-11-02 02:30:30'
    Case6Notify * pNotify = new Case6Notify;
    pEngine = CMDDBQueryEngineFactory::createDBQueryEngine();
    bool b = pEngine->load("D:/NetSCADA6/code/DBQuery/TestCase/case6");
    assert(b);
    QString queryID;
    b = pEngine->queryView("hisgroup", pNotify, queryID);
    assert(b);
}

void TestCase9()
{
    Case9Notify * pNotify = new Case9Notify;
    pEngine = CMDDBQueryEngineFactory::createDBQueryEngine();
    bool b = pEngine->load("D:/NetSCADA6/code/DBQuery/TestCase/case9");
    assert(b);
    QString queryID;
    b = pEngine->queryView("t_employee", pNotify, queryID);
    assert(b);
}

void Test()
{
    //TestCase1();
    //TestCase2();
    //TestCase3();
    //TestCase4();
    //TestCase5();
    //TestCase6();
    TestCase9();
}
