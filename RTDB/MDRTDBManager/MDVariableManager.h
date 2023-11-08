/*!
\author enli 2015.01.08
\section 变量管理类，CMDRTDBManager+CMDVariableGroup成员
*/
#ifndef CMDVARIABLEMANAGER_H
#define CMDVARIABLEMANAGER_H

#include <QMap>
#include <QList>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QDomElement>
#include "IMDRTDBVariable.h"
#include "IMDVariableManager.h"
#include "IMDVariableGroup.h"
#include "IMDDriverManager.h"
#include "MDRTDBManager.h"
#include "SortJsonObject.h"
class CMDVariableManager:public IMDVariableManager
{
public:
    CMDVariableManager();
    virtual ~CMDVariableManager();
/*!
\section 变量管理
*/
public:
    //!新建变量
    IMDRTDBVariable*  createVariable(IMDVariableGroup* group = NULL);
    //!添加变量
    bool addVariable(IMDRTDBVariable* nsVar,QString &error);
    //!移除变量
    bool removeVariable(IMDRTDBVariable* nsVar);
    //!删除所有变量
    void removeAllVariable();
    //!获取变量
    IMDRTDBVariable* getVariableAt(int index);
    //!获取变量
    IMDRTDBVariable *getVariable(const QString & varName);
    //!获取变量数量
    int getVariableCount();
    //!检验变量有效性
    void checkVariableValid(IMDDriverManager* nsDriverManager);
    //!检验单个变量有效性
    void checkVariableValid(IMDDriverManager* nsDriverManager,IMDRTDBVariable* nsVariable);
private:
    QMap<QString,IMDRTDBVariable*> m_variableMap;//变量Map
    QMutex m_variableMapMutex;
/*!
\section 变量索引模式
*/
public:
   void enableVarKeyIsLocalName(bool enable = true);
private:
    bool m_varKeyIsLocalName;

/*!
\section 变量读写
*/
public:
    bool readVariableValue(IMDVariable *var);
    QVariant readVariableValue(const QString &varName);
    bool writeVariableValue(const QString &varName,QVariant varValue);
/*!
\section 变量读写配置
*/
public:
    void saveConfiguration(QDomDocument &doc,QDomElement &parentElement);
    void readConfiguration(CMDVariableManager *nsRTDBVariableManager,IMDDriverManager *nsDriverManager,IMDVariableGroup *parentVariableGroup,QDomElement &domElement);
    void save(QDataStream &out);
    void read(CMDVariableManager *nsRTDBVariableManager,IMDDriverManager *nsDriverManager,IMDVariableGroup *parentVariableGroup,QDataStream &in);
    void serialize(CSortJsonObject &json);
    void deserialize(const QJsonObject &json,CMDVariableManager *nsRTDBVariableManager,IMDDriverManager *nsDriverManager,IMDVariableGroup *parentVariableGroup);
    /*!
\section 变量安全策略
*/
public:
    CMDActionSPList *variableActionSPList(const QString &varName);

/*!
\section RTDBManager,获取DriverManager
*/
    void setRTDBManager(IMDRTDBManager* rtdbManager);
private:
    CMDRTDBManager* m_rtdbManager;
};

#endif // CMDVARIABLEMANAGER_H
