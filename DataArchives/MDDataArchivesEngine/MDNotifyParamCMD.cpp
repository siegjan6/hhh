#include "MDNotifyParamCMD.h"
#include "MDDatabaseService.h"
#include "IMDDatabaseParam.h"
#include "IMDDataSource.h"
#include "IMDDataTable.h"
#include "IMDDataField.h"
#include "IMDDataItem.h"
#include "IMDDataArchives.h"
#include "IMDDataGroupManager.h"
#include "IMDDataGroup.h"
#include "MDDataDefine.h"
#include "MDDatabaseCheckCMD.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

//TODO:命称与实际功能不匹配 需重新命名
CMDNotifyParamCMD::CMDNotifyParamCMD(int dbType, DataArchives::DB_NOTIFY_TYPE type,
                                                                                CMDDataArchivesNotifyParam *pParam, CMDDBSourceService * pService)
    : m_dbType(dbType),
      m_dbNotifyType(type),
      m_pService(pService)
{
    m_pDBNotifyParam = *pParam;
}

CMDNotifyParamCMD::~CMDNotifyParamCMD()
{
}

void CMDNotifyParamCMD::Do()
{
    switch (m_dbNotifyType)
    {
    case DataArchives::DB_NOTIFY_ADD:
        constructAddSql();
        break;
    case DataArchives::DB_NOTIFY_UPDATE:
        constructUpdateSql();
        break;
    case DataArchives::DB_NOTIFY_DELETE:
        constructDeleteSql();
        break;
    case DataArchives::DB_NOTIFY_ALM_OFF:

        break;
    default:
        break;
    }
}

//构造SQL并执行SQL
void CMDNotifyParamCMD::constructAddSql()
{
    if (NULL == m_pService)
    {
        return;
    }

    /*
     * 数据库的插入命令构造方法有两种：占位符与直接Sql语句
     * 此方法中都对两种方式都进行构造了，建议不需要采用占位符的方式来操作，实际测下来
     * 直接sql语句，也很实时，也很快速
     *
     */
    QList<IMDDataTable*> tableList;

    //TODO:一个数据源会允许多个同名表存在么
    m_pService->dataSource()->getDataTableByGroupName(m_pDBNotifyParam.dataGroupName(), tableList);
    if (tableList.isEmpty())
    {
        return; //数据丢了
    }

    QString strSQLCmd       = "";
    QString strData         = "";
    QString strTab          = "";

    IMDDataTable *pTable    = NULL;
    IMDDataField *pField    = NULL;
    IMDDataItem *pItem      = NULL;

    int fieldCount          = 0;

    //TODO:这种拼接字符串的方法会反复执行new操作吧
    for (int i = 0; i < tableList.count(); ++i)
    {
        pTable = tableList.at(i); //从链表中取出并移除元素
        if (NULL == pTable)
        {
            continue;
        }

        strTab = pTable->name();
        if (m_pService->dataSource()->isEnableDataTablePrefix()
                && !m_pService->dataSource()->dataTablePrefix().isEmpty())
        {
            strTab = m_pService->dataSource()->dataTablePrefix() + "_" + pTable->name();
        }

        strSQLCmd = "insert into ";
        strSQLCmd += strTab;
        strSQLCmd += " values ( ";

        fieldCount = pTable->dataFieldCount();
        for (int j = 0; j < fieldCount; ++j)
        {
            pField = pTable->getDataField(j);
            if (nullptr == pField)
            {
                continue;
            }

            QVariant vExtType = pField->ext("type");
            if(!vExtType.isNull() && vExtType.isValid())
            {
                if ("virtual" == vExtType.toString())
                {
                    continue;
                }
            }

            QString fieldItemName = pField->dataItemName();
            if(fieldItemName.isEmpty())
            {
                strData.clear();
                strData = "NULL, ";
            }
            else
            {
                pItem = m_pDBNotifyParam.getItem(pField->dataItemName());
                if (NULL == pItem)
                {
                    MDLOG_INFO_TO_LOGGER(QString("engine---can not find field '%1'\'s value!").arg(pField->name()));
                    return;
                }

                strData.clear();
                //根据datatype跟data转换成Sql语句字串
                strData = m_pService->variantDataToString(pItem->dataType(), pItem->data());
            }

            strSQLCmd += strData;
        }

        //去除sql语句中的最后两个字符:逗号与空格
        strSQLCmd = strSQLCmd.mid(0, strSQLCmd.size() - 2);
        strSQLCmd += ");";
        qDebug() << strSQLCmd;

        if(!m_pService->executeInsertSQL(strSQLCmd))
            m_pService->SaveSQLToTempDB(strSQLCmd);
    }//end for
}

