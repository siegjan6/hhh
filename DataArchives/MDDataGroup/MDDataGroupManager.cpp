#include "MDDataGroupManager.h"
#include "MDDataGroup.h"
#include "MDDataDefine.h"
#include "MDDataItem.h"

#include <QFile>
#include <QJsonDocument>
#include <QTextStream>
#include <QJsonArray>

CMDDataGroupManager::CMDDataGroupManager()
{
    m_alarmDataGroup         = NULL;  //! 报警组
    m_logDataGroup           = NULL;  //! 日志组
    m_fileName                     = "";    //! 数据组序列化文件位置
    initial();
}

CMDDataGroupManager::~CMDDataGroupManager()
{
    removeAllHisDataGroup();
    if (m_alarmDataGroup)
    {
        m_alarmDataGroup->removeAllItem();
        delete m_alarmDataGroup;
        m_alarmDataGroup = NULL;
    }

    if (m_logDataGroup)
    {
        m_logDataGroup->removeAllItem();
        delete m_logDataGroup;
        m_logDataGroup = NULL;
    }
}

/*!
\section 加载数据源
*/
bool CMDDataGroupManager::load(const QString &qfile)
{
    // TODO: 数据组存储配置需要改成json格式存储
    bool bSeccess = false;
    try
    {
        m_fileName = qfile;
        QFile file(qfile);
        if (file.open(QIODevice::ReadOnly))
        {
            QDataStream ar(&file);
            int uVersion = 0;
            ar >> uVersion;

            m_alarmDataGroup->serialize(&ar, false);
            m_logDataGroup->serialize(&ar, false);
            //uVersion==1

            int iCount = 0;
            ar >> iCount;
            for(int i = 0; i < iCount; i++)
            {
                IMDDataGroup *dataGroup = new CMDDataGroup();
                dataGroup->serialize(&ar, false);
                m_hisDataGroupList.append(dataGroup);
            }

            bSeccess = true;

            file.close();
        }
    }
    catch(...)
    {
        bSeccess = false;
    }
    return bSeccess;
}

