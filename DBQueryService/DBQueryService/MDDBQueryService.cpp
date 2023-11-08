#include "MDDBQueryService.h"

#include "MDDBConnect.h"
#include "IMDDatabaseParam.h"
#include "DBQueryCommand.h"
#include "IMDDataSourceManager.h"
#include "IMDDataSource.h"
#include "IMDDatabaseParam.h"
#include "MDQueryDatabaseThread.h"
#include "MDDBConnCache.h"

#include <QSqlDatabase>

//======================== CMDDBQueryService begin ===============================
CMDDBQueryService::CMDDBQueryService()
{
    m_iPageRows = 0;     //! 每页行数
    m_pQueryThread = new CMDQueryDatabaseThread;
}

CMDDBQueryService::~CMDDBQueryService()
{
    stop();
}

//启动 停止服务
bool CMDDBQueryService::start()
{
    if(m_pQueryThread)
    {
        MDDBConnCache::Init();
        m_pQueryThread->start();
    }

    return true;
}

bool CMDDBQueryService::stop()
{
    removeAllQueryCmd();

    if(m_pQueryThread)
    {
        MDDBConnCache::DestroyCache();
        m_pQueryThread->stop();
        delete m_pQueryThread;
        m_pQueryThread = NULL;
    }

    return true;
}

//增加查询命令
bool CMDDBQueryService::query(IMDDBQueryParam *pQueryParam, IMDDataArchives *pDataArchives)
{
    CMDDBQueryParam *pParam = dynamic_cast<CMDDBQueryParam*>(pQueryParam);
    if (!pParam)
    {
        return false;
    }

    if (!pParam->queryExternal())
    {        
        IMDDataSource *pDataSrc = pDataArchives->dataSourceManager()->getDataSource(pParam->dataSourceName());
        if (NULL == pDataSrc)
        {
            return false;
        }

        IMDDataBaseParam *pDBParam = pDataSrc->dbParam();
        pQueryParam->setDBParam(pDBParam);
        if (pDataSrc->isEnableDataTablePrefix()
                && !pDataSrc->dataTablePrefix().isEmpty())
        {
            pQueryParam->setQuerytable(pDataSrc->dataTablePrefix() + "_" + pParam->queryTable());
        }
    } 

    return query(pQueryParam);
}

bool CMDDBQueryService::query(IMDDBQueryParam *pQueryParam)
{
    bool bSuccess = false;
    CMDDBQueryParam *pParam = dynamic_cast<CMDDBQueryParam*>(pQueryParam);
    if (!pParam)
    {
        return bSuccess;
    }

    CDBQueryCommand *pQueryCmd = NULL;
    QString strSql = calculateSQLString(pParam);
    pQueryCmd = new CDBQueryCommand(pParam->queryID(), strSql);
    pQueryCmd->setDBParam(pParam->DBParam()->clone());
    pQueryCmd->setQueryRslNotify(pParam->queryResultNotify());
    pQueryCmd->start();

    if (NULL == m_pQueryThread)
    {
        return bSuccess;
    }

    CMDCommand *pCmd = new CMDCommand();
    pCmd->setCmdPara(reinterpret_cast<void*>(pQueryCmd));
    m_pQueryThread->submitCommand(pCmd);

//    MDLOG_INFO_TO_LOGGER(QString("query---add cmd to queue, sql: %1...").arg(strSql));

//    delete pParam;
//    pParam = NULL;

    return bSuccess;
}

bool CMDDBQueryService::queryUnion(const QList<IMDDBQueryParam *> & pQueryParams, IMDDataArchives *pDataArchives)
{
    if(pQueryParams.isEmpty())
        return false;

    IMDDBQueryParam * pQueryParam = pQueryParams[0];
    CMDDBQueryParam *pParam = dynamic_cast<CMDDBQueryParam*>(pQueryParam);
    if (!pParam)
    {
        return false;
    }

    //目前只支持单数据源
    //TODO:日后支持多机多数据源跨库查询

    if (!pParam->queryExternal())
    {
        IMDDataSource *pDataSrc = pDataArchives->dataSourceManager()->getDataSource(pParam->dataSourceName());
        if (NULL == pDataSrc)
        {
            return false;
        }

        IMDDataBaseParam *pDBParam = pDataSrc->dbParam();
        pQueryParam->setDBParam(pDBParam);
        if (pDataSrc->isEnableDataTablePrefix()
                && !pDataSrc->dataTablePrefix().isEmpty())
        {
            pQueryParam->setQuerytable(pDataSrc->dataTablePrefix() + "_" + pParam->queryTable());
        }
    }

    return queryUnion(pQueryParams);
}

