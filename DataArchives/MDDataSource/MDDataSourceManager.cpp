#include "MDDataSourceManager.h"
#include "MDDataSource.h"
#include <QDataStream>
#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

CMDDataSourceManager::CMDDataSourceManager()
{
    m_file                          = ""; //! 数据源序列化文件
    m_fileTmpDBPath                 = ""; //! 临时数据库文件存放路径
    m_dataSourceList.clear();
}

CMDDataSourceManager::~CMDDataSourceManager()
{
    removeAllDataSource();
}

/*!
\section 加载数据源
*/
bool CMDDataSourceManager::load(const QString &file)
{
    bool bSeccess = false;
    try
    {
        m_file = file;

        QFile streamFile(file);
        if (streamFile.open(QIODevice::ReadOnly))
        {
            QDataStream ar(&streamFile);
            int uVersion = 0;

            ar >> uVersion;
            if(uVersion >= 1)
            {
                int iCount = 0;
                ar >> iCount;
                for(int i = 0; i < iCount; ++i)
                {
                    IMDDataSource *pDBSource = new CMDDataSource;
                    pDBSource->serialize(&ar, false);
                    //pDBSource->setTempDatabasePath(m_file);
                    m_dataSourceList.push_back(pDBSource);
                }
            }

            bSeccess = true;
            streamFile.close();
        }
    }
    catch(...)
    {
        bSeccess = false;
    }

    return bSeccess;
}

bool CMDDataSourceManager::load_json(const QString &file)
{
    bool bSeccess = false;
    try
    {
        m_file = file;

        QFile streamFile(file);
        if (streamFile.open(QIODevice::ReadOnly))
        {
            QByteArray ba;
            QTextStream stream(&streamFile);
            stream.setCodec("UTF-8");
            ba = stream.readAll().toUtf8();
            streamFile.close();

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
            if(error.error != QJsonParseError::NoError)
                return false;

            QJsonObject json = doc.object();

            int uVersion = json.value("Version").toInt();

            QJsonArray dataSourceArr = json.value("DataSourceArray").toArray();
            int iCount = dataSourceArr.count();
            for(int i = 0; i < iCount; ++i)
            {
                IMDDataSource *pDBSource = new CMDDataSource;
                pDBSource->deserialize(dataSourceArr.at(i).toObject());
                //pDBSource->setTempDatabasePath(m_file);
                m_dataSourceList.push_back(pDBSource);
            }

            bSeccess = true;
            streamFile.close();
        }
    }
    catch(...)
    {
        bSeccess = false;
    }

    return bSeccess;
}

/*!
\section 存储数据源
*/
bool CMDDataSourceManager::save(const QString &file)
{
    bool bSeccess = false;

    try
    {
        m_file = file;

        QFile streamFile(file);
        if (streamFile.open(QIODevice::WriteOnly))
        {
            QDataStream ar(&streamFile);

            int uVersion = 1;

            ar << uVersion;
            if(uVersion >= 1)
            {
                int iCount = m_dataSourceList.size();
                ar << iCount;

                for(int i = 0; i < iCount; ++i)
                {
                   IMDDataSource *pDBSource = m_dataSourceList.at(i);
                   if(pDBSource)
                   {
                        pDBSource->serialize(&ar, true);
                   }
                }
            }

            streamFile.close();
            bSeccess = true;
        }
    }
    catch(...)
    {
        bSeccess = false;
    }

    return bSeccess;
}


bool CMDDataSourceManager::save_json(const QString &file)
{
    bool bSeccess = false;

    try
    {
        m_file = file;

        QFile streamFile(file);
        if (streamFile.open(QIODevice::WriteOnly))
        {
            QJsonObject json;
            int uVersion = 1;
            json.insert("Version", uVersion);

            QJsonArray dataSourceArr;
            int iCount = m_dataSourceList.size();
            for(int i = 0; i < iCount; ++i)
            {
               IMDDataSource *pDBSource = m_dataSourceList.at(i);
               if(pDBSource)
               {
                   QJsonObject dataSource;
                   pDBSource->serialize(dataSource);
                   dataSourceArr.append(dataSource);
               }
            }
            json.insert("DataSourceArray", dataSourceArr);

            QJsonDocument doc;
            doc.setObject(json);
            QByteArray ba = doc.toJson(/*QJsonDocument::Compact*/);
            QTextStream stream(&streamFile);
            stream.setCodec("UTF-8");
            stream << ba;
            streamFile.close();
            bSeccess = true;
        }
    }
    catch(...)
    {
        bSeccess = false;
    }

    return bSeccess;
}

/*!
\section 存储数据源
*/
bool CMDDataSourceManager::save()
{
    return save_json(m_file);
}

bool CMDDataSourceManager::addDataSource(IMDDataSource *pDBSource)
{
    bool success = false;
    if (pDBSource && (NULL == getDataSource(pDBSource->name())))
    {
//        pDBSource->setTempDatabasePath(m_fileTmpDBPath);
        m_dataSourceList.append(pDBSource);
        success = true;
    }

    return success;
}

/*!
\section 移除指定数据源
*/
bool CMDDataSourceManager::removeDataSource(IMDDataSource *pDBSource)
{
    bool success = false;
    if (pDBSource)
    {
        int index = m_dataSourceList.indexOf(pDBSource);
        if (-1 != index)
        {
            m_dataSourceList.removeAt(index);
            delete pDBSource;
            pDBSource = NULL;
            success = true;
        }
    }

    return success;
}

/*!
\section 删除所有数据源
*/
void CMDDataSourceManager::removeAllDataSource()
{
    if (0 < m_dataSourceList.size())
    {
        qDeleteAll(m_dataSourceList.begin(), m_dataSourceList.end());
        m_dataSourceList.clear();
    }
}

/*!
\section 获取数据源--通过名字
*/
IMDDataSource *CMDDataSourceManager::getDataSource(const QString &name)
{
    IMDDataSource *pDataSrc = NULL;

    for (int i = 0; i < m_dataSourceList.size(); ++i)
    {
        pDataSrc = m_dataSourceList.at(i);
        if (pDataSrc && (pDataSrc->name() == name))
        {
            return pDataSrc;
        }
    }

    return NULL;
}

/*!
\section 获取数据源--通过索引
*/
IMDDataSource *CMDDataSourceManager::getDataSource(int index)
{
     IMDDataSource *pDataSrc = NULL;

    if (0 <= index && index < m_dataSourceList.size())
    {
        pDataSrc = m_dataSourceList.at(index);
    }

    return pDataSrc;
}
