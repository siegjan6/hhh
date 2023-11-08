#ifndef MDACCOUNTFORBROWSER_H
#define MDACCOUNTFORBROWSER_H

#include <QObject>
#include <QMap>
#include <QVariant>

class CMDAccountProperty_Config;
class CMDActionSPList;
class CMDSecurityPolicyConfiger;
class CMDAccountTreeWindow;

class CMDAccountForBrowser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QMap translateName READ translateName)
    QMap<QString,QVariant> translateName(){return m_translateMap;}

    Q_PROPERTY(QString GUID_General)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString comment READ comment WRITE setComment)
    Q_PROPERTY(bool isEnable READ isEnable WRITE setEnable)
    Q_PROPERTY(bool isLocked READ isLocked WRITE setLocked)
    Q_PROPERTY(bool isVisible READ isVisible WRITE setVisible)

    Q_PROPERTY(QString GUID_Password)
    Q_PROPERTY(QString DILpassword READ password WRITE setPassword)

    Q_PROPERTY(QString GUID_SecurityPolicy)
    Q_PROPERTY(QString DILsp READ spList WRITE setSPList)
protected:
    QMap<QString,QVariant> m_translateMap;
public:
    CMDAccountForBrowser();
    void setMDObject(CMDAccountProperty_Config* MDObject);
    void setMDObjects(QList<CMDAccountProperty_Config*>& MDObjects);
    void setConfiger(CMDSecurityPolicyConfiger* configer);
protected:
    QList<CMDAccountProperty_Config*> m_MDObjects;
    CMDSecurityPolicyConfiger* m_configer;
public:
    void setObjTreeWindow(CMDAccountTreeWindow* objTreeWindow);
protected:
    CMDAccountTreeWindow* m_ObjTreeWindow;

/*!
\section 属性
*/
public:
    QString name() const;
    void setName(const QString& name);

    QString password() const;
    void setPassword(const QString& password);

    QString comment() const;
    void setComment(const QString& comment);
public:
    bool isEnable() const;
    void setEnable(bool enable);

    bool isLocked() const;
    void setLocked(bool locked);

    bool isVisible() const;
    void setVisible(bool visible);
public:
    QString spList();
    bool setSPList(const QString& spList);
};

#endif // MDACCOUNTFORBROWSER_H