bool CMDDataGroupManager::load_json(const QString &qfile)
{
    bool bSeccess = false;
    try
    {
        m_fileName = qfile;
        QFile file(qfile);
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray ba;
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            ba = stream.readAll().toUtf8();
            file.close();

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
            if(error.error != QJsonParseError::NoError)
                return false;

            QJsonObject json = doc.object();

            int uVersion = json.value("Version").toInt();

            QJsonObject alarmDataGroup = json.value("AlarmDataGroup").toObject();
            m_alarmDataGroup->deserialize(alarmDataGroup);
            QJsonObject logDataGroup = json.value("LogDataGroup").toObject();
            m_logDataGroup->deserialize(logDataGroup);
            //uVersion==1

            QJsonArray HistoryDataGroupArr = json.value("HistoryDataGroup").toArray();


            int iCount = HistoryDataGroupArr.count();
            for(int i = 0; i < iCount; i++)
            {
                IMDDataGroup *dataGroup = new CMDDataGroup();
                dataGroup->deserialize(HistoryDataGroupArr.at(i).toObject());
                m_hisDataGroupList.append(dataGroup);
            }

            bSeccess = true;

            file.close();
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
bool CMDDataGroupManager::save(const QString &qfile)
{
    bool bSeccess = false;
    try
    {
        m_fileName = qfile;

        QFile file(qfile);
        if (file.open(QIODevice::WriteOnly))
        {
            QDataStream ar(&file);
            int uVersion = 1;
            ar << uVersion;

            m_alarmDataGroup->serialize(&ar, true);
            m_logDataGroup->serialize(&ar, true);

            //uVersion==1
            int iCount = m_hisDataGroupList.size();
            ar << iCount;
            for (int i = 0; i < iCount; ++i)
            {
               IMDDataGroup *dataGroup = m_hisDataGroupList.at(i);
               if(NULL != dataGroup)
               {
                   dataGroup->serialize(&ar, true);
               }
            }
            bSeccess = true;
        }


    }
    catch(...)
    {
        bSeccess = false;
    }
    return bSeccess;
}


bool CMDDataGroupManager::save_json(const QString &qfile)
{
    bool bSeccess = false;
    try
    {
        m_fileName = qfile;

        QFile file(qfile);
        if (file.open(QIODevice::WriteOnly))
        {
            QJsonObject json;
            int uVersion = 1;
            json.insert("Version", uVersion);

            QJsonObject alarmDataGroup;
            m_alarmDataGroup->serialize(alarmDataGroup);
            json.insert("AlarmDataGroup", alarmDataGroup);
            QJsonObject logDataGroup;
            m_logDataGroup->serialize(logDataGroup);
            json.insert("LogDataGroup", logDataGroup);

            QJsonArray HistoryDataGroupArr;
            int iCount = m_hisDataGroupList.size();
            for (int i = 0; i < iCount; ++i)
            {
               IMDDataGroup *dataGroup = m_hisDataGroupList.at(i);
               if(NULL != dataGroup)
               {
                   QJsonObject hisDataGroup;
                   dataGroup->serialize(hisDataGroup);
                   HistoryDataGroupArr.append(hisDataGroup);
               }
            }

            json.insert("HistoryDataGroup", HistoryDataGroupArr);

            QJsonDocument doc;
            doc.setObject(json);
            QByteArray ba = doc.toJson(/*QJsonDocument::Compact*/);
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream << ba;
            file.close();

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
bool CMDDataGroupManager::save()
{
    return save_json(m_fileName);
}

/*!
\section 初始化系统固定的报警、日志组字段
*/
void CMDDataGroupManager::initial()
{
    m_alarmDataGroup = new CMDDataGroup();
    CMDDataGroup *pTmpGrp = dynamic_cast<CMDDataGroup*>(m_alarmDataGroup);
    if (pTmpGrp)
    {
        pTmpGrp->setName(ALARM_DATA_GROUP);
        pTmpGrp->setDescription(ALARM_DATA_GROUP);
        pTmpGrp->createDefaultAlarmItem();
    }

    m_logDataGroup  = new CMDDataGroup();
    pTmpGrp = dynamic_cast<CMDDataGroup*>(m_logDataGroup);
    if (pTmpGrp)
    {
        pTmpGrp->setName(LOG_DATA_GROUP);
        pTmpGrp->setDescription(LOG_DATA_GROUP);
        pTmpGrp->createDefaultLogItem();
    }
}

/*!
\section 增加历史组
*/
bool CMDDataGroupManager::addHisDataGroup(IMDDataGroup *dataGroup)
{
    bool success = false;
    if (dataGroup && (NULL == getHisDataGroup(dataGroup->name())))
    {
        m_hisDataGroupList.append(dataGroup);
        success = true;
    }

    return success;
}

/*!
\section 移除指定历史组
*/
bool CMDDataGroupManager::removeHisDataGroup(IMDDataGroup *dataGroup)
{
    bool success = false;
    if (dataGroup)
    {
        int index = m_hisDataGroupList.indexOf(dataGroup);
        if (-1 != index)
        {
            m_hisDataGroupList.removeAt(index);
            delete dataGroup;
            dataGroup = NULL;
            success = true;
        }
    }

    return success;
}

/*!
\section 删除所有历史组
*/
void CMDDataGroupManager::removeAllHisDataGroup()
{
    if (0 < m_hisDataGroupList.size())
    {
        qDeleteAll(m_hisDataGroupList.begin(), m_hisDataGroupList.end());
        m_hisDataGroupList.clear();
    }
}

/*!
\section 查找数据组--通过数据组名
*/
IMDDataGroup *CMDDataGroupManager::getDataGroup(const QString &name)
{
    if ((NULL != m_alarmDataGroup) && (name == m_alarmDataGroup->name()))
    {
        return m_alarmDataGroup;
    }
    else if ((NULL != m_logDataGroup) && (name == m_logDataGroup->name()))
    {
        return m_logDataGroup;
    }
    else
    {
        return getHisDataGroup(name);
    }
}

/*!
\section 查找历史组--通过历史组名
*/
IMDDataGroup *CMDDataGroupManager::getHisDataGroup(const QString &name)
{
    IMDDataGroup *pDataGrp = NULL;
    for (int i = 0; i < m_hisDataGroupList.size(); ++i)
    {
        pDataGrp = m_hisDataGroupList.at(i);
        if (pDataGrp && pDataGrp->name() == name)
        {
            return pDataGrp;
        }
    }

    return NULL;
}

/*!
\section 查找历史组--通过索引
*/
IMDDataGroup *CMDDataGroupManager::getHisDataGroup(int index)
{
    IMDDataGroup *pDataGrp = NULL;

    if (0 <= index && index < m_hisDataGroupList.size())
    {
        pDataGrp = m_hisDataGroupList.at(index);
    }

    return pDataGrp;
}

