#include "MDActionSPListData.h"
#include "MDActionSP.h"
#include <QJsonArray>
#include <QJsonObject>

CMDActionSPListData::CMDActionSPListData()
{
    m_version = 0;
}

CMDActionSPListData::~CMDActionSPListData()
{
    release();
}

CMDActionSPListData::CMDActionSPListData(const CMDActionSPListData& obj)
{
    copy(&obj);
}

CMDActionSPListData& CMDActionSPListData::operator = (const CMDActionSPListData& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDActionSPListData::copy(const CMDActionSPListData* source)
{
    m_version = source->m_version;

    release();

    int count = source->m_spList.count();
    for(int i = 0; i < count; i++)
    {
        CMDActionSP* srcSP = source->m_spList.at(i);
        CMDActionSP* destSP = new CMDActionSP(*srcSP);
        m_spList.append(destSP);
    }

    return true;
}

void CMDActionSPListData::release()
{
    int count = m_spList.count();
    for(int i = 0; i < count; i++)
    {
        CMDActionSP* sp = m_spList.at(i);
        delete sp;
    }
    m_spList.clear();
}

CMDActionSP* CMDActionSPListData::find(MDActionType type)
{
    int count = m_spList.count();
    for(int i = 0; i < count; i++)
    {
        CMDActionSP* sp = m_spList.at(i);
        if(sp->actionID().type() == type)
            return sp;
    }
    return NULL;
}

void CMDActionSPListData::enumEnabled(QList<CMDActionSP*>& spList)
{
    int count = m_spList.count();
    for(int i = 0; i < count; i++)
    {
        CMDActionSP* sp = m_spList.at(i);
        if(sp->protectMode() != MDActionProtectMode::null_mode)
        {
            spList.append(sp);
        }
    }
}

void CMDActionSPListData::deleteAllDisabled()
{
    QList<CMDActionSP*>::iterator i = m_spList.begin();
    while (i != m_spList.end())
    {
        CMDActionSP* sp = *i;
        if(sp->protectMode() == MDActionProtectMode::null_mode)
        {
            delete sp;
            i = m_spList.erase(i);
        }
        else ++i;
    }
}

void CMDActionSPListData::update(const CMDActionSPList& srcList, const QString& objName)
{
    int count = m_spList.count();
    for(int i = 0; i < count; i++)
    {
        CMDActionSP* destSP = m_spList.at(i);

        CMDActionSP* srcSP = srcList.find(destSP->actionID().type());
        if(srcSP)
            *destSP = *srcSP;

        destSP->actionID().setObjName(objName);
    }
}

// 按照版本序列化/反序列化
const quint8 CURRENT_VERSION_ASPLD = 1;
bool CMDActionSPListData::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_ASPLD : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else
        result = false;
    return result;
}
void CMDActionSPListData::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

// 版本 1
void CMDActionSPListData::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{
    qint16 count = (qint16)m_spList.count();

    stream << count;
    for(int i = 0; i < count; i++)
    {
        CMDActionSP* sp = m_spList.at(i);
        sp->serialize(stream, useCurrentVersion);
    }
}
void CMDActionSPListData::deserializeOnVersion_1(QDataStream& stream)
{
    qint16 count;
    stream >> count;
    for(int i = 0; i < count; i++)
    {
        CMDActionSP* destSP = new CMDActionSP();
        destSP->deserialize(stream);
        m_spList.append(destSP);
    }
}

void CMDActionSPListData::serialize(QJsonArray& json)
{
    foreach (CMDActionSP* sp, m_spList)
    {
        QJsonObject spJson;
        sp->serialize(spJson);
        json.append(spJson);
    }
}

void CMDActionSPListData::deserialize(const QJsonArray& json)
{
    int spCount = json.count();
    for(int i = 0; i < spCount; i++)
    {
        CMDActionSP* destSP = new CMDActionSP();
        destSP->deserialize(json.at(i).toObject());
        m_spList.append(destSP);
    }
}

