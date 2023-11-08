#include "MDVariableExpertImp.h"
#include "../../include/MDActionHelper/MDAction.h"
#include "../../include/MDVariableExpert/MDVariableContacter.h"
#include "../../include/MDRTDBManager/MDRTDBManagerFactory.h"
#include "MDActionSPSession.h"

/*!
 * \brief 变量专家类的实现类，实现运行时的变量处理。
 */
CMDVariableExpertImp::CMDVariableExpertImp()
{
    m_checkVariableSPEngine.m_expert = this;

    m_contacter = NULL;
    m_RTDBMgr = NULL;
    m_vcNotifier = NULL;   //! RTDB 的通知类

    m_pid = "";
    m_lsOnVariableChanged = NULL;
    m_lsWriteVariable = NULL;
}

CMDVariableExpertImp::~CMDVariableExpertImp()
{
}

/*!
\section 初始化与释放
*/
void CMDVariableExpertImp::setParaBeforeLoad(const QString& pid, IMDRTDBManager* rtdbMgr, CMDVariableContacter* contacter)
{
    m_pid = pid;
    m_contacter = contacter;
    m_RTDBMgr = rtdbMgr;
}

void CMDVariableExpertImp::load()
{
    createSwitchs(true);

    start();
}

void CMDVariableExpertImp::unload()
{
    stop();

    m_RTDBMgr = NULL;

    createSwitchs(false);
}

void CMDVariableExpertImp::start()
{
    CMDLogSwitch* lsProjectLoad = CMDLogSwitchFactory::findByGroup(m_pid, "ProjectExpert.Load_and_Unload");

    m_checkVariableSPEngine.startInCurrentThread();

    if(!m_vcNotifier)
        m_vcNotifier = new CMDVariableChangeNotifier(this);

    lsProjectLoad->LOG_INFO("启动实时数据库 m_RTDBMgr->start()");
    if(m_RTDBMgr)
    {
        m_RTDBMgr->setRuntimeMode(true);
        m_RTDBMgr->setVariableChangeNotifier(m_vcNotifier);
        m_RTDBMgr->start();
//        m_RTDBMgr->startSimulatior();
    }
}

void CMDVariableExpertImp::stop()
{
    CMDLogSwitch* lsProjectLoad = CMDLogSwitchFactory::findByGroup(m_pid, "ProjectExpert.Load_and_Unload");

    m_checkVariableSPEngine.stop();

    lsProjectLoad->LOG_INFO("停止实时数据库 m_RTDBMgr->unLoadProject()");
    if(m_RTDBMgr)
    {
        m_RTDBMgr->unLoadProject();
        m_RTDBMgr->setVariableChangeNotifier(NULL);
    }

    if(m_vcNotifier)
    {
        delete m_vcNotifier;
        m_vcNotifier = NULL;
    }
}

void CMDVariableExpertImp::createSwitchs(bool create)
{
    if(create)
    {
        QString subFileDir = "ProjectExpert";
        m_lsOnVariableChanged = CMDLogSwitchFactory::createSwitchWithSubFileDir(m_pid, "VariableExpert.OnVariableChanged", "变量变化", subFileDir);
        m_lsOnVariableChanged->turnOn();
        m_lsWriteVariable = CMDLogSwitchFactory::createSwitchWithSubFileDir(m_pid, "VariableExpert.WriteVariable", "写变量", subFileDir);
        m_lsWriteVariable->turnOn();
    }
    else
    {
        m_lsOnVariableChanged = NULL;
        m_lsWriteVariable = NULL;
    }
}

/*!
\section 从 RTDB 读写变量
*/
bool CMDVariableExpertImp::readVariableValue(IMDVariable *var)
{
    return (m_RTDBMgr ? m_RTDBMgr->readVariableValue(var) : false);
}

QVariant CMDVariableExpertImp::readVariableValue(const QString &varName)
{
    return (m_RTDBMgr ? m_RTDBMgr->readVariableValue(varName) : QVariant());
}

