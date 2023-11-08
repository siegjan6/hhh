#ifndef MDNOTIFYPARAMCMD_H
#define MDNOTIFYPARAMCMD_H

/*!

\author bHVvY2hhbw==

\section 功能
1.处理数据库sql语句命令。
2.通过传入的相应参数进行组织成相应的sql语句。插入、更新、删除语句。

*/

#include <QVariant>
#include "MDDataArchivesNotifyParam.h"
#include "../../include/interface/Common/IMDDataType.h"
#include "MDSysLogExpert.h"
#include "MDDatabaseCommand.h"
#define LOG_SOURCE "DataArchives"

class IMDDataSource;
class CMDDBSourceService;

class CMDNotifyParamCMD :public CMDDatabaseCommand
{
public:
    explicit CMDNotifyParamCMD(int dbType, DataArchives::DB_NOTIFY_TYPE type, CMDDataArchivesNotifyParam *pParam, CMDDBSourceService * pService);
    virtual ~CMDNotifyParamCMD();

    //inline void setDBType(int type){m_dbType = type;}
    //inline int DBType()const {return m_dbType;}

    //inline void setType(DataArchives::DB_NOTIFY_TYPE type){m_dbNotifyType = type;}
    //inline DataArchives::DB_NOTIFY_TYPE type()const {return m_dbNotifyType;} //! 设置/获取操作数据库的类型即是插入、更新还是删除。

    //inline void setNotifyParam(CMDDataArchivesNotifyParam *pParam){m_pDBNotifyParam = *pParam;}
    //inline CMDDataArchivesNotifyParam *notifyParam() {return &m_pDBNotifyParam;} //! 设置/获取数据库通过所有修改项

    //inline void setParent(CMDDBSourceService *pService){m_pService = pService;} //! 设置父指针


    /*!
    \section 命令处理，在线程中外部调用的接口
    */
public:
    virtual void Do();

    /*!
    \section 组织成相应的sql语句。插入、更新、删除语句。
    */
private:
    void constructAddSql();
    void constructUpdateSql();
    void constructDeleteSql();

    //void isEnabledMarks(QString &strSql); //! 根据不同的数据库对字段名是否加上双引号

private:
    int m_dbType                                 ;    //! 数据库类型
    DataArchives::DB_NOTIFY_TYPE m_dbNotifyType  ;   //! 操作数据库的类型
    CMDDBSourceService *m_pService               ;    //! 父指针
    CMDDataArchivesNotifyParam m_pDBNotifyParam ;   //! 数据库需要修改的字段值
};

#endif // MDNOTIFYPARAMCMD_H
