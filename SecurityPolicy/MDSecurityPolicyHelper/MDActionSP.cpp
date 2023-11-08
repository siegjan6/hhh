#include "MDActionSP.h"
#include "MDActionSPData.h"
#include "MDActionSPListData.h"

const QStringList EMPTY_STRINGLIST;

/*!
 * \brief 安全策略数据类
 */
CMDActionSP::CMDActionSP()
{
    m_data = new CMDActionSPData;
}

CMDActionSP::CMDActionSP(MDActionType type)
{
    m_data = new CMDActionSPData;
    if(m_data)
        m_data->m_actionID.setType(type);
}

CMDActionSP::~CMDActionSP()
{
    if(m_data)
        delete m_data;
}

CMDActionSP::CMDActionSP(const CMDActionSP& obj)
{
    m_data = new CMDActionSPData();

    copy(&obj);
}

CMDActionSP& CMDActionSP::operator = (const CMDActionSP& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

/*!
\section 属性
*/

CMDActionID& CMDActionSP::actionID() const
{
    return (m_data ? m_data->m_actionID : CMDActionID::theDefault());
}
void CMDActionSP::setActionID(const CMDActionID& actionID)
{
    if(m_data)
        m_data->m_actionID = actionID;
}

QString CMDActionSP::actionIDObjName() const
{
    return (m_data ? m_data->m_actionID.objName() : "");
}

MDActionProtectMode CMDActionSP::protectMode() const
{
    return (m_data ? m_data->m_protectMode : MDActionProtectMode::null_mode);
}
void CMDActionSP::setProtectMode(MDActionProtectMode mode)
{
    if(m_data)
        m_data->m_protectMode = mode;
}
bool CMDActionSP::isEnable() const
{
    return (MDActionProtectMode::null_mode != protectMode());
}

MDSigType CMDActionSP::sigType() const
{
    return (m_data ? m_data->m_sigType : MDSigType::null_type);
}
void CMDActionSP::setSigType(MDSigType type)
{
    if(m_data)
        m_data->m_sigType = type;
}

bool CMDActionSP::isEnableLog() const
{
    return (m_data ? m_data->m_enableLog : false);
}
void CMDActionSP::setEnableLog(bool enable)
{
    if(m_data)
        m_data->m_enableLog = enable;
}

bool CMDActionSP::needSignature() const
{
    return (m_data ? m_data->m_needSignature : false);
}
void CMDActionSP::setNeedSignature(bool flag)
{
    if(m_data)
        m_data->m_needSignature = flag;
}

const QStringList& CMDActionSP::operatorGroupList() const
{
    return (m_data ? m_data->m_operatorGroupList : EMPTY_STRINGLIST);
}
void CMDActionSP::setOperatorGroupList(const QStringList& groupList)
{
    if(m_data)
    {
        m_data->m_operatorGroupList.clear();
        m_data->m_operatorGroupList.append(groupList);
    }
}

const QStringList& CMDActionSP::verifierGroupList() const
{
    return (m_data ? m_data->m_verifierGroupList : EMPTY_STRINGLIST);
}
void CMDActionSP::setVerifierGroupList(const QStringList& groupList)
{
    if(m_data)
    {
        m_data->m_verifierGroupList.clear();
        m_data->m_verifierGroupList.append(groupList);
    }
}

bool CMDActionSP::copy(const CMDActionSP* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

/*!
\section 序列化
*/
bool CMDActionSP::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDActionSP::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CMDActionSP::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDActionSP::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}

/*!
\section 辅助函数
*/

// 保护模式
void CMDActionSP::enumProtectModes(QStringList& modeList)
{
   modeList.append(modeToString(MDActionProtectMode::null_mode));
   modeList.append(modeToString(MDActionProtectMode::MindSCADA_Sig));
   modeList.append(modeToString(MDActionProtectMode::Electronic_Sig));
}

const QString STRING_NO_SECURE = QObject::tr("NoSecure");
const QString STRING_MINDSCADA_SIG = QObject::tr("MindSCADA_Sig");
const QString STRING_ELECTRONIC_SIG = QObject::tr("Electronic_Sig");
const QString STRING_OPERATOR_ONLY = QObject::tr("operator_only");
const QString STRING_OPERATOR_THEN_VERIFIER = QObject::tr("operator_then_verifier");
QString CMDActionSP::modeToString(MDActionProtectMode mode)
{
   QString str;
   switch (mode) {
   case MDActionProtectMode::null_mode: str = STRING_NO_SECURE;
       break;
   case MDActionProtectMode::MindSCADA_Sig: str = STRING_MINDSCADA_SIG;
       break;
   case MDActionProtectMode::Electronic_Sig: str = STRING_ELECTRONIC_SIG;
       break;
   default:
       break;
   }
   return str;
}

MDActionProtectMode CMDActionSP::stringToMode(const QString& str)
{
   MDActionProtectMode m = MDActionProtectMode::null_mode;
   if(0 == str.compare(STRING_NO_SECURE))
       m = MDActionProtectMode::null_mode;
   else if(0 == str.compare(STRING_MINDSCADA_SIG))
       m = MDActionProtectMode::MindSCADA_Sig;
   else if(0 == str.compare(STRING_ELECTRONIC_SIG))
       m = MDActionProtectMode::Electronic_Sig;
   else{};
   return m;
}

// 签名类型
void CMDActionSP::enumSigTypes(QStringList& typeList)
{
   typeList.append(sigTypeToString(MDSigType::operator_only));
   typeList.append(sigTypeToString(MDSigType::operator_then_verifier));
}

QString CMDActionSP::sigTypeToString(MDSigType type)
{
   QString str;
   switch (type) {
   case MDSigType::operator_only: str = STRING_OPERATOR_ONLY;
       break;
   case MDSigType::operator_then_verifier: str = STRING_OPERATOR_THEN_VERIFIER;
       break;
   default:
       break;
   }
   return str;
}

MDSigType CMDActionSP::stringToSigType(const QString& str)
{
   MDSigType type = MDSigType::null_type;
   if(0 == str.compare(STRING_OPERATOR_ONLY))
       type = MDSigType::operator_only;
   else if(0 == str.compare(STRING_OPERATOR_THEN_VERIFIER))
       type = MDSigType::operator_then_verifier;
   else{};
   return type;
}

/*!
 * \brief 安全策略链表数据类
 */
CMDActionSPList::CMDActionSPList()
{
    m_data = new CMDActionSPListData;
}

CMDActionSPList::~CMDActionSPList()
{
    if(m_data)
        delete m_data;
}

CMDActionSPList::CMDActionSPList(const CMDActionSPList& obj)
{
    m_data = new CMDActionSPListData();

    copy(&obj);
}

CMDActionSPList& CMDActionSPList::operator = (const CMDActionSPList& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

/*!
\section CMDActionSP 对象管理
*/
bool CMDActionSPList::append(CMDActionSP* newSP)
{
    if(!m_data)
    {
        delete newSP;
        return false;
    }
    m_data->m_spList.append(newSP);
    return true;
}

CMDActionSP* CMDActionSPList::append(MDActionType type)
{
    if(!m_data)
        return NULL;

    CMDActionSP* newSP = new CMDActionSP(type);
    m_data->m_spList.append(newSP);
    return newSP;
}

int CMDActionSPList::count() const
{
    return (m_data ? m_data->m_spList.count() : 0);
}

void CMDActionSPList::deleteAll()
{
    if(m_data)
        m_data->release();
}

CMDActionSP* CMDActionSPList::at(int index) const
{
    return (m_data ? m_data->m_spList.at(index) : NULL);
}

CMDActionSP* CMDActionSPList::find(MDActionType type) const
{
    return (m_data ? m_data->find(type) : NULL);
}

void CMDActionSPList::enumEnabled(QList<CMDActionSP*>& spList) const
{
    if(m_data)
        m_data->enumEnabled(spList);
}

void CMDActionSPList::deleteAllDisabled()
{
    if(m_data)
        m_data->deleteAllDisabled();
}

void CMDActionSPList::update(const CMDActionSPList& srcList, const QString& objName)
{
    if(m_data)
        m_data->update(srcList, objName);
}

bool CMDActionSPList::copy(const CMDActionSPList* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

/*!
\section 序列化
*/
bool CMDActionSPList::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDActionSPList::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CMDActionSPList::serialize(QJsonArray& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDActionSPList::deserialize(const QJsonArray& json)
{
    if(m_data)
        m_data->deserialize(json);
}
