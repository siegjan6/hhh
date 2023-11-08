/*!
\author Y2hhaWhq
\section 日志控件属性栏
*/
#ifndef PROPERTYDRAWLOG_H
#define PROPERTYDRAWLOG_H

#include "MDPropertyDrawObj.h"

class CMDDrawLog;

class CPropertyDrawLog : public CMDPropertyDrawObj
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(QString DILconfig READ config WRITE setConfig)

/*!
\section  属性配置项
*/
private:
    void setConfig(const QString& value);
    QString config();

public:
    CPropertyDrawLog(CMDDrawLog* parent);
    virtual ~CPropertyDrawLog(){}
protected:
    CMDDrawLog* object();
};

#endif // PROPERTYDRAWLOG_H

