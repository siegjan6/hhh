/*!

\author dGFuZ3p3

\section 功能

事件数据基类，具体的事件由此类派生。

*/

#ifndef MDEVENTPROPERTY_CONFIG_H
#define MDEVENTPROPERTY_CONFIG_H

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QVariant>
#include "MDEventConstant.h"
#include "MDEventHelper.h"

#include "MDEventHelper_Global.h"

class CMDEventPropertyData_Config;
class MDEVENTHELPERSHARED_EXPORT CMDEventProperty_Config
{
public:
    CMDEventProperty_Config();
    CMDEventProperty_Config(MDEventType eventType);
     virtual ~CMDEventProperty_Config();
    CMDEventProperty_Config(const CMDEventProperty_Config& obj);
    CMDEventProperty_Config& operator = (const CMDEventProperty_Config& obj);
/*!
\section 属性
*/
public:
    static QString typeToString(MDEventType eventType);
public:
    QString name() const;
    void setName(const QString& name);

    QString comment() const;
    void setComment(const QString& comment);

    bool isEnable() const;
    void setEnable(bool flag);

    MDEventType type() const;
    void setType(MDEventType type);

    bool isEnableLog() const;
    void setEnableLog(bool enable);
public:
    QString objName() const;
    void setObjName(const QString& objName);
public:
    QString expressionString() const;
    void setExpressionString(const QString& expressionString);
public:
    const QStringList& actionNameList() const;
    void setActionNameList(const QStringList& actionNameList);
public:
    virtual bool copy(const CMDEventProperty_Config* source);

/*!
\section 序列化
*/
public:
    virtual bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    virtual void deserialize(QDataStream& stream);
public:
    virtual QString condition() const;
/*!
\section 序列化：JSON格式
*/
public:
   virtual void serialize(QJsonObject& json);
   virtual void deserialize(const QJsonObject& json);
protected:
    CMDEventPropertyData_Config* m_data;
};

#endif // MDEVENTPROPERTY_CONFIG_H
