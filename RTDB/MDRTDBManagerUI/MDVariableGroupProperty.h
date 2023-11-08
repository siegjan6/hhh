/*!
author enli 2015.01.08
\section 变了组管理类
*/
#ifndef CMDVARIABLEGROUPPROP_H
#define CMDVARIABLEGROUPPROP_H

#include <QString>
#include <QObject>
#include <QVariant>
#include <QMap>

class IMDVariableGroup;
class CMDRTDBVariablesWidget;
//变量组
class CMDVariableGroupProperty:public QObject
{
   Q_OBJECT
   Q_PROPERTY(QMap translateName READ translateName)
   Q_PROPERTY(QString GUID_General)
   Q_PROPERTY(QString Name READ localName WRITE setLocalName)
   Q_PROPERTY(QString Discription READ description WRITE setDescription)

public:
    CMDVariableGroupProperty();
    virtual ~CMDVariableGroupProperty();
/*!
\section 基本属性
*/
public:
    //!不带父Group的名
    QString localName();
    void setLocalName(QString name);
    //! 描述
    QString description();
    void setDescription(QString description);

    //!复制
    void copy(IMDVariableGroup *otherVarGroup);

    //!便于及时刷新修改的变量属性
    void setParentWidget(CMDRTDBVariablesWidget* parentWidget);
private:
    bool isSystemVariableGroup();
    //!属性框组名及属性名翻译
    QMap<QString,QVariant> translateName(){return m_translateMap;}
    //!弹出错误信息框
    void promptErrorMsg(const QString& errMsg);
private:

    QString m_localName;
    QString m_description;

    IMDVariableGroup *m_VariableGroup;
    CMDRTDBVariablesWidget* m_parentWidget;
    QMap<QString,QVariant>  m_translateMap;
};

#endif // CMDVARIABLEGROUPPROP_H
