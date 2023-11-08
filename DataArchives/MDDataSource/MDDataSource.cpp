#include "MDDataSource.h"
#include "IMDDatabaseParam.h"
#include "IMDDataTable.h"
#include "MDDataTable.h"
#include "MDDatabaseParam.h"
#include <QList>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>

CMDDataSource::CMDDataSource()
{
    m_name                          = "";       //! 数据源名字
    m_description                   = "";       //! 数据源描述
    m_dataTablePrefix               = "";       //! 数据表前缀
    m_isEnable                         = false;    //! 数据源启用标识
    m_isEnableDataTablePrefix          = false;    //! 数据源创建表是否启用表前缀标识
    m_isEnableCreateDataBase           = false;    //! 是否创建数据库标识
    m_isEnableVerificationA            = false;    //! 启用数据验证方法A标识
    m_isEnableVerificationB            = false;    //! 启用数据验证方法B标识
    m_createDefaultTable               = false;    //! 是否创建默认表标识
    m_isShowTable                      = false;    //! 是否显示表标识
    m_mdDataBaseBParam    = NULL;     //! 数据库参数

    m_dataTableList.clear();
    m_mdDataBaseBParam = new CMDDataBaseParam();
}

CMDDataSource::~CMDDataSource()
{
    removeAllDataTable();

    if(m_mdDataBaseBParam)
    {
        delete m_mdDataBaseBParam;
        m_mdDataBaseBParam = NULL;
    }
}

/*!
\section 克隆函数
*/
IMDDataSource *CMDDataSource::clone()
{
    CMDDataSource *pDataSrc = new CMDDataSource();
    *pDataSrc = *this;
    return pDataSrc;
}

/*!
\section 赋值运算符
*/
void CMDDataSource::operator=(const CMDDataSource &otherObj)
{
    m_name                      = otherObj.m_name;
    m_description               = otherObj.m_description;
    m_dataTablePrefix           = otherObj.m_dataTablePrefix;
    m_isEnable                  = otherObj.m_isEnable;
    m_isEnableDataTablePrefix   = otherObj.m_isEnableDataTablePrefix;
    m_createDefaultTable        = otherObj.m_createDefaultTable;
    m_isShowTable               = otherObj.m_isShowTable;
    m_isEnableCreateDataBase    = otherObj.m_isEnableCreateDataBase;
    m_isEnableVerificationA     = otherObj.m_isEnableVerificationA;
    m_isEnableVerificationB     = otherObj.m_isEnableVerificationB;
    m_mdDataBaseBParam          = otherObj.m_mdDataBaseBParam->clone();
    removeAllDataTable();
    int iCount = otherObj.m_dataTableList.count();
    for(int i = 0; i < iCount; ++i)
    {
        IMDDataTable *pDataTable = otherObj.m_dataTableList.at(i);
        if(pDataTable)
        {
            addDataTable(pDataTable->clone());
        }
    }
}

/*!
\section 序列化
*/
void CMDDataSource::serialize(QDataStream *ar, bool bSave)
{
    if(NULL == ar)
    {
        return;
    }

    if(bSave)
    {
        quint32 uVersion = 1;
        *ar << uVersion;
        if(uVersion >= 1)
        {
            *ar << m_name;
            *ar << m_description;
            *ar << m_dataTablePrefix;
            *ar << m_isEnable;
            *ar << m_isEnableDataTablePrefix;
            *ar << m_isEnableCreateDataBase;
            *ar << m_isEnableVerificationA;
            *ar << m_isEnableVerificationB;
            *ar << m_createDefaultTable;
            *ar << m_isShowTable;

            m_mdDataBaseBParam->serialize(ar, bSave);

            int iCount = m_dataTableList.size();
            *ar << iCount;

            for (int i = 0; i < iCount; ++i)
            {
                IMDDataTable *pDataTable = m_dataTableList.at(i);
                if(pDataTable)
                {
                    pDataTable->serialize(ar, bSave);
                }
            }
        }
    }
    else
    {
        quint32 uVersion = 0;

        *ar >> uVersion;
        if(uVersion >= 1)
        {
            *ar >> m_name;
            *ar >> m_description;
            *ar >> m_dataTablePrefix;
            *ar >> m_isEnable;
            *ar >> m_isEnableDataTablePrefix;
            *ar >> m_isEnableCreateDataBase;
            *ar >> m_isEnableVerificationA;
            *ar >> m_isEnableVerificationB;
            *ar >> m_createDefaultTable;
            *ar >> m_isShowTable;

            if(m_mdDataBaseBParam)
            {
                m_mdDataBaseBParam->serialize(ar, bSave);
            }

            int iCount = m_dataTableList.size();
            *ar >> iCount;

            for (int i = 0; i < iCount; ++i)
            {
                IMDDataTable *pDataTable = new CMDDataTable();
                if(pDataTable)
                {
                    pDataTable->serialize(ar, bSave);
                    addDataTable(pDataTable);
                }
            }
        }
    }
}

