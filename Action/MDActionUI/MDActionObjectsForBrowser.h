#ifndef MDACTIONOBJECTSFORBROWSER_H
#define MDACTIONOBJECTSFORBROWSER_H

#include <QObject>

#include "APC_VariableWriteValue.h"
#include "APC_WindowOperation.h"
#include "APC_PlayTTS.h"
class CAPC_VariableWriteValue;
class CMDActionConfiger;
class CMDActionSPList;
class CMDActionTreeWindow;

// 基本属性类
class CMDActionProperty_ConfigForBrowser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QMap translateName READ translateName)
    QMap<QString,QVariant> translateName(){return m_translateMap;}

    Q_PROPERTY(QString GUID_General)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString comment READ comment WRITE setComment)
    Q_PROPERTY(QString actionType READ actionType)
    Q_PROPERTY(QString objName READ objName WRITE setObjName)
    Q_PROPERTY(bool isEnable READ isEnable WRITE setEnable)
    Q_PROPERTY(bool isEnableLog READ isEnableLog WRITE setEnableLog)
    Q_PROPERTY(bool isEnableDelay READ isEnableDelay WRITE setEnableDelay)
    Q_PROPERTY(qint32 delayLength READ delayLength WRITE setDelayLength)

    Q_PROPERTY(QString GUID_SecurityPolicy)
    Q_PROPERTY(QString DILsp READ spList WRITE setSPList)
protected:
    QMap<QString,QVariant> m_translateMap;
public:
    CMDActionProperty_ConfigForBrowser();
    void setMDObject(CMDActionProperty_Config* MDObject);
    void setMDObjects(QList<CMDActionProperty_Config*>& MDObjects);
    void setConfiger(CMDActionConfiger* configer);
    bool isSameType() const;
protected:
    QList<CMDActionProperty_Config*> m_MDObjects;
    CMDActionConfiger* m_configer;
public:
    void setObjTreeWindow(CMDActionTreeWindow* objTreeWindow);
protected:
    CMDActionTreeWindow* m_ObjTreeWindow;

/*!
\section 属性
*/
public:
    QString name() const;
    void setName(const QString& name);

    QString comment() const;
    void setComment(const QString& comment);

    bool isEnable() const;
    void setEnable(bool enable);
public:
    QString actionType() const;

    QString objName() const;
    void setObjName(const QString& name);

    bool isEnableLog() const;
    void setEnableLog(bool enable);
public:
    bool isEnableDelay() const;
    void setEnableDelay(bool enable);

    qint32 delayLength() const;
    void setDelayLength(qint32 delayLength);
public:
    QString spList();
    bool setSPList(const QString& spList);
};

// 派生类：写变量

class CAPC_VariableWriteValueForBrowser : public CMDActionProperty_ConfigForBrowser
{
    Q_OBJECT
    Q_ENUMS(MDWriteMode1)         //! 声明枚举

    Q_PROPERTY(QString GUID_WriteVariable)
    Q_PROPERTY(MDWriteMode1 mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QString value READ value WRITE setValue)
//    Q_PROPERTY(qint8 valueSource READ valueSource WRITE setValueSource)
//    Q_PROPERTY(QString sourceName READ sourceName WRITE setSourceName)
public:
    CAPC_VariableWriteValueForBrowser();

    enum class MDWriteMode1
    {
//        null_mode  = 0,
        mode_fixed  = 1,
        mode_relative    = 2,
        mode_switch_0_1 = 3
    };
/*!
\section 属性
*/
public:
    MDWriteMode1 mode() const;
    void setMode(MDWriteMode1 mode);

    QString value() const;
    void setValue(const QString& value);
Q_SIGNALS:
    void modeChanged(MDWriteMode1);
};

// 派生类：窗口动作
class CAPC_WindowOperationForBrowser : public CMDActionProperty_ConfigForBrowser
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_WindowOperation)
    Q_PROPERTY(QString second READ second WRITE setSecond)
    Q_PROPERTY(int left READ left WRITE setLeft)
    Q_PROPERTY(int top READ top WRITE setTop)
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
public:
    CAPC_WindowOperationForBrowser();

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
};

#endif // MDACTIONOBJECTSFORBROWSER_H
