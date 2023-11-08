#include <QCoreApplication>
#include <QDebug>
#include <MDDBQueryConfigure.h>
#include <assert.h>

void Test();
void Convert();
void TestCase10();
void TestDeleteView();
void TestRenameView();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //Test();
    //Convert();
    //TestCase10();
    //TestDeleteView();
    TestRenameView();
    return a.exec();
}

void TestCase10()
{
    DBQ::View * pView = 0;
    QString errStr;
    QString viewName("查询1");
    bool b = DBQ::LoadViewFromDir("D:/NetSCADA6/code/DBQuery/ConfigureTestCase/case2",  viewName, &pView, errStr);
    assert(b);

    b = DBQ::LoadViewFromDir("D:/NetSCADA6/code/DBQuery/ConfigureTestCase/case2",  "Q1", &pView, errStr);
    assert(b);
}

QString dir("D:/NetSCADA6/code/DBQuery/ConfigureTestCase/case1/");
void Test()
{
    QList<DBQ::DBSource *>  dbSources1;
    QList<DBQ::DBSource *>  dbSources2;
    QList<DBQ::DBSource *>  dbSources3;
    QString errStr;
    DBQ:: LoadDataSources(dir+"dbsources.xml", DBQ::FILE_TYPE_XML, dbSources1, errStr);
    assert(dbSources1.size() == 2);
    DBQ:: LoadDataSources(dir+"dbsources.json", DBQ::FILE_TYPE_JSON, dbSources2, errStr);
    assert(dbSources2.size() == 2);
    DBQ:: WriteDataSourcesToFile(dir+"dbsources_w1.json", DBQ::FILE_TYPE_JSON, dbSources1, errStr);
    DBQ:: LoadDataSources(dir+"dbsources_w1.json", DBQ::FILE_TYPE_JSON, dbSources3, errStr);
    assert(dbSources3.size() == 2);

    DBQ:: View * pView = NULL;
    errStr.clear();
    bool b = DBQ::LoadView(dir+"view.json",  DBQ::FILE_TYPE_JSON, &pView, errStr);
    assert(b);
    b = DBQ::WriteViewToFile(dir+"view_w1.json", DBQ::FILE_TYPE_JSON, pView, errStr);
    assert(b);

    QList<DBQ::ViewVariable *> viewVars;
    b = DBQ::LoadViewVariable(dir+"viewvariables.json",DBQ::FILE_TYPE_JSON, viewVars, errStr);
    assert(b);
    b = DBQ::WriteViewVariableToFile(dir+"viewvariables_w1.json", DBQ::FILE_TYPE_JSON, viewVars, errStr);
    assert(b);

    QList<DBQ::RuntimeViewVariable *> viewRunVars;
    errStr.clear();
    b = DBQ::LoadRuntimeViewVariable(dir+"view_runtime_variables_viewname.json", DBQ::FILE_TYPE_JSON, viewRunVars, errStr);
    assert(b);
    b = DBQ::WriteRuntimeViewVariableToFile(dir+"view_runtime_variables_viewname_w1.json",  DBQ::FILE_TYPE_JSON, viewRunVars, errStr);
    assert(b);

    /*
    QString whereClauseStr;
    DBQ::Filter_CreateWhereClause(pView->pFilter, whereClauseStr,  errStr);
    QStringList tokens;
    whereClauseStr.append("66[%vt7%][%v8%]%][%..]");
    DBQ::Filter_ExtractTokensWithBound2(whereClauseStr, "[%", "%]", tokens);

    QString sqlTemplateOut;
    QString emptyStr;
    DBQ::View_CreateSQLTemplate(pView, emptyStr, sqlTemplateOut, errStr);
    DBQ::View_CreateSQLTemplate(pView, "[#db#]", sqlTemplateOut, errStr);
    */
}


void Convert()
{
    QString dir("D:/NetSCADA6/code/DBQuery/UITestCase/case1/");
    QList<DBQ::DBSource *>  dbSources;
    QString errStr;
    DBQ:: LoadDataSources(dir+"global_dbsources.xml", DBQ::FILE_TYPE_XML, dbSources, errStr);
    DBQ:: WriteDataSourcesToFile(dir+"global_dbsources.json", DBQ::FILE_TYPE_JSON, dbSources, errStr);

    QList<DBQ::ViewVariable *> viewVars;
    DBQ::LoadViewVariable(dir+"global_view_vars.xml", DBQ::FILE_TYPE_XML,viewVars, errStr);
    DBQ::WriteViewVariableToFile(dir+"global_view_vars.json", DBQ::FILE_TYPE_JSON, viewVars, errStr);

    DBQ:: View * pView = NULL;
    errStr.clear();
    bool b = DBQ::LoadView(dir+"view_t_employee.xml", DBQ::FILE_TYPE_XML, &pView, errStr);
    assert(b);
    WriteViewToFile(dir+"view_t_employee.json", DBQ::FILE_TYPE_JSON, pView, errStr);

    //QList<DBQ::RuntimeViewVariable *> rtVars;
    //errStr.clear();
    //LoadRuntimeViewVariable(dir+"view_t_employee_run_view_vars.xml",  DBQ::FILE_TYPE_XML,  rtVars, errStr);
    //WriteRuntimeViewVariableToFile(dir+"view_t_employee_run_view_vars.json", DBQ::FILE_TYPE_JSON, rtVars, errStr);
}

void TestDeleteView()
{
    bool b = DBQ::DeleteViewInDir("D:/NetSCADA6/code/DBQuery/ConfigureTestCase/case3", "t_employee");
    assert(b);
}

void TestRenameView()
{
    bool b = DBQ::RenameViewInDir("D:/NetSCADA6/code/DBQuery/ConfigureTestCase/case3", "t_employee", "t_emp");
    assert(b);
}
