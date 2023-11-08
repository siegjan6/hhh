/*!

\author dGFuZ3p3

\section 功能

权限组数据类。

*/

#ifndef MDPOWERGROUP_H
#define MDPOWERGROUP_H

#include <QString>
#include "MDSecurityPolicyHelper_Global.h"
#include <QJsonObject>

class CMDActionSPList;
class CMDPowerGroupData;
class MDSECURITYPOLICYHELPERSHARED_EXPORT CMDPowerGroup
{
public:
    CMDPowerGroup();
    virtual ~CMDPowerGroup();
   CMDPowerGroup(const CMDPowerGroup& obj);
   CMDPowerGroup& operator = (const CMDPowerGroup& obj);
/*!
\section 属性
*/
public:
    QString name() const;
    void setName(const QString& name);

    QString comment() const;
    void setComment(const QString& comment);

    int level() const;
    void setLevel(const int& level);

public:
    const QStringList& accountList() const;
    void setAccountList(const QStringList& accountList);
public:
    bool isEnable() const;
    void setEnable(bool enable);

    bool isVisible() const;
    void setVisible(bool visible);
public:
    //! 有权操作的动作列表
    CMDActionSPList* spList();
    bool setSPList(const CMDActionSPList* spList);
public:
    virtual bool copy(const CMDPowerGroup* source);

/*!
\section 序列化
*/
public:
    virtual bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    virtual void deserialize(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
protected:
    CMDPowerGroupData* m_data;
};

#endif // MDPOWERGROUP_H
