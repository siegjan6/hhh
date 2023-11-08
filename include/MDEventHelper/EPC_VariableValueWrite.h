﻿/*!

\author dGFuZ3p3

\section 功能

事件数据类：写变量事件。

*/

#ifndef EPC_VARIABLEVALUEWRITE_H
#define EPC_VARIABLEVALUEWRITE_H

#include "MDEventConstant.h"
#include "MDEventHelper.h"
#include "MDEventProperty_Config.h"
#include "MDEventHelper_Global.h"

class CEPCData_VariableValueWrite;
class MDEVENTHELPERSHARED_EXPORT CEPC_VariableValueWrite : public CMDEventProperty_Config
{
public:
    CEPC_VariableValueWrite();
    ~CEPC_VariableValueWrite();
   CEPC_VariableValueWrite(const CEPC_VariableValueWrite& obj);
   CEPC_VariableValueWrite& operator = (const CEPC_VariableValueWrite& obj);
/*!
\section 属性
*/
public:
    quint8 checkConditionBeforeOrAfterWrite() const;
    void setCheckConditionBeforeOrAfterWrite(quint8 flag);
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
    CEPCData_VariableValueWrite* m_data1;
};

#endif // EPC_VARIABLEVALUEWRITE_H
