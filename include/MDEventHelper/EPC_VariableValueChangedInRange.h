/*!

\author dGFuZ3p3

\section 功能

事件数据类：变量数值事件。

*/

#ifndef EPC_VARIABLEVALUECHANGEDINRANGE_H
#define EPC_VARIABLEVALUECHANGEDINRANGE_H

#include "MDEventConstant.h"
#include "MDEventHelper.h"
#include "MDEventProperty_Config.h"
#include "MDEventHelper_Global.h"

class CEPCData_VariableValueChangedInRange;
class MDEVENTHELPERSHARED_EXPORT CEPC_VariableValueChangedInRange : public CMDEventProperty_Config
{
public:
    CEPC_VariableValueChangedInRange();
    ~CEPC_VariableValueChangedInRange();
   CEPC_VariableValueChangedInRange(const CEPC_VariableValueChangedInRange& obj);
   CEPC_VariableValueChangedInRange& operator = (const CEPC_VariableValueChangedInRange& obj);
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
    CEPCData_VariableValueChangedInRange* m_data1;
};

#endif // EPC_VARIABLEVALUECHANGEDINRANGE_H
