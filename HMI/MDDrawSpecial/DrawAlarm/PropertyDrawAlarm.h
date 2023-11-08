/*!
\author Y2hhaWhq
\section 报警控件属性栏
*/
#ifndef PROPERTYDRAWALARM_H
#define PROPERTYDRAWALARM_H

#include "MDPropertyDrawObj.h"

class CMDDrawAlarm;

class CPropertyDrawAlarm : public CMDPropertyDrawObj
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
    CPropertyDrawAlarm(CMDDrawAlarm* parent);
    virtual ~CPropertyDrawAlarm(){}
protected:
    CMDDrawAlarm* object();
};

#endif // PROPERTYDRAWALARM_H

