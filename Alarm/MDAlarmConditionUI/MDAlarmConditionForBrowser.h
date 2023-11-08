#ifndef MDALARMCONDITIONFORBROWSER_H
#define MDALARMCONDITIONFORBROWSER_H

#include <QObject>
#include <QMap>
#include <QVariant>

class CMDAlarmCondition;
class CMDActionSPList;
class CMDAlarmConditionConfiger;
class CMDAlarmConditionTreeWindow;
class CMDAlarmConditionForBrowser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QMap translateName READ translateName)
    QMap<QString,QVariant> translateName(){return m_translateMap;}

    Q_PROPERTY(QString GUID_General)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString comment READ comment WRITE setComment)
    Q_PROPERTY(QString levelName READ levelName WRITE setLevelName)

    Q_PROPERTY(QString GUID_Condition)
    Q_PROPERTY(QString DILcondition READ expression WRITE setExpression)

    Q_PROPERTY(QString GUID_SecurityPolicy)
    Q_PROPERTY(QString DILsp READ spList WRITE setSPList)
protected:
    QMap<QString,QVariant> m_translateMap;
public:
    CMDAlarmConditionForBrowser();
    void setMDObject(CMDAlarmCondition* MDObject);
    void setMDObjects(QList<CMDAlarmCondition*>& MDObjects);
    void setConfiger(CMDAlarmConditionConfiger* configer);
protected:
    QList<CMDAlarmCondition*> m_MDObjects;
    CMDAlarmConditionConfiger* m_configer;
public:
    void setObjTreeWindow(CMDAlarmConditionTreeWindow* objTreeWindow);
protected:
    CMDAlarmConditionTreeWindow* m_ObjTreeWindow;

/*!
\section 属性
*/
public:
   QString name() const;
   void setName(const QString& name);

   QString comment() const;
   void setComment(const QString& comment);

   QString levelName() const;
   void setLevelName(const QString& levelName);

   QString expression() const;
   void setExpression(const QString& expression);
public:
    QString spList();
    bool setSPList(const QString& spList);
};

#endif // MDALARMCONDITIONFORBROWSER_H
