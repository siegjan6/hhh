#ifndef MDDATABASECHECKCMD_H
#define MDDATABASECHECKCMD_H

/*!

\author bHVvY2hhbw==

\section 功能
//TODO:更新注释
1.数据库检查命令--检查工作数据库与临时数据库，并且保证临时数据库与工作数据库的所有表相互对应，
  临时数据库用于保证数据库数据不丢失。
2.检查数据库是否重新创建。
3.检查表否重新创建。

\section 修改历史
2016.7.28 / c2pj / 整理命令体系 修改CMDDatabaseCheckCMD为CMDDatabaseCommand子类
                            修改构造函数 去掉setParent、checkoutDatabase函数
*/

#include "MDSysLogExpert.h"
#include "MDDatabaseCommand.h"
#include <MDDataArchivesInterface/IMDDatabaseParam.h>
#define LOG_SOURCE "DataArchives"

class CMDDBSourceService;

class IMDDataTable;

class CMDDatabaseCheckCMD :public CMDDatabaseCommand
{
public:
    //pDBConnParam 数据库连接参数 由本类释放内存
    CMDDatabaseCheckCMD(CMDDBSourceService *parent, IMDDataBaseParam * pDBConnParam);
    virtual ~CMDDatabaseCheckCMD();

    //! 设置父指针
    //inline void setParent(CMDDBSourceService *parent){m_pService = parent;}
public:
    virtual void Do();
private:
    /*!
    \section 检查数据库结构
    */
    //void checkoutDatabase();
    bool checkDatabase();
    bool createDatabaseAndConnect(); //! 检查数据库结构//?
    QString getFieldSqlCmdPart(void *pF);
    /*!
    \section 检查数据库以及对应的表是否存在，是否在创建，
             检查是否要从临时数据库中导入到工作数据库.
    */
    bool checkTable();
    bool createDbTables(IMDDataTable *pTable);
    bool createDbTables(IMDDataTable *pTable, QString &strError); //! 检查表结构
private:
    CMDDBSourceService *m_pService; //! 父指针
    IMDDataBaseParam * m_pDBConnParam;
};

#endif // MDDATABASECHECKCMD_H
