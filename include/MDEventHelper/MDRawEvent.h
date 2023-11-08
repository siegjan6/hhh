/*!

\author dGFuZ3p3

\section 功能

原始事件数据类及其派生类，原始事件是 MindSCADA 系统中最底层的事件。

*/

#ifndef MDRAWEVENT_H
#define MDRAWEVENT_H

#include <QDateTime>
#include <QString>
#include "MDEventConstant.h"
#include "MDEventHelper_Global.h"
#include "../../include/MDRTDBInterfaces/IMDChangeVariable.h"

class CMDRawEventData;
/*!
 * \brief 原始事件数据类
 */
class CMDLogSwitch;
class MDEVENTHELPERSHARED_EXPORT CMDRawEvent
{
public:
    CMDRawEvent();
    CMDRawEvent(const QString& rawObjName, MDEventType eventType);
    virtual ~CMDRawEvent();
    CMDRawEvent(const CMDRawEvent& obj);
    CMDRawEvent& operator = (const CMDRawEvent& obj);

public:
    QString rawObjName() const;
    void setRawObjName(const QString& name);

    MDEventType eventType() const;
    void setEventType(const MDEventType& eventType);

    QDateTime timeStamp() const;
    void setTimeStamp(const QDateTime& dateTime);

    QString comment() const;
    void setComment(const QString& comment);
public:
    CMDLogSwitch* logSwitch() const;
    void setLogSwitch(CMDLogSwitch* logSwitch);
protected:
    CMDRawEventData* m_data;
};

class CMDRawVariableEventData;
/*!
 * \brief 原始变量事件数据类
 */
class MDEVENTHELPERSHARED_EXPORT CMDRawVariableEvent : public CMDRawEvent
{
public:
    CMDRawVariableEvent();
    CMDRawVariableEvent(MDEventType eventType, IMDChangeVariable* changedVariable);
    virtual ~CMDRawVariableEvent();
    CMDRawVariableEvent(const CMDRawVariableEvent& obj);
    CMDRawVariableEvent& operator = (const CMDRawVariableEvent& obj);
public:
    IMDChangeVariable* changedVariable() const;
    void setChangedVariable(IMDChangeVariable* changedVariable);
protected:
    CMDRawVariableEventData* m_data1;
};

#endif // MDRAWEVENT_H
