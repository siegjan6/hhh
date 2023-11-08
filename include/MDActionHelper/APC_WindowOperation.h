/*!

\author dGFuZ3p3

\section 功能

动作数据类：窗口操作。

*/

#ifndef APC_WINDOWOPERATION_H
#define APC_WINDOWOPERATION_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include "MDActionConstant.h"
#include "MDActionHelper_Global.h"
#include "MDActionProperty_Config.h"

class CAPCData_WindowOperation;
class MDACTIONHELPERSHARED_EXPORT CAPC_WindowOperation : public CMDActionProperty_Config
{
public:
    CAPC_WindowOperation();
    ~CAPC_WindowOperation();
   CAPC_WindowOperation(const CAPC_WindowOperation& obj);
   CAPC_WindowOperation& operator = (const CAPC_WindowOperation& obj);
/*!
\section 属性
*/
public:
    QString second() const;
    void setSecond(const QString& windowName);
public:
    int left() const;
    void setLeft(int left);
    int top() const;
    void setTop(int top);
public:
    int width() const;
    void setWidth(int width);
    int height() const;
    void setHeight(int height);
public:
    int pwid() const;
    void setPwid(int pwid);
    QRect geometry() const;
    void setGeometry(const QRect grect);
    QString meta() const;
    void setMeta(const QString& meta);
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
    CAPCData_WindowOperation* m_data1;
};

#endif // APC_WINDOWOPERATION_H
