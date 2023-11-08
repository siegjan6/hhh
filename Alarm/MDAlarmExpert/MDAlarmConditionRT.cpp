#include "MDAlarmConditionRT.h"
#include "MDAlarmContacter.h"
#include "../../include/MDRTDBManager/MDRTDBManagerFactory.h"

CMDAlarmConditionRT::CMDAlarmConditionRT(CMDAlarmCondition* condition, CMDAlarmLevel* level,
                                         CMDAlarmContacter* contacter, CMDActionSP* acknowledgeSP, CMDActionSP* confirmSP)
{
    m_condition = condition;
    m_limitAlarmMode = condition->currentLimitAlarmMode();

    m_eventExpression.compile(condition->expression());
    m_refVarList = m_eventExpression.referencedVarList();
    m_contacter = contacter;

    m_level = level;
    m_acknowledgeSP = acknowledgeSP;
    m_confirmSP = confirmSP;
}

CMDAlarmConditionRT::~CMDAlarmConditionRT()
{
}

/*!
\section 判断报警条件是否满足
*/
bool CMDAlarmConditionRT::handle()
{
    foreach (const QString varName, m_refVarList)
    {
        QVariant curValue;
		// todo: 报警表达式检查涉及变量的有效性
        if(isGoodExpressionVar(varName, curValue))
            m_eventExpression.setVariableValue(varName, curValue);
        else
            return false;

        // 如果报警仅绑定了一个变量，且此变量启用了报警上下限值
    }

    QVariant vResult = m_eventExpression.excute();
    bool result = vResult.toBool();
    return result;
}

bool CMDAlarmConditionRT::isGoodExpressionVar(const QString& varName, QVariant& curValue)
{
    IMDVariable *expressionVar = CMDRTDBManagerFactory::createVariable();
    expressionVar->setName(varName);
    if(!m_contacter->variable(varName, expressionVar))
    {
        delete expressionVar;
        return false;
    }

    curValue = expressionVar->currentValue();
    MDVariableDataQuality curQuality = expressionVar->currentQuality();
    delete expressionVar;
    return (curQuality == MDVariableDataQuality::Good &&
                curValue.isValid());
}
