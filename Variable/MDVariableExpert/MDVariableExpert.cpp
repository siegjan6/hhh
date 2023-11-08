#include "../../include/MDVariableExpert/MDVariableExpert.h"
#include "MDVariableExpertImp.h"

CMDVariableExpert::CMDVariableExpert()
{
    m_imp = new CMDVariableExpertImp;
}

CMDVariableExpert::~CMDVariableExpert()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化与释放
*/
void CMDVariableExpert::setParaBeforeLoad(const QString& pid, IMDRTDBManager* rtdbMgr, CMDVariableContacter* contacter)
{
    if(m_imp)
        m_imp->setParaBeforeLoad(pid, rtdbMgr, contacter);
}

void CMDVariableExpert::load()
{
    if(m_imp)
        m_imp->load();
}

void CMDVariableExpert::unload()
{
    if(m_imp)
        m_imp->unload();
}

/*!
\section 访问 RTDB 的对象
*/
IMDRTDBManager* CMDVariableExpert::RTDBManager()
{
    return (m_imp ? m_imp->m_RTDBMgr : NULL);
}

IMDVariableArchivesManager *CMDVariableExpert::variableArchivesManager()
{
    IMDRTDBManager* rtdbMgr = RTDBManager();
    return (rtdbMgr ? rtdbMgr->variableArchivesManager() : NULL);
}

/*!
\section 从 RTDB 读写变量
*/
bool CMDVariableExpert::readVariableValue(IMDVariable *var)
{
    return (m_imp ? m_imp->readVariableValue(var) : false);
}

IMDRTDBVariable* CMDVariableExpert::getVariable(const QString& varName)
{
    return (m_imp ? m_imp->m_RTDBMgr->variableManager()->getVariable(varName) : NULL);
}

QVariant CMDVariableExpert::readVariableValue(const QString &varName)
{
    return (m_imp ? m_imp->readVariableValue(varName) : QVariant());
}

bool CMDVariableExpert::writeVariableValue(const QString &varName,QVariant varValue)
{
    return (m_imp ? m_imp->writeVariableValue(varName, varValue) : false);
}

bool CMDVariableExpert::writeVariableValueRelative(const QString &varName, QVariant varValueRelative)
{
    return (m_imp ? m_imp->writeVariableValueRelative(varName, varValueRelative) : false);
}

bool CMDVariableExpert::writeVariableValueSwitch01(const QString &varName)
{
    return (m_imp ? m_imp->writeVariableValueSwitch01(varName) : false);
}

bool CMDVariableExpert::writeVariableValueWithoutSig(const QString& varName, QVariant varValue)
{
    return (m_imp ? m_imp->writeVariableValue(varName, varValue, false) : false);
}

bool CMDVariableExpert::writeVariableValueRelativeWithoutSig(const QString &varName, QVariant varValueRelative)
{
    return (m_imp ? m_imp->writeVariableValueRelative(varName, varValueRelative, false) : false);
}

bool CMDVariableExpert::writeVariableValueSwitch01WithoutSig(const QString &varName)
{
    return (m_imp ? m_imp->writeVariableValueSwitch01(varName, false) : false);
}

bool CMDVariableExpert::writeVariables(QList<IMDVariable*>& nsVarList)
{
    return (m_imp ? m_imp->writeVariables(nsVarList) : false);
}

qint8 CMDVariableExpert::executeAction(CMDAction* action)
{
    return (m_imp ? m_imp->executeAction(action) : -1);
}
