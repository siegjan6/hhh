/*!

\author dGFuZ3p3

\section 功能

CMDActionSP - 安全策略数据类 ：将动作与相关的安全配置绑定，作为一个整体处理；
CMDActionSPList - 安全策略链表数据类 ：管理多个 CMDActionSP 对象，方便代码处理。

*/

#ifndef MDACTIONSP_H
#define MDACTIONSP_H

#include <QString>
#include "MDSecurityPolicyConstant.h"
#include "../../include/MDActionIDHelper/MDActionID.h"
#include "MDSecurityPolicyHelper_Global.h"

class CMDActionSPData;
/*!
 * \brief 安全策略数据类
 */
class MDSECURITYPOLICYHELPERSHARED_EXPORT CMDActionSP
{
public:
    CMDActionSP();
    CMDActionSP(MDActionType type);
    virtual ~CMDActionSP();
   CMDActionSP(const CMDActionSP& obj);
   CMDActionSP& operator = (const CMDActionSP& obj);
/*!
\section 辅助函数
*/
public:
   static void enumProtectModes(QStringList& modeList);
   static QString modeToString(MDActionProtectMode mode);
   static MDActionProtectMode stringToMode(const QString& str);
public:
   static void enumSigTypes(QStringList& typeList);
   static QString sigTypeToString(MDSigType type);
   static MDSigType stringToSigType(const QString& str);

/*!
\section 属性
*/
public:
   //! 动作ID
    CMDActionID& actionID() const;
    void setActionID(const CMDActionID& actionID);
    QString actionIDObjName() const;
public:
    //! 保护模式
    MDActionProtectMode protectMode() const;
    void setProtectMode(MDActionProtectMode mode);
public:
    //! 是否启用，内部通过检查保护模式来判断，代码如下：
    //! MDActionProtectMode::null_mode != protectMode()
    bool isEnable() const;

    //! 签名类型
    MDSigType sigType() const;
    void setSigType(MDSigType type);

    //! 运行时临时不需要签名
    bool needSignature() const;
    void setNeedSignature(bool flag);
public:
    //! 是否将验证结果记录到日志，仅【 MindSCADA签名保护 】 模式下可配置，
    //! 【 电子签名 】的结果被强制记录到日志
    bool isEnableLog() const;
    void setEnableLog(bool enable);
public:
    //! 具有授权的操作者组
    const QStringList& operatorGroupList() const;
    void setOperatorGroupList(const QStringList& groupList);

    //! 具有授权的校验者组
    const QStringList& verifierGroupList() const;
    void setVerifierGroupList(const QStringList& groupList);
public:
    virtual bool copy(const CMDActionSP* source);

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
    virtual void serialize(QJsonObject& json);
    virtual void deserialize(const QJsonObject& json);

protected:
    CMDActionSPData* m_data;
};

class CMDActionSPListData;
/*!
 * \brief 安全策略链表数据类
 */
class MDSECURITYPOLICYHELPERSHARED_EXPORT CMDActionSPList
{
public:
    CMDActionSPList();
    virtual ~CMDActionSPList();
   CMDActionSPList(const CMDActionSPList& obj);
   CMDActionSPList& operator = (const CMDActionSPList& obj);
/*!
\section CMDActionSP 对象管理
*/
public:
   /*!
    追加一个 CMDActionSP 对象到链表尾部。

    \param newSP 追加的 CMDActionSP 对象，必须是通过 new 创建的。
    \returns 成功返回 true ，否则 false 。
    */
    bool append(CMDActionSP* newSP); // 必须是 new CMDActionSP
    CMDActionSP* append(MDActionType type);
public:
    int count() const;
    void deleteAll();
    CMDActionSP* at(int index) const;
    CMDActionSP* find(MDActionType type) const;
public:
    void enumEnabled(QList<CMDActionSP*>& spList) const;
    void deleteAllDisabled();
public:
    /*!
     更新管理的 CMDActionSP 对象信息。

     \param srcList 源对象链表，目标链表中动作类型相同的对象将被更新。
     \param objName 目标对象名称，目标链表中动作的目标对象名将被更新为该值。
     \returns 无 。
     \code
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
     \endcode
     */
    void update(const CMDActionSPList& srcList, const QString& objName);
public:
    virtual bool copy(const CMDActionSPList* source);

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
    virtual void serialize(QJsonArray& json);
    virtual void deserialize(const QJsonArray& json);

protected:
    CMDActionSPListData* m_data;
};

#endif // MDACTIONSP_H