void CMDDataSource::serialize(QJsonObject& json)
{
    json.insert("Name", m_name);
    json.insert("Desc", m_description);
    json.insert("TablePrefix", m_dataTablePrefix);
    json.insert("Enable", m_isEnable);
    json.insert("EnableDataTablePrefix", m_isEnableDataTablePrefix);
    json.insert("EnableCreateDataBase", m_isEnableCreateDataBase);
    json.insert("EnableVerificationA", m_isEnableVerificationA);
    json.insert("EnableVerificationB", m_isEnableVerificationB);
    json.insert("createDefaultTable", m_createDefaultTable);
    json.insert("Visible", m_isShowTable);

    QJsonObject dbparam;
    m_mdDataBaseBParam->serialize(dbparam);
    json.insert("DatabaseParam", dbparam);

    QJsonArray dataTableArr;
    int iCount = m_dataTableList.size();
    for (int i = 0; i < iCount; ++i)
    {
        IMDDataTable *pDataTable = m_dataTableList.at(i);
        if(pDataTable)
        {
            QJsonObject dataTable;
            pDataTable->serialize(dataTable);
            dataTableArr.append(dataTable);
        }
    }
    json.insert("TableArray", dataTableArr);
}

void CMDDataSource::deserialize(QJsonObject json)
{
    m_name = json.value("Name").toString();
    m_description = json.value("Desc").toString();
    m_dataTablePrefix = json.value("TablePrefix").toString();
    m_isEnable = json.value("Enable").toBool();
    m_isEnableDataTablePrefix = json.value("EnableDataTablePrefix").toBool();
    m_isEnableCreateDataBase = json.value("EnableCreateDataBase").toBool();
    m_isEnableVerificationA = json.value("EnableVerificationA").toBool();
    m_isEnableVerificationB = json.value("EnableVerificationB").toBool();
    m_createDefaultTable = json.value("createDefaultTable").toBool();
    m_isShowTable = json.value("Visible").toBool();

    if(m_mdDataBaseBParam)
    {
        QJsonObject dbparam = json.value("DatabaseParam").toObject();
        m_mdDataBaseBParam->deserialize(dbparam);
    }

    QJsonArray dataTableArr = json.value("TableArray").toArray();

    int iCount = dataTableArr.count();
    for (int i = 0; i < iCount; ++i)
    {
        IMDDataTable *pDataTable = new CMDDataTable();
        if(pDataTable)
        {
            pDataTable->deserialize(dataTableArr.at(i).toObject());
            addDataTable(pDataTable);
        }
    }
}

/*!
\section 设置临时数据库保存路径
*/
void CMDDataSource::setTempDatabasePath(const QString &path)
{
    if (m_mdDataBaseBParam)
    {
        QString strPath = path + "/TempDatabase";
        QDir dir(strPath);
        if (!dir.exists())
        {
            dir.mkdir(strPath);
        }
        m_mdDataBaseBParam->setDBPath(strPath);
        m_mdDataBaseBParam->setDbBackUpPath(strPath);
    }
}

/*!
\section 增加表
*/
bool CMDDataSource::addDataTable(IMDDataTable *dataTable)
{
    bool success = false;
    if (dataTable && (NULL == getDataTable(dataTable->name())))
    {
        m_dataTableList.append(dataTable);
        success = true;
    }

    return success;
}

/*!
\section 移除指定表
*/
bool CMDDataSource::removeDataTable(IMDDataTable *dataTable)
{
    bool success = false;
    if (dataTable)
    {
        for (int i = 0; i < dataTableCount(); ++i)
        {
            if (dataTable == m_dataTableList.at(i))
            {
                m_dataTableList.removeAt(i);
                delete dataTable;
                dataTable = NULL;
                success = true;
                return success;
            }
        }
//        int index = m_dataTableList.indexOf(dataTable);
//        if (-1 != index)
//        {
//            m_dataTableList.removeAt(index);
//            success = true;
//        }
    }

    return success;
}

/*!
\section 删除所有表
*/
void CMDDataSource::removeAllDataTable()
{
    if (0 < m_dataTableList.size())
    {
        qDeleteAll(m_dataTableList.begin(), m_dataTableList.end());
        m_dataTableList.clear();
    }
}

/*!
\section 通过表名查找表
*/
IMDDataTable *CMDDataSource::getDataTable(const QString &name)
{
    IMDDataTable *pDataTable = NULL;
    for (int i = 0; i < m_dataTableList.size(); ++i)
    {
        pDataTable = m_dataTableList.at(i);
        if (pDataTable && (pDataTable->name() == name))
        {
            return pDataTable;
        }
    }

    return NULL;
}

/*!
\section 通过索引查找表
*/
IMDDataTable *CMDDataSource::getDataTable(int index)
{
    IMDDataTable *pDataTable = NULL;

    if (0 <= index && index < m_dataTableList.size())
    {
        pDataTable = m_dataTableList.at(index);
    }

    return pDataTable;
}

/*!
  通过数据组名查找表

\param name 查找的数据组名。
\param nList 返回所有数据组名相同的表。
\returns 无。
\note 无。
*/
void CMDDataSource::getDataTableByGroupName(const QString &name, QList<IMDDataTable *> &nList)
{
    IMDDataTable *pDataTable = NULL;
    for (int i = 0; i < m_dataTableList.size(); ++i)
    {
        pDataTable = m_dataTableList.at(i);
        if (pDataTable && (pDataTable->dataGroupName() == name))
        {
            nList.append(pDataTable);
        }
    }
}
