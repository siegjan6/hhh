#ifndef MDEVENTOBJECTSFORBROWSER_H
#define MDEVENTOBJECTSFORBROWSER_H

#include <QObject>

#include "EPC_Combined.h"
#include "EPC_TimeCycle.h"
#include "EPC_VariableQualityChanged.h"
#include "EPC_VariableValueChangedInRange.h"
#include "EPC_VariableValueWrite.h"
class CMDEventConfiger;
class CMDEventTreeWindow;

// 基本属性类
class CMDEventProperty_ConfigForBrowser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QMap translateName READ translateName)
    QMap<QString,QVariant> translateName(){return m_translateMap;}

    Q_PROPERTY(QString GUID_General)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString comment READ comment WRITE setComment)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(QString objName READ objName WRITE setObjName)
    Q_PROPERTY(bool isEnable READ isEnable WRITE setEnable)
    Q_PROPERTY(bool isEnableLog READ isEnableLog WRITE setEnableLog)
    Q_PROPERTY(QString expressionString READ expressionString WRITE setExpressionString)

    Q_PROPERTY(QString GUID_Action)
    Q_PROPERTY(QString DILactionNameList READ actionNameList WRITE setActionNameList)
protected:
    QMap<QString,QVariant> m_translateMap;
public:
    CMDEventProperty_ConfigForBrowser();
    void setMDObject(CMDEventProperty_Config* MDObject);
    void setMDObjects(QList<CMDEventProperty_Config*>& MDObjects);
    void setConfiger(CMDEventConfiger* configer);
    bool isSameType() const;
protected:
    QList<CMDEventProperty_Config*> m_MDObjects;
    CMDEventConfiger* m_configer;
public:
    void setObjTreeWindow(CMDEventTreeWindow* objTreeWindow);
protected:
    CMDEventTreeWindow* m_ObjTreeWindow;

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
    QString type() const;

    bool isEnableLog() const;
    void setEnableLog(bool enable);
public:
    QString objName() const;
    void setObjName(const QString& objName);
public:
    QString expressionString() const;
    void setExpressionString(const QString& expressionString);
public:
    QString actionNameList() const;
    void setActionNameList(const QString& actionNameList);
};

// 派生类：周期
class CEPC_TimeCycleForBrowser : public CMDEventProperty_ConfigForBrowser
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_TimeCycle)
    Q_PROPERTY(QString DILtimeCycle READ timeCycle WRITE setTimeCycle)
public:
    CEPC_TimeCycleForBrowser();
/*!
\section 属性
*/
public:
    QString timeCycle() const;
    void setTimeCycle(const QString& timeCycle);
};

// 派生类：变量品质变化
class CEPC_VariableQualityChangedForBrowser : public CMDEventProperty_ConfigForBrowser
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_QualityChanged)
    Q_PROPERTY(QString DILrangeCondition READ rangeCondition WRITE setRangeCondition)
public:
    CEPC_VariableQualityChangedForBrowser();
/*!
\section 属性
*/
public:
    QString rangeCondition() const;
    void setRangeCondition(const QString& rangeCondition);
};

// 派生类：变量数值变化范围
class CEPC_VariableValueChangedInRangeForBrowser : public CMDEventProperty_ConfigForBrowser
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_ValueChangedInRange)
    Q_PROPERTY(QString DILrangeCondition READ rangeCondition WRITE setRangeCondition)
public:
    CEPC_VariableValueChangedInRangeForBrowser();
/*!
\section 属性
*/
public:
    QString rangeCondition() const;
    void setRangeCondition(const QString& rangeCondition);
};

// 派生类：写变量
class CEPC_VariableValueWriteForBrowser : public CMDEventProperty_ConfigForBrowser
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_WriteVariable)
    Q_PROPERTY(quint8 checkConditionBeforeOrAfterWrite READ checkConditionBeforeOrAfterWrite WRITE setCheckConditionBeforeOrAfterWrite)
    Q_PROPERTY(QString DILrangeCondition READ rangeCondition WRITE setRangeCondition)
public:
    CEPC_VariableValueWriteForBrowser();

/*!
\section 属性
*/
public:
    quint8 checkConditionBeforeOrAfterWrite() const;
    void setCheckConditionBeforeOrAfterWrite(quint8 flag);
public:
    QString rangeCondition() const;
    void setRangeCondition(const QString& rangeCondition);
};

#endif // MDEVENTOBJECTSFORBROWSER_H
