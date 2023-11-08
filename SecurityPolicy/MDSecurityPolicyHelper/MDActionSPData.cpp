#include "MDActionSPData.h"
#include <QJsonArray>
#include <QJsonObject>

CMDActionSPData::CMDActionSPData()
{
    m_version = 0;

    m_protectMode = MDActionProtectMode::null_mode;
    m_sigType = MDSigType::operator_only;

    m_enableLog = false;
    m_needSignature = true;
}

CMDActionSPData::~CMDActionSPData()
{

}

CMDActionSPData::CMDActionSPData(const CMDActionSPData& obj)
{
    copy(&obj);
}

CMDActionSPData& CMDActionSPData::operator = (const CMDActionSPData& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDActionSPData::copy(const CMDActionSPData* source)
{
    m_version = source->m_version;

    m_actionID.copy(&source->m_actionID);
    m_protectMode = source->m_protectMode;
    m_sigType = source->m_sigType;
    m_enableLog = source->m_enableLog;

    m_operatorGroupList.clear();
    if(source->m_operatorGroupList.count() > 0)
        m_operatorGroupList.append(source->m_operatorGroupList);
    m_verifierGroupList.clear();
    if(source->m_verifierGroupList.count() > 0)
        m_verifierGroupList.append(source->m_verifierGroupList);

    m_needSignature = source->m_needSignature;
    return true;
}

// 按照版本序列化/反序列化
const quint8 CURRENT_VERSION_ASPD = 1;
bool CMDActionSPData::serialize(QDataStream& stream, bool useCurrentVersion)
{
    bool result = true;

    quint8 actualVersion = (useCurrentVersion ? CURRENT_VERSION_ASPD : m_version);
    stream << actualVersion;
    if(actualVersion <= 1)
        serializeOnVersion_1(stream, useCurrentVersion);
    else
        result = false;
    return result;
}
void CMDActionSPData::deserialize(QDataStream& stream)
{
    stream >> m_version;
    if(m_version <= 1)
        deserializeOnVersion_1(stream);
    else{}
}

// 版本 1
void CMDActionSPData::serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion)
{
    quint8 actionProtectMode = (quint8)m_protectMode;
    quint8 sigType = (quint8)m_sigType;

    m_actionID.serialize(stream, useCurrentVersion);
    stream << actionProtectMode << sigType;
    stream << m_enableLog;
    stream << m_operatorGroupList;
    stream << m_verifierGroupList;
}
void CMDActionSPData::deserializeOnVersion_1(QDataStream& stream)
{
    quint8 actionProtectMode;
    quint8 sigType;

    m_actionID.deserialize(stream);
    stream >> actionProtectMode >> sigType;
    stream >> m_enableLog;
    stream >> m_operatorGroupList;
    stream >> m_verifierGroupList;

    m_protectMode = (MDActionProtectMode)actionProtectMode;
    m_sigType = (MDSigType)sigType;
}

void CMDActionSPData::serialize(QJsonObject& json)
{
    QJsonObject ActionIDJson;
    m_actionID.serialize(ActionIDJson);
    // ActionID
    json.insert("@OKI", ActionIDJson);
    // ActionProtectMode
    json.insert("@PM", (quint8)m_protectMode);
    // SigType
    json.insert("@ST", (quint8)m_sigType);
    // EnableLog
    json.insert("@EL", m_enableLog ? 1 : 0);
    QJsonArray operatorGroupArray;
    foreach (const QString& op, m_operatorGroupList)
        operatorGroupArray.append(op);
    // OperatorGroupList
    json.insert("WorkerGroupArray", operatorGroupArray);
    QJsonArray verifierGroupArray;
    foreach (const QString& vp, m_verifierGroupList)
        verifierGroupArray.append(vp);
    // VerifierGroupList
    json.insert("InspectorGroupArray", verifierGroupArray);
}

void CMDActionSPData::deserialize(const QJsonObject& json)
{
    QJsonObject ActionIDJson = json.value("@OKI").toObject();
    m_actionID.deserialize(ActionIDJson);

    quint8 actionProtectMode = json.value("@PM").toInt();
    m_protectMode = (MDActionProtectMode)actionProtectMode;
    quint8 sigType = json.value("@ST").toInt();
    m_sigType = (MDSigType)sigType;
    if(json.value("@EL").toString().toLower()=="true")
        m_enableLog = true;
    else if(json.value("@EL").toString().toLower()=="false")
        m_enableLog = false;
    else
        m_enableLog = json.value("@EL").toInt() ? true : false;

    QJsonArray operatorGroupArray = json.value("WorkerGroupArray").toArray();
    int opCount = operatorGroupArray.count();
    for(int i = 0; i < opCount; i++)
        m_operatorGroupList.append(operatorGroupArray.at(i).toString());
    QJsonArray verifierGroupArray = json.value("InspectorGroupArray").toArray();
    int vpCount = verifierGroupArray.count();
    for(int j = 0; j < vpCount; j++)
        m_verifierGroupList.append(verifierGroupArray.at(j).toString());
}
