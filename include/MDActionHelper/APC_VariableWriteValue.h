/*!

\author dGFuZ3p3

\section 功能

动作数据类：写变量。

*/

#ifndef APC_VARIABLEWRITEVALUE_H
#define APC_VARIABLEWRITEVALUE_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include "MDActionConstant.h"
#include "MDActionHelper_Global.h"
#include "MDActionProperty_Config.h"

class CAPCData_VariableWriteValue;
class MDACTIONHELPERSHARED_EXPORT CAPC_VariableWriteValue : public CMDActionProperty_Config
{
public:
    CAPC_VariableWriteValue();
    ~CAPC_VariableWriteValue();
   CAPC_VariableWriteValue(const CAPC_VariableWriteValue& obj);
   CAPC_VariableWriteValue& operator = (const CAPC_VariableWriteValue& obj);
/*!
\section 属性
*/
public:
   static void enumMode(QStringList& modeList);
   static QString modeToString(MDWriteMode mode);
   static MDWriteMode stringToMode(const QString& str);
public:
    MDWriteMode mode() const;
    void setMode(MDWriteMode mode);

    QVariant value() const;
    void setValue(const QVariant& value);
public:
    qint8 valueSource() const;
    void setValueSource(qint8 valueSource);

    QString sourceName() const;
    void setSourceName(const QString& sourceName);
public:
    virtual bool copy(const CMDActionProperty_Config* source);

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
    CAPCData_VariableWriteValue* m_data1;
};

#endif // APC_VARIABLEWRITEVALUE_H
