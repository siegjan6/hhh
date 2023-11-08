#include "MDExpression.h"
#include "./Core/ExpCompileMgr.h"
CMDExpression::CMDExpression()
{
    _engine = new CExpCompileMgr();
}

CMDExpression::CMDExpression(const QString &exp)
{
    _engine = new CExpCompileMgr();
    _engine->compile(exp);
}

CMDExpression::~CMDExpression()
{
    if(_engine)
        delete _engine;
}

bool CMDExpression::compile(const QString &exp)
{
    return _engine->compile(exp);
}

QVariant CMDExpression::excute(bool *isok)
{
    QVariant var;
    var = _engine->excute(isok);
    return var;
}

void CMDExpression::setVariableValue(const QString &varName, QVariant value)
{
    _engine->setVariableValue(varName,value);
}

QList<QString> CMDExpression::referencedVarList()
{
    return _engine->referencedVarList();
}
void CMDExpression::repaceVariableName(const QString& varName,const QString& newVarName)
{
    _engine->repaceVariableName(varName,newVarName);
}

/*
template<typename T>
void setVariableValue(const QString& varName,const T& varValue)
{
    QVariant v;
    v.setValue(varValue);
    setVariableValue(varName,v);
}
*/