bool CMDDBQueryService::queryUnion(const QList<IMDDBQueryParam *> & pQueryParams)
{
    QString strSQL;
    CDBQueryCommand *pQueryCmd = NULL;

    if(!calculateSQLStringWithUnion(pQueryParams, strSQL))
        return false;

    if(pQueryParams.isEmpty())
        return false;

    IMDDBQueryParam * pQueryParam = pQueryParams[0];
    CMDDBQueryParam *pParam = dynamic_cast<CMDDBQueryParam*>(pQueryParam);
    if (!pParam)
    {
        return false;
    }

    pQueryCmd = new CDBQueryCommand(pParam->queryID(), strSQL);
    pQueryCmd->setDBParam(pParam->DBParam()->clone());
    pQueryCmd->setQueryRslNotify(pParam->queryResultNotify());
    pQueryCmd->start();

    if (NULL == m_pQueryThread)
    {
        return false;
    }

    CMDCommand *pCmd = new CMDCommand();
    pCmd->setCmdPara(reinterpret_cast<void*>(pQueryCmd));
    m_pQueryThread->submitCommand(pCmd);

    return true;
}

//中断查询
bool CMDDBQueryService::breakoffQuery()
{
    return  breakoffAndRecoverQuery(true);
}

//恢复中断
bool CMDDBQueryService::recoverQuery()
{
    return  breakoffAndRecoverQuery(false);
}

bool CMDDBQueryService::addQueryCmd(CDBQueryCommand *pCmd)
{
    bool bSuccess = false;
    if(pCmd)
    {
        m_Mutex_cmd.lock();
        m_QueryCommandList.push_back(pCmd);
        m_Mutex_cmd.unlock();

        bSuccess = true;
    }
    return bSuccess;
}

void CMDDBQueryService::removeAllQueryCmd()
{
    m_Mutex_cmd.lock();

    qDeleteAll(m_QueryCommandList.begin(), m_QueryCommandList.end());
    m_QueryCommandList.clear();

    m_Mutex_cmd.unlock();
}

CDBQueryCommand *CMDDBQueryService::removeQueryCmdHead()
{
    CDBQueryCommand *pReturn = NULL;

    m_Mutex_cmd.lock();
    if(m_QueryCommandList.size() > 0)
    {
        pReturn = m_QueryCommandList.at(0);
        m_QueryCommandList.removeAt(0);
    }
    m_Mutex_cmd.unlock();

    return pReturn;
}

