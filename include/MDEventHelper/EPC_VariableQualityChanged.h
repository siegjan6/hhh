/*!

\author dGFuZ3p3

\section 功能

事件数据类：变量品质事件。

*/

#ifndef EPC_VARIABLEQUALITYCHANGED_H
#define EPC_VARIABLEQUALITYCHANGED_H

#include "MDEventConstant.h"
#include "MDEventHelper.h"
#include "MDEventProperty_Config.h"
#include "MDEventHelper_Global.h"

class CEPCData_VariableQualityChanged;
class MDEVENTHELPERSHARED_EXPORT CEPC_VariableQualityChanged : public CMDEventProperty_Config
{
public:
    CEPC_VariableQualityChanged();
    ~CEPC_VariableQualityChanged();
   CEPC_VariableQualityChanged(const CEPC_VariableQualityChanged& obj);
   CEPC_VariableQualityChanged& operator = (const CEPC_VariableQualityChanged& obj);
/*!
\section 属性
*/
public:
    CValueRangeCondition& rangeCondition() const;
    void setRangeCondition(const CValueRangeCondition& rangeCondition);
public:
    virtual bool copy(const CMDEventProperty_Config* source);

/*!
\section 序列化
*/
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void deserialize(QDataStream& stream);
public:
    virtual QString condition() const; //! 事件触发的条件
/*!
\section 序列化：JSON格式
*/
public:
   virtual void serialize(QJsonObject& json);
   virtual void deserialize(const QJsonObject& json);
protected:
    CEPCData_VariableQualityChanged* m_data1;
};


#endif // EPC_VARIABLEQUALITYCHANGED_H
