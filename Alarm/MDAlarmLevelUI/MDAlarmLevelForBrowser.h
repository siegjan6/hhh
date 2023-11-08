#ifndef MDALARMLEVELFORBROWSER_H
#define MDALARMLEVELFORBROWSER_H

#include <QObject>
#include <QMap>
#include <QVariant>

#include "MDAlarmLevel.h"
class CMDAlarmLevel;
class CMDAlarmConfiger;
class CMDAlarmLevelTreeWindow;
class CMDAlarmLevelForBrowser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QMap translateName READ translateName)
    QMap<QString,QVariant> translateName(){return m_translateMap;}

    Q_PROPERTY(QString GUID_General)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString comment READ comment WRITE setComment)
//    Q_PROPERTY(qint32 delayLength READ delayLength WRITE setDelayLength)

    Q_PROPERTY(QString GUID_Color)
    Q_PROPERTY(QString DILalarmColor READ alarmColor WRITE setAlarmColor)

protected:
    QMap<QString,QVariant> m_translateMap;
public:
    CMDAlarmLevelForBrowser();
    void setMDObject(CMDAlarmLevel* MDObject);
    void setMDObjects(QList<CMDAlarmLevel*>& MDObjects);
    void setConfiger(CMDAlarmConfiger* configer);
protected:
    QList<CMDAlarmLevel*> m_MDObjects;
    CMDAlarmConfiger* m_configer;
public:
    void setObjTreeWindow(CMDAlarmLevelTreeWindow* objTreeWindow);
protected:
    CMDAlarmLevelTreeWindow* m_ObjTreeWindow;

/*!
\section 属性
*/
public:
    QString name() const;
    void setName(const QString& name);

    QString comment() const;
    void setComment(const QString& comment);

//    qint32 delayLength() const;
//    void setDelayLength(qint32 delayLength);
public:
    QString alarmColor() const;
    void setAlarmColor(const QString& alarmColor);
};

#endif // MDALARMLEVELFORBROWSER_H
