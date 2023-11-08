#ifndef MDACTIOMDPLISTDATA_H
#define MDACTIOMDPLISTDATA_H

#include <QList>
#include <QDataStream>
#include "../../include/MDActionHelper/MDActionConstant.h"
#include "MDActionSP.h"

class CMDActionSP;
class CMDActionSPListData
{
    friend class CMDActionSPList;
public:
    CMDActionSPListData();
    virtual ~CMDActionSPListData();
   CMDActionSPListData(const CMDActionSPListData& obj);
   CMDActionSPListData& operator = (const CMDActionSPListData& obj);
protected:
   QList<CMDActionSP*> m_spList;
   void release();
    CMDActionSP* find(MDActionType type);
    void enumEnabled(QList<CMDActionSP*>& spList);
    void deleteAllDisabled();
protected:
    void update(const CMDActionSPList& srcList, const QString& objName);
public:
    bool copy(const CMDActionSPListData* source);
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion);
public:
    void deserialize(QDataStream& stream);
    void deserializeOnVersion_1(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonArray& json);
    void deserialize(const QJsonArray& json);
public:
    quint8 m_version;
};

#endif // MDACTIOMDPLISTDATA_H