//将传入的数据表、字段、条件组织成SQL语句
QString CMDDBQueryService::calculateSQLString(CMDDBQueryParam *pQueryParam)
{
    QString strSQL = QString();
    if (!pQueryParam)
    {
        return strSQL;
    }

    QString strWhere = "";
    CMDCondition *pCondition = NULL;
    CMDConditionItem *pCondItem = NULL;
    QList<IMDConditionItem*> pListCondItem;
    pListCondItem.clear();

    switch (pQueryParam->queryMode())
    {
    case DB_QUERY::SQL_Mode:
        strSQL = pQueryParam->querySQL();
        return strSQL;
        break;
    case DB_QUERY::Where_Condi_Mode:
        strSQL = calculateBeforeWhereSQL(pQueryParam, false);
        strWhere = pQueryParam->queryWhereConditionstr();
        if (!strWhere.isEmpty())
        {
            strSQL += "where ";
            strSQL += strWhere;
        }
        break;
    case DB_QUERY::Combination_Mode:
        strSQL = calculateBeforeWhereSQL(pQueryParam, false);
        pCondition = dynamic_cast<CMDCondition*>(pQueryParam->queryCondition());
        if (!pCondition)
        {
            return (strSQL += ";");
        }

        if (pCondition->isAdvanced())
        {
            pListCondItem = pCondition->getItemList();
            if (pListCondItem.count() > 0)
            {
                strSQL += "where";
            }

            for (int i = 0; i < pListCondItem.count(); ++i)
            {
                pCondItem =  dynamic_cast<CMDConditionItem*>(pListCondItem.at(i));
                if (pCondItem)
                {
                    strSQL += getRelation(pCondItem->getOrAnd());
                    strSQL += " ";
                    strSQL += pCondItem->objName();
                    strSQL += " ";
                    strSQL += getOperator(pCondItem->getOperater());
                    strSQL += " ";
                    if (calculateVarianType(pCondItem->getOperData()))
                    {
                        strSQL += "'" + pCondItem->getOperData().toString() + "'";
                    }
                    else
                    {
                        strSQL += pCondItem->getOperData().toString();
                    }

                    strSQL += " ";
                }
            }
        }
        else
        {
            strSQL += pCondition->objName();
            strSQL += " ";
            strSQL += getOperator(pCondition->getOperater());
            strSQL += " ";
            if (calculateVarianType(pCondition->operData()))
            {
                strSQL += "'" + pCondition->operData().toString() + "'";
            }
            else
            {
                strSQL += pCondition->operData().toString();
            }
        }

        break;
    default:
        break;
    }

    strSQL += " ;";
    return strSQL;
}

bool CMDDBQueryService::calculateSQLStringWithUnion(const QList<IMDDBQueryParam *> & pQueryParams, QString & sqlOut)
{
    QString strSQL;
    QString strWhere;
    CMDDBQueryParam * pQueryParam = 0;
    int count = pQueryParams.size();

    for(int i=0; i<count; ++i)
    {
        pQueryParam = dynamic_cast<CMDDBQueryParam *>(pQueryParams[i]);
        if(!pQueryParam)
            return false; //TODO:log

        //对于union语句,目前只支持where条件语句
        if(pQueryParam->queryMode() != DB_QUERY::Where_Condi_Mode)
            return false; //TODO:log

        strSQL += calculateBeforeWhereSQL(pQueryParam, true);
        strWhere = pQueryParam->queryWhereConditionstr();
        if (!strWhere.isEmpty())
        {
            strSQL += "where ";
            strSQL += strWhere;
        }

        if(i==(count-1)) //最后句SQL不加union
        {
            sqlOut = strSQL;
            sqlOut += ";";
            return true; //唯一的成功条件
        }

        strSQL += " union ";
    }

    return false;
}

//拼接where条件之前的字串
//修改:添加bool bDBNamePrefix变量.决定在表名前是否添加数据库前缀
QString CMDDBQueryService::calculateBeforeWhereSQL(CMDDBQueryParam *pQueryParam, bool bDBNamePrefix)
{
    QString strSQL = QString("select ");
    QString strMarks =  "";
    QList<QString> pListField = pQueryParam->fieldList();
    int count = pListField.count();
    for (int i = 0; i < count; ++i)
    {
        strMarks = pListField.at(i);
        isEnabledMarks(pQueryParam->DBParam(), strMarks);
        strSQL += strMarks;
        if (i != (count - 1))
        {
            strSQL += ", ";
        }
    }

    if (0 >= count)
    {
        strSQL += "*";
    }

    strMarks = pQueryParam->queryTable();

    if(bDBNamePrefix)
    {
        QString tempStr = pQueryParam->DBParam()->dbName();
        tempStr += "."; //MySQL下没问题 TODO:测试MS SQL Server、ORACLE是否支持。
        strMarks = tempStr + strMarks;
    }

    isEnabledMarks(pQueryParam->DBParam(), strMarks);    
    strSQL += " from ";
    strSQL += strMarks;
    strSQL += + " ";

    return strSQL;
}

