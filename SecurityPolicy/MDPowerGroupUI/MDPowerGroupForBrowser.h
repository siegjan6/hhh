#ifndef MDPOWERGROUPFORBROWSER_H
#define MDPOWERGROUPFORBROWSER_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include <QVariant>

class CMDPowerGroup;
class CMDActionSPList;
class CMDPowerGroupConfiger;
class CMDPowerGroupTreeWindow;

class CMDPowerGroupForBrowser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QMap translateName READ translateName)
    QMap<QString,QVariant> translateName(){return m_translateMap;}

    Q_PROPERTY(QString GUID_General)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString comment READ comment WRITE setComment)
    Q_PROPERTY(bool isEnable READ isEnable WRITE setEnable)
    Q_PROPERTY(bool isVisible READ isVisible WRITE setVisible)

    Q_PROPERTY(QString GUID_Account)
    Q_PROPERTY(QString DILaccountList READ accountList WRITE setAccountList)

    Q_PROPERTY(QString GUID_SecurityPolicy)
    Q_PROPERTY(QString DILsp READ spList WRITE setSPList)
protected:
    QMap<QString,QVariant> m_translateMap;
public:
    CMDPowerGroupForBrowser();
    void setMDObject(CMDPowerGroup* MDObject);
    void setMDObjects(QList<CMDPowerGroup*>& MDObjects);
    void setConfiger(CMDPowerGroupConfiger* configer);
protected:
    QList<CMDPowerGroup*> m_MDObjects;
    CMDPowerGroupConfiger* m_configer;
public:
    void setObjTreeWindow(CMDPowerGroupTreeWindow* objTreeWindow);
protected:
    CMDPowerGroupTreeWindow* m_ObjTreeWindow;

/*!
\section 属性
*/
public:
    QString name() const;
    void setName(const QString& name);

    QString comment() const;
    void setComment(const QString& comment);
public:
    QString accountList() const;
    void setAccountList(const QString& accountList);
public:
    bool isEnable() const;
    void setEnable(bool enable);

    bool isVisible() const;
    void setVisible(bool visible);
public:
    QString spList();
    bool setSPList(const QString& spList);
};

#endif // MDPOWERGROUPFORBROWSER_H