void CMDNotifyParamCMD::constructUpdateSql()
{
    if (NULL == m_pService)
        return; //TODO:log

    QList<IMDDataTable*> tableList;
    m_pService->dataSource()->getDataTableByGroupName(m_pDBNotifyParam.dataGroupName(), tableList);
    if (tableList.isEmpty())
    {
        return; //TODO:log
    }

    QString strSQLCmd       = "";
    QString strData         = "";
    QString strTab          = "";

    IMDDataTable *pTable    = nullptr;
    IMDDataItem *pItem      = nullptr;
    IMDDataField *pField    = nullptr;

    int fieldCount          = 0;

    for (int i = 0; i < tableList.count(); ++i)
    {
        pTable = tableList.at(i);
        if (nullptr == pTable)
        {
            continue;
        }        

        strTab = pTable->name();
        if (m_pService->dataSource()->isEnableDataTablePrefix()
                && !m_pService->dataSource()->dataTablePrefix().isEmpty())
        {
            strTab = m_pService->dataSource()->dataTablePrefix() + "_" +  pTable->name();;
        }

        strSQLCmd = "update ";
        strSQLCmd += strTab;
        strSQLCmd += " set ";

        fieldCount = pTable->dataFieldCount();
        for (int k = 0; k < fieldCount; ++k)
        {
            pField = pTable->getDataField(k);
            if (nullptr == pField)
            {
                continue;
            }

            QVariant vExtType = pField->ext("type");
            if(!vExtType.isNull() && vExtType.isValid())
            {
                if ("virtual" == vExtType.toString())
                {
                    continue;
                }
            }

            pItem = m_pDBNotifyParam.getItem(pField->dataItemName());
            if (nullptr == pItem)
            {
                MDLOG_INFO_TO_LOGGER(QString("CMDNotifyParamCMD::constructUpdateSql: item %1 not exist in the %2 .").arg(pField->dataItemName()).arg(pTable->name()));
                return;
            }

            strData.clear();
            strData = m_pService->variantDataToString(pItem->dataType(), pItem->data());

            strSQLCmd += pField->name();
            strSQLCmd += " = " + strData;
        }

        pField = pTable->getDataField(ALM_ID);
        pItem = m_pDBNotifyParam.getItem(ALM_ID);
        if (nullptr == pField || nullptr == pItem)
        {
            MDLOG_INFO_TO_LOGGER(QString("CMDNotifyParamCMD::constructUpdateSql: item %1 not exist in the %2 .").arg(ALM_ID).arg(pTable->name()));
            return;
        }

        strSQLCmd = strSQLCmd.mid(0, strSQLCmd.size() - 2);
        strData.clear();
        strData = m_pService->variantDataToString(pItem->dataType(), pItem->data());
        strSQLCmd += " where " + pField->name() + " = " + strData;
        strSQLCmd = strSQLCmd.mid(0, strSQLCmd.size() - 2);
        strSQLCmd += ";";

        m_pService->executeCommonSQL(strSQLCmd);
    }
}

void CMDNotifyParamCMD::constructDeleteSql()
{
    if (NULL == m_pService)
    {
        return; //TODO:log
    }

    QList<IMDDataTable*> tableList;
    m_pService->dataSource()->getDataTableByGroupName(m_pDBNotifyParam.dataGroupName(), tableList);
    if (tableList.isEmpty())
    {
        return; //TODO:log
    }

    //SQL语句
    QString strSQLCmd;
    QString strData;
    QString strTab;

    IMDDataTable *pTable    = NULL;
    IMDDataItem *pItem      = NULL;
    IMDDataField *pField    = NULL;

    for (int i = 0; i < tableList.count(); ++i)
    {
        pTable = tableList.at(i);
        if (NULL == pTable)
        {
            continue;
        }

        strTab = pTable->name();
        if (m_pService->dataSource()->isEnableDataTablePrefix()
                && !m_pService->dataSource()->dataTablePrefix().isEmpty())
        {
            strTab = m_pService->dataSource()->dataTablePrefix() + "_" +  pTable->name();;
        }

        strSQLCmd = "delete from ";
        strSQLCmd += strTab;

        pField = pTable->getDataField(ALM_ID);
        pItem = m_pDBNotifyParam.getItem(ALM_ID);
        if (NULL == pField || NULL == pItem)
        {
            MDLOG_INFO_TO_LOGGER(QString("engine---drop sql: item ALM_ID not exist in the %1 table.").arg(strTab));
            return;
        }

        strData = m_pService->variantDataToString(pItem->dataType(), pItem->data());
        strSQLCmd += " where " + pField->name() + " = " + strData;
        strSQLCmd = strSQLCmd.mid(0, strSQLCmd.size() - 2);
        strSQLCmd += ";";
        m_pService->executeCommonSQL(strSQLCmd);
    }
}
