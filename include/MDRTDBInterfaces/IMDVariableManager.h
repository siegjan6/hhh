/*!
\author zyb 2015.01.08
\section 变量管理器 MDRTDBManager成员变量
*/
#ifndef IMDVARIABLEMANAGER_H
#define IMDVARIABLEMANAGER_H

#include <QString>
#include "IMDRTDBVariable.h"
#include "../../include/MDDriverInterfaces/IMDDriverManager.h"
class IMDRTDBManager;
class IMDVariableGroup;
//变量管理器
class IMDVariableManager
{
public:
    virtual ~IMDVariableManager(){}
//!关联变量
public:
    //!新建变量
    virtual IMDRTDBVariable*  createVariable(IMDVariableGroup* group=NULL)= 0;
    //!添加变量
    virtual bool addVariable(IMDRTDBVariable* nsVar,QString &error)= 0;
    //!移除变量
    virtual bool removeVariable(IMDRTDBVariable* nsVar)= 0;
    //!删除所有变量
    virtual void removeAllVariable()= 0;
    //!获取变量
    virtual IMDRTDBVariable* getVariableAt(int index)= 0;
    //!获取变量
    virtual IMDRTDBVariable *getVariable(const QString & varShortName)= 0;
    //!获取变量数量
    virtual int getVariableCount()= 0;
    //!检验变量有效性
    virtual void checkVariableValid(IMDDriverManager* nsDriverManager)= 0;
/*!
section 变量管理
*/
public:
    virtual bool readVariableValue(IMDVariable *var)=0;
    virtual QVariant readVariableValue(const QString &varName)=0;
    virtual bool writeVariableValue(const QString &varName,QVariant varValue)=0;    
/*!
section 变量安全策略
*/
public:
    virtual CMDActionSPList *variableActionSPList(const QString &varName)=0;

/*!
\section RTDBManager,获取DriverManager
*/
    virtual void setRTDBManager(IMDRTDBManager* rtdbManager)=0;
};

#endif // IMDVARIABLEMANAGER_H
