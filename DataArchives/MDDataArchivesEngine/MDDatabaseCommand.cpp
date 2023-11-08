//#include "NDSDBCheckConnectionCmd.h"
#include "MDDatabaseService.h"

CNDSDBCheckConnectionCmd::CNDSDBCheckConnectionCmd(CMDDBSourceService * pDBSourceService)
    :m_pDBSourceService(pDBSourceService)
{}

void CNDSDBCheckConnectionCmd::Do()
{
    if (NULL == m_pDBSourceService->dataSource())
    {
        //TODO:log
        return;
    }

    CMDDBConnect *pDBConnect = m_pDBSourceService->currdb();//?有可能是临时数据库
    if (NULL == pDBConnect)
    {
        //TODO:log
        return ;
    }

    // 获取数据库参数
    IMDDataBaseParam *pDBParam = m_pDBSourceService->dataSource()->dbParam();
    if(NULL == pDBParam)
    {
        //TODO:log
        return ;
    }

    QString strError    = "";
    QString strPassword = pDBParam->dbPassword();
    QString strDbName	= pDBParam->dbName();
    QString strUser		= pDBParam->dbUser();
    int     port           = pDBParam->dbServerPort();
    QString strDBIP		= pDBParam->dbServerIP();

    DATABASEPARAM::DATABASE_TYPE type = pDBParam->dbServeType();
    if (DATABASEPARAM::DB_TYPE_SQL == type)
    {
        ConnectSQLServer();

    }
    else
    {
        //代码意图:当程序第一次运行时,数据库中可能没有对应的数据库实例。
        //例如，数据库实例名为mtDB。当连接mtDB时就会出错。
        //出错有两种可能:1、连接不通 2、数据库中不存在mtDB实例
        //所以,以下代码先以空数据库实例(金仓数据库为SAMPLES)测试连接是否通畅
        //如果通暢，则创建mtDB数据库实例

        //连接成功 说明strDbName数据库存在
        bSuccess = pDBConnect->connectDatabase(type, strDbName, strDBIP, strUser, strPassword, port, strError);
        if (!bSuccess)
        {
            pDBConnect->close();
            //连接名为"netscada_query_anme_%1" 因为连接名可能会重复 所以使用另一类连接名
            //但仍然有可能重复
            //测试连接数据库是否正常.使用金仓数据库时,strDBName为"SAMPLES",非金仓数据库为空.
            bSuccess = pDBConnect->queryConnectDatabase(type, strDBName, strDBIP, strUser, strPassword, port, strError);
            if (bSuccess)
            {
                QString strCreate = "";
                strCreate += "create database ";
                strCreate += strDbName;
                strCreate += ";";
                QSqlQuery query(pDBConnect->currentDatabase());
                bSuccess = query.exec(strCreate);
                strError = query.lastError().text();
                pDBConnect->close();
                if (bSuccess)
                {
                    //上面的语句创建了数据库.再次打开数据库.但可能还会失败.
                    pDBConnect->connectDatabase(type, strDbName, strDBIP, strUser, strPassword, port, strError);
                }
            }
        }
    }

    MDLOG_INFO_TO_LOGGER(QString("engine---database name: %1, ip: %2, user: %4, port: %5, error: %6, number: %7").arg(strDbName).arg(strDBIP).arg(strUser)\
                         .arg(port).arg(strError).arg(pDBConnect->currentDatabase().lastError().number()));

    return bSuccess;
}

void CNDSDBCheckConnectionCmd::ConnectSQLServer()
{
    //当前模式为工作数据库且插入的数据库为sqlserver数据库的处理方式，采用数据源的方式连接数据库，这样即不用再配置数据源
    //内部使用ODBC连接SQL Server数据库

    IMDDataBaseParam *pDBParam = m_pDBSourceService->dataSource()->dbParam();

    QString strError    = "";
    QString strPassword = pDBParam->dbPassword();
    QString strDbName	= pDBParam->dbName();
    QString strUser		= pDBParam->dbUser();
    int     port           = pDBParam->dbServerPort();
    QString strDBIP		= pDBParam->dbServerIP();

    //连接strDbName指定的数据库
    QString strDsn = QString("DRIVER={SQL SERVER};SERVER=%1,%2;DATABASE=%3;uid=%4;pwd=%5;")\
            .arg(strDBIP).arg(port).arg(strDbName).arg(strUser).arg(strPassword);
    bool bSuccess = pDBConnect->connectDatabase(type, strDsn, strError);//此次连接的数据库名字为strDbName b为小写
    if(bSuccess)
        return ;

    //Log:记录失败信息

    //连接strDbName指定的数据库实例,失败
    //失败原因:
    //1.数据库中不存在strDbName指定的数据库实例
    //2.由于网络、配置参数出错

    if (!bSuccess) //连接不成功 尝试用strDBName连接 B为大写
    {
        pDBConnect->close();
        strDsn = QString("DRIVER={SQL SERVER};SERVER=%1,%2;DATABASE=%3;uid=%4;pwd=%5;")\
                .arg(strDBIP).arg(port).arg(strDBName).arg(strUser).arg(strPassword);
        bSuccess = pDBConnect->connectDatabase(type, strDsn, strError);
        if (bSuccess)
        {
            QString strCreate = "";
            strCreate += "create database ";
            strCreate += strDbName;
            strCreate += ";";
            QSqlQuery query(pDBConnect->currentDatabase());
            bSuccess = query.exec(strCreate);
            strError = query.lastError().text();
            pDBConnect->close();
            if (bSuccess)
            {
                strDsn = QString("DRIVER={SQL SERVER};SERVER=%1,%2;DATABASE=%3;uid=%4;pwd=%5;")\
                        .arg(strDBIP).arg(port).arg(strDbName).arg(strUser).arg(strPassword);
                pDBConnect->connectDatabase(type, strDsn, strError);
            }
        }
    }
}