bool CMDVariableExpertImp::writeVariableValue(const QString &varName, QVariant varValue, bool needSignature/* = true*/)
{
    CMDActionSP* sp = preCheckWriteSP(varName);
    // 未启用签名略过验证直接写变量
    if(sp && sp->isEnable())
    {
        if(m_lsWriteVariable->isOn())
        {
            QString varInfo = QString("{%1:%2}").arg(varName).arg(varValue.toString());
            m_lsWriteVariable->LOG_INFO_IS("写变量配置了安全策略，提交到等待队列: " + varInfo);
        }

        CMDActionID& aid = sp->actionID();
        QString desc = "";
        QString unit = "";
        IMDRTDBVariable* var = m_RTDBMgr ? m_RTDBMgr->variableManager()->getVariable(varName) : NULL;
        if(var)
        {
            desc = var->getDescription();
            unit = var->getUnit().trimmed();
        }
        aid.setObjName(QString("%1%2[%3]").arg(desc).arg(unit.isEmpty() ? "" : "("+unit+")").arg(varName));
        aid.setDestState(varValue);
        if(readVariableValue(var))
        {
            aid.setSrcState(var->currentValue());
            if(var->currentValue().toString() == varValue.toString())
            {
                // 变量值相同直接返回
                return true;
            }
        }

        sp->setNeedSignature(needSignature);
        m_checkVariableSPEngine.submitWriteVarWithSP(varName, varValue, sp);
        return false;
    }

//    QString dynaVarName = varValue.toString();
//    IMDVariable *var = CMDRTDBManagerFactory::createVariable();
//    var->setName(dynaVarName);
//    if(m_RTDBMgr->readVariableValue(var))
//    {
//        varValue = var->currentValue();
//    }

    if(m_lsWriteVariable->isOn())
    {
        QString varInfo = QString("{%1:%2}").arg(varName).arg(varValue.toString());
        m_lsWriteVariable->LOG_INFO_IS("写变量 m_RTDBMgr->writeVariableValue: " + varInfo);
    }

    return (m_RTDBMgr ? m_RTDBMgr->writeVariableValue(varName, varValue) : false);
}

bool CMDVariableExpertImp::writeVariableValueRelative(const QString &varName, QVariant varValueRelative, bool needSignature/* = true*/)
{
    QVariant srcValue = m_RTDBMgr->readVariableValue(varName);
    bool srcResult;
    double srcDouble = srcValue.toDouble(&srcResult);
    if(srcResult)
    {
        bool stepResult;
        double stepDouble = varValueRelative.toDouble(&stepResult);
        if(stepResult)
        {
            double destDouble = srcDouble + stepDouble;
            return writeVariableValue(varName, QVariant(destDouble));
        }
    }

    return false;
}

bool CMDVariableExpertImp::writeVariableValueSwitch01(const QString &varName, bool needSignature/* = true*/)
{
    QVariant srcValue = m_RTDBMgr->readVariableValue(varName);
    bool srcResult;
    double srcDouble = srcValue.toDouble(&srcResult);
    if(srcResult)
    {
        double destDouble = (0.0 == srcDouble ? 1 : 0);
        return writeVariableValue(varName, QVariant(destDouble));
    }
    return false;
}

CMDActionSP* CMDVariableExpertImp::preCheckWriteSP(const QString &varName)
{
    CMDActionSPList* spList = m_RTDBMgr->variableActionSPList(varName);
    CMDActionSP* sp = (spList ? spList->find(MDActionType::variable_WriteValue) : NULL);
    if(!sp)
        return NULL;

    return (sp->isEnable() ? sp : NULL);
}

bool CMDVariableExpertImp::writeVariableValue_AfterCheckSP(const QString &varName, QVariant varValue, CMDActionSP* sp)
{
    if(sp->needSignature())
    {
        CMDActionSPSession spSession(*sp);
        if(!m_contacter->checkSP_WithUI(&spSession))
            return false;
    }

    if(m_lsWriteVariable->isOn())
    {
        QString varInfo = QString("{%1:%2}").arg(varName).arg(varValue.toString());
        m_lsWriteVariable->LOG_INFO_IS("写变量 m_RTDBMgr->writeVariableValue: " + varInfo);
    }

    return (m_RTDBMgr ? m_RTDBMgr->writeVariableValue(varName, varValue) : false);
}

bool CMDVariableExpertImp::writeVariables(QList<IMDVariable*>& nsVarList)
{
    if(m_lsWriteVariable->isOn())
    {
        QString varInfo;
        foreach (IMDVariable* var, nsVarList)
        {
            varInfo += QString("{%1:%2}").arg(var->name()).arg(var->currentValue().toString());
        }
        m_lsWriteVariable->LOG_INFO_IS("批量写变量 m_RTDBMgr->writeVariables: " + varInfo);
    }

    return (m_RTDBMgr ? m_RTDBMgr->writeVariables(nsVarList) : false);
}