//判断字段类型用以是否加单引号
bool CMDDBQueryService::calculateVarianType(const QVariant &vData)
{
    bool success = true;
    switch (vData.type())
    {
    case QVariant::Int:
    case QVariant::Bool:
    case QVariant::LongLong:
    case QVariant::UInt:
    case QVariant::ULongLong:
    case QVariant::Double:
        success = false;
        break;
    default:
        break;
    }

    return success;
}

//获取条件操作符
QString CMDDBQueryService::getOperator(DB_QUERY::DB_OPERATER oper)
{
    QString strOper = QString();
    switch (oper)
    {
    case DB_QUERY::Oper_Big:
        strOper = ">";
        break;
    case DB_QUERY::Oper_Small:
        strOper = "<";
        break;
    case DB_QUERY::Oper_Equal:
        strOper = "=";
        break;
    case DB_QUERY::Oper_Big_Equal:
        strOper = ">=";
        break;
    case DB_QUERY::Oper_Small_Equal:
        strOper = "<=";
        break;
    case DB_QUERY::Oper_Not_Equal:
        strOper = "!=";
        break;
    default:
        break;
    }

    return strOper;
}

//获取条件之间的关系类型
QString CMDDBQueryService::getRelation(int orAnd)
{
    QString strRelation = QString(" ");
    switch (orAnd)
    {
    case RELATION_AND:
        strRelation = "and";
        break;
    case RELATION_OR:
        strRelation = "or";
        break;
    default:
        break;
    }

    return strRelation;
}

//中断 恢复中断接口
bool CMDDBQueryService::breakoffAndRecoverQuery(bool iFlag)
{
    CDBQueryCommand *pReturn = NULL;

    m_Mutex_cmd.lock();

    for(int i = 0; i < m_QueryCommandList.size(); i++)
    {
        pReturn = m_QueryCommandList.at(i);
        if(pReturn)
        {
            //LD 中断处理待定接口
            pReturn->setStarted(!iFlag);
        }
    }
    m_Mutex_cmd.unlock();

    return true;
}

CDBQueryCommand *CMDDBQueryService::findQueryCmd(const QString &strQueryID)
{
    CDBQueryCommand *pReturn = NULL;

    m_Mutex_cmd.lock();
    for(int i = 0; i < m_QueryCommandList.size(); i++)
    {
        CDBQueryCommand *pCmd = m_QueryCommandList.at(i);
        if(NULL == pCmd)
        {
            continue;
        }

        if(0 == (pCmd->getQueryID().compare(strQueryID)))
        {
            pReturn = pCmd;
            m_QueryCommandList.removeAt(i);
            return pReturn;
        }

    }
    m_Mutex_cmd.unlock();

    return NULL;
}

void CMDDBQueryService::isEnabledMarks(IMDDataBaseParam *pDBParam, QString &strSql)
{
    if (NULL == pDBParam)
    {
        return;
    }

    switch (pDBParam->dbServeType())
    {
    case DATABASEPARAM::DB_TYPE_ACCESS:
    case DATABASEPARAM::DB_TYPE_SQL:
    case DATABASEPARAM::DB_TYPE_DB2:
    case DATABASEPARAM::DB_TYPE_SQLLITE:
    case DATABASEPARAM::DB_TYPE_ORACLE:
    case DATABASEPARAM::DB_TYPE_KINGBASE:
        strSql = "\"" + strSql + "\"";
        break;
    case DATABASEPARAM::DB_TYPE_MYSQL:
        break;
    default:
        break;
    }
}

//======================== CMDCondition begin ===============================
CMDCondition::CMDCondition()
    : m_bAdvanced(false),
      m_strName(QString()),
      m_iOperater(DB_QUERY::Oper_Undefine),
      m_varData(QVariant())
{
    m_pListCondItem.clear();
}

CMDCondition::~CMDCondition()
{
    removeAllConditionItem();
}

void CMDCondition::operator=(const CMDCondition &otherObj)
{
    m_bAdvanced     = otherObj.m_bAdvanced;
    m_strName       = otherObj.m_strName;
    m_iOperater     = otherObj.m_iOperater;
    m_varData       = otherObj.m_varData;

    while(!m_pListCondItem.isEmpty())
    {
        IMDConditionItem *pItem = m_pListCondItem.takeFirst();
        delete pItem;
        pItem = NULL;
    }

    int iCount = otherObj.m_pListCondItem.count();
    for(int i = 0; i < iCount; ++i)
    {
        IMDConditionItem *pItem = otherObj.m_pListCondItem.at(i);
        if(pItem)
        {
            addConditionItem(pItem->clone());
        }
    }
}

