/*!

\author dGFuZ3p3

\section 功能

变量专家类的实现类，实现运行时的变量处理。

*/

#ifndef MDVARIABLEEXPERTIMP_H
#define MDVARIABLEEXPERTIMP_H

#include <QtGlobal>
#include <QVariant>
#include "../../include/MDRTDBInterfaces/IMDVariableChangeNotifier.h"
#include "../../include/MDRTDBManager/MDRTDBManagerFactory.h"
#include "CommandEngineForCheckVariableSP.h"
#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"

class CMDVariableExpertImp;
/*!
 * \brief RTDB 的变量变化通知类
 */
class CMDVariableChangeNotifier : public IMDVariableChangeNotifier
{
public:
    CMDVariableChangeNotifier(CMDVariableExpertImp* variableExpert);
protected:
    CMDVariableExpertImp* m_variableExpert ;// NULL;
    virtual void onChange(IMDChangeVariable *changedVar);
    virtual void  onDeviceVariablesChange(const QList<QByteArray>& jsonMsgList);
    virtual void  onDeviceVariablesBatchNotify(const QList<QByteArray>& redisCmdList);
    virtual void  onVariablesBatchNotify(const QStringList& varNameList, const QList<QVariant>& varValueList);
};

class CMDAction;
class CMDVariableContacter;
class CMDActionSP;
/*!
 * \brief 变量专家类的实现类，实现运行时的变量处理。
 */
class CMDVariableExpertImp
{
public:
    CMDVariableExpertImp();
    ~CMDVariableExpertImp();

/*!
\section 初始化与释放
*/
public:
    void setParaBeforeLoad(const QString& pid, IMDRTDBManager* rtdbMgr, CMDVariableContacter* contacter);
    void load();
    void unload();
public:
    void start();
    void stop();
public:
    CMDVariableContacter* m_contacter ;// NULL;
    IMDRTDBManager* m_RTDBMgr ;// NULL;
    QString m_pid;

/*!
\section 从 RTDB 读写变量
*/
public:
    bool readVariableValue(IMDVariable *var);
    QVariant readVariableValue(const QString &varName);
    bool writeVariableValue(const QString &varName,QVariant varValue, bool needSignature = true);
    bool writeVariableValueRelative(const QString &varName, QVariant varValueRelative, bool needSignature = true);
    bool writeVariableValueSwitch01(const QString &varName, bool needSignature = true);
    bool writeVariables(QList<IMDVariable*>& nsVarList);
    bool writeVariableValue_AfterCheckSP(const QString &varName, QVariant varValue, CMDActionSP* sp);
protected:
    CMDActionSP* preCheckWriteSP(const QString &varName);
    CCommandEngineForCheckVariableSP m_checkVariableSPEngine;
public:
    qint8 executeAction(CMDAction* action);

/*!
\section 处理变量变化通知
*/
public:
    CMDVariableChangeNotifier* m_vcNotifier ;// NULL;   //! RTDB 的通知类
public:
    void onVariableChanged(IMDChangeVariable *changedVar);
    void  onDeviceVariablesChange(const QList<QByteArray>& jsonMsgList);
    void  onDeviceVariablesBatchNotify(const QList<QByteArray>& redisCmdList);
    void  onVariablesBatchNotify(const QStringList& varNameList, const QList<QVariant>& varValueList);
protected:
    void createSwitchs(bool create);
    CMDLogSwitch* m_lsOnVariableChanged;
    CMDLogSwitch* m_lsWriteVariable;
};

#endif // MDVARIABLEEXPERTIMP_H