qint8 CMDVariableExpertImp::executeAction(CMDAction* action)
{
    CMDActionProperty_Config* cp = action->configProperty();
    switch (cp->actionID().type())
    {
        case MDActionType::variable_WriteValue:
        {
            CAPC_VariableWriteValue* vwv = (CAPC_VariableWriteValue*)cp;
            switch (vwv->mode())
            {
            case MDWriteMode::mode_fixed:
            {
                writeVariableValue(vwv->actionID().objName(), vwv->value());
            }
                break;
            case MDWriteMode::mode_relative:
            {
                QVariant srcValue = m_RTDBMgr->readVariableValue(vwv->actionID().objName());
                bool srcResult;
                double srcDouble = srcValue.toDouble(&srcResult);
                if(srcResult)
                {
                    bool stepResult;
                    double stepDouble = vwv->value().toDouble(&stepResult);
                    if(stepResult)
                    {
                        double destDouble = srcDouble + stepDouble;
                        writeVariableValue(vwv->actionID().objName(), QVariant(destDouble));
                    }
                }
            }
                break;
            case MDWriteMode::mode_switch_0_1:
            {
                QVariant srcValue = m_RTDBMgr->readVariableValue(vwv->actionID().objName());
                bool srcResult;
                double srcDouble = srcValue.toDouble(&srcResult);
                double destDouble = (0.0 == srcDouble ? 1 : 0);
                writeVariableValue(vwv->actionID().objName(), QVariant(destDouble));
            }
                break;
            default:
                break;
            }
        }
        break;
        case MDActionType::variable_ReadValue:
        {
        }
        break;
    default:
        break;
    }

    return -1;
}

/*!
\section 处理变量变化通知
*/

//! 变量变化通知
void CMDVariableExpertImp::onVariableChanged(IMDChangeVariable *changedVar)
{
    if(m_lsOnVariableChanged->isOn())
        m_lsOnVariableChanged->LOG_INFO_IS("onVariableChanged: " + changedVar->name());

    if(!m_contacter)
    {
        delete changedVar;
        return;
    }
    m_contacter->onVariableChanged(changedVar);
}

void CMDVariableExpertImp::onDeviceVariablesChange(const QList<QByteArray>& jsonMsgList)
{
    if(m_lsOnVariableChanged->isOn())
        m_lsOnVariableChanged->LOG_INFO_IS("onDeviceVariablesChange");

    if(m_contacter)
        m_contacter->onDeviceVariablesChange(jsonMsgList);
}

void CMDVariableExpertImp::onDeviceVariablesBatchNotify(const QList<QByteArray>& redisCmdList)
{
    if(m_lsOnVariableChanged->isOn())
        m_lsOnVariableChanged->LOG_INFO_IS("onDeviceVariablesBatchNotify");

    if(m_contacter)
        m_contacter->onDeviceVariablesBatchNotify(redisCmdList);
}

void CMDVariableExpertImp::onVariablesBatchNotify(const QStringList& varNameList, const QList<QVariant>& varValueList)
{
    if(m_lsOnVariableChanged->isOn())
        m_lsOnVariableChanged->LOG_INFO_IS("onVariablesBatchNotify");

    if(m_contacter)
        m_contacter->onVariablesBatchNotify(varNameList, varValueList);
}

/*!
 * \brief RTDB 的变量变化通知类
 */
CMDVariableChangeNotifier::CMDVariableChangeNotifier(CMDVariableExpertImp* variableExpert)
{
    m_variableExpert = variableExpert;
}

void CMDVariableChangeNotifier::onChange(IMDChangeVariable *changedVar)
{
    if(m_variableExpert)
        m_variableExpert->onVariableChanged(changedVar);
}

void CMDVariableChangeNotifier::onDeviceVariablesChange(const QList<QByteArray>& jsonMsgList)
{
    if(m_variableExpert)
        m_variableExpert->onDeviceVariablesChange(jsonMsgList);
}

void CMDVariableChangeNotifier::onDeviceVariablesBatchNotify(const QList<QByteArray>& redisCmdList)
{
    if(m_variableExpert)
        m_variableExpert->onDeviceVariablesBatchNotify(redisCmdList);
}

void CMDVariableChangeNotifier::onVariablesBatchNotify(const QStringList& varNameList, const QList<QVariant>& varValueList)
{
    if(m_variableExpert)
        m_variableExpert->onVariablesBatchNotify(varNameList, varValueList);
}

