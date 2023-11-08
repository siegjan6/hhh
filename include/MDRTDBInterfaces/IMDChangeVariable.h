/*!
\author zyb 2015.01.08
\section 变化变量类，发送给tang时new创建
*/
#ifndef IMDCHANGEVARIABLE_H
#define IMDCHANGEVARIABLE_H

#include "IMDVariable.h"
 //变量变化的类型
enum class VariableChangeType
{
    ValueChanged = 0,
    QualityChanged = 1,
    BothChanged = 2,
};
class  IMDChangeVariable:public IMDVariable
{
public:
    virtual ~IMDChangeVariable(){}
public:
    //!变化通知类型
    virtual void  setVariableChangeType(VariableChangeType varChangeType)=0;
    virtual VariableChangeType  variableChangeType()=0;
    //!复制
    virtual void copy(IMDChangeVariable *otherChangeVariable)=0;
};
#endif // IMDCHANGEVARIABLE_H
