/*!

\author dGFuZ3p3

\section 功能

变量专家类，实现运行时的变量处理。

*/

#ifndef MDVARIABLEEXPERT_H
#define MDVARIABLEEXPERT_H

#include <QVariant>
#include "MDVariableExpert_Global.h"

class CMDVariableExpertImp;
class CMDVariableContacter;
class CMDAction;
class IMDVariable;
class IMDRTDBVariable;
class IMDRTDBManager;
class IMDVariableArchivesManager;
class MDVARIABLEEXPERTSHARED_EXPORT CMDVariableExpert
{
public:
    CMDVariableExpert();
    virtual ~CMDVariableExpert();
/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, IMDRTDBManager* rtdbMgr, CMDVariableContacter* contacter);
    void load();
    void unload();

/*!
\section 访问 RTDB 的对象
*/
public:
    IMDRTDBManager* RTDBManager();
    IMDVariableArchivesManager *variableArchivesManager();

/*!
\section 从 RTDB 读写变量
*/
public:
    bool readVariableValue(IMDVariable *var);
    IMDRTDBVariable* getVariable(const QString& varName);
    QVariant readVariableValue(const QString &varName);
    bool writeVariableValue(const QString &varName,QVariant varValue);
    bool writeVariableValueRelative(const QString &varName, QVariant varValueRelative);
    bool writeVariableValueSwitch01(const QString &varName);
    bool writeVariableValueWithoutSig(const QString& varName, QVariant varValue);
    bool writeVariableValueRelativeWithoutSig(const QString &varName, QVariant varValueRelative);
    bool writeVariableValueSwitch01WithoutSig(const QString &varName);
    bool writeVariables(QList<IMDVariable*>& nsVarList);
public:
    qint8 executeAction(CMDAction* action); //! 执行写变量动作

protected:
    CMDVariableExpertImp* m_imp;
};

#endif // MDVARIABLEEXPERT_H