IMDCondition *CMDCondition::clone()
{
    CMDCondition *pCondition = new CMDCondition;
    *pCondition = *this;
    return pCondition;
}

bool CMDCondition::removeConditionItem(IMDConditionItem *pItem)
{
    bool success = false;
    if (pItem)
    {
        int index = m_pListCondItem.indexOf(pItem);
        if (-1 != index)
        {
            m_pListCondItem.removeAt(index);
            delete pItem;
            pItem = NULL;
            success = true;
        }
    }

    return success;
}

void CMDCondition::removeAllConditionItem()
{
    if (!m_pListCondItem.isEmpty())
    {
        qDeleteAll(m_pListCondItem.begin(), m_pListCondItem.end());
        m_pListCondItem.clear();
    }
}

//======================== CMDDBQueryParam begin ===============================
CMDDBQueryParam::CMDDBQueryParam()
{
    m_pDBParam           = NULL;                      //! 数据库参数
    m_pCond              = NULL;                      //! 条件
    m_iRows              = 0;                         //! 行数
    m_strTabName         = "";                        //! 表名
    m_strQueryID         = "";                        //! 查询ID
    m_strWhereCond       = "";                        //! where条件
    m_strSQLStr          = "";                        //! sql语句
    m_strDataSrcNmae     = "";                        //! 数据源名
    m_iMode              = DB_QUERY::Invalid_Mode;    //! 查询模式
    m_pRestNotify        = NULL;                      //! 回调指针
    m_iExternalFlag = false;
    m_pListField.clear();
}

CMDDBQueryParam::~CMDDBQueryParam()
{
    if (m_pCond)
    {
        delete m_pCond;
        m_pCond = NULL;
    }

    if (m_pDBParam)
    {
        delete m_pDBParam;
        m_pDBParam = NULL;
    }
}

IMDDBQueryParam *CMDDBQueryParam::clone()
{
    CMDDBQueryParam *pObj = new CMDDBQueryParam;
    *pObj = *this;
    return pObj;
}

void CMDDBQueryParam::setDBParam(IMDDataBaseParam *pDBParam)
{
    if(!pDBParam)
    {
        m_pDBParam = 0;
        return ;
    }

    delete m_pDBParam;
    m_pDBParam = pDBParam->clone();
}

void CMDDBQueryParam::operator=(const CMDDBQueryParam &otherObj)
{
    m_pDBParam      = otherObj.m_pDBParam;
    m_iRows         = otherObj.m_iRows;
    m_strTabName    = otherObj.m_strTabName;
    m_strQueryID    = otherObj.m_strQueryID;
    m_strWhereCond  = otherObj.m_strWhereCond;
    m_strSQLStr     = otherObj.m_strSQLStr;
    m_iMode         = otherObj.m_iMode;

    m_pListField.clear();
    m_pListField += otherObj.m_pListField;
    if (NULL != m_pCond)
    {
        m_pCond = otherObj.m_pCond->clone();
    }
}

//======================== CMDConditionItem begin ===============================
CMDConditionItem::CMDConditionItem()
    : m_strName(QString()),
      m_iRelation(-1),
      m_iOperater(DB_QUERY::Oper_Undefine),
      m_varData(QVariant())
{

}

CMDConditionItem::~CMDConditionItem()
{

}

IMDConditionItem *CMDConditionItem::clone()
{
    CMDConditionItem *pTmpItem = new CMDConditionItem;
    *pTmpItem = *this;
    return pTmpItem;
}

void CMDConditionItem::operator=(const CMDConditionItem &otherObj)
{
    m_strName       = otherObj.m_strName;
    m_iOperater     = otherObj.m_iOperater;
    m_iRelation     = otherObj.m_iRelation;
    m_varData       = otherObj.m_varData;
}
