/*!

\author dGFuZ3p3

\section 功能

事件数据类：组合事件。

*/

#ifndef EPC_COMBINED_H
#define EPC_COMBINED_H

#include "MDEventConstant.h"
#include "MDEventHelper.h"
#include "MDEventProperty_Config.h"
#include "MDEventHelper_Global.h"

class CEPCData_Combined;
class MDEVENTHELPERSHARED_EXPORT CEPC_Combined : public CMDEventProperty_Config
{
public:
    CEPC_Combined();
    ~CEPC_Combined();
   CEPC_Combined(const CEPC_Combined& obj);
   CEPC_Combined& operator = (const CEPC_Combined& obj);
/*!
\section 属性
*/
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
    CEPCData_Combined* m_data1;
};

#endif // EPC_COMBINED_H
