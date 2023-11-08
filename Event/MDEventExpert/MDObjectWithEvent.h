/*!

\author dGFuZ3p3

\section 功能

关联了事件的对象及其容器类，用于根据对象名快速查找关联事件。

*/

#ifndef MDOBJECTWITHEVENT_H
#define MDOBJECTWITHEVENT_H

#include <QList>
#include <QMap>
#include <QString>
#include "../../include/MDCommonHelper/MDObject.h"
#include "MDEventProperty_Config.h"

/*!
 * \brief 关联了事件的对象类
 */
class CMDEvent;
class CMDRawEvent;
class CMDEventContacter;
class CMDObjectWithEvent : public CMDObject
{
public:
    CMDObjectWithEvent();
    CMDObjectWithEvent(const QString& name, MDObjectType objectType);
    virtual ~CMDObjectWithEvent();
public:
    void ProcessRawEvent(CMDRawEvent* rawEvent);
public:
    bool initAddEvent(CMDEvent* event);
protected:
    QList<CMDEvent*> m_eventList; //! 对象所关联的事件
};

/*!
 * \brief 对象容器类
 */
class CMDObjectContainer
{
public:
    CMDObjectContainer();
    virtual ~CMDObjectContainer();
public:
    CMDObject* addObj(const QString& name, MDObjectType objectType, qint8& result);
    CMDObject* findObj(const QString& name);
    bool deleteObj(const QString& name);
    void deleteAll();
public:
    virtual CMDObject*newObj(const QString& name, MDObjectType objectType, qint8& result);
protected:
    QMap<QString, CMDObject*> m_objectMap;
};

class CMDRawEvent;
/*!
 * \brief 关联了事件的对象容器类
 */
class CContainerForObjectWithEvent : public CMDObjectContainer
{
public:
    CContainerForObjectWithEvent();
public:
    virtual CMDObject*newObj(const QString& name, MDObjectType objectType, qint8& result);
public:
    bool ProcessRawEvent(CMDRawEvent* rawEvent);
    bool ProcessRawTimeEvent(CMDRawEvent* rawEvent);
};

#endif // MDOBJECTWITHEVENT_H
