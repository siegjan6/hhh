#ifndef EXPCOMPILEMGR_H
#define EXPCOMPILEMGR_H
#include "./Core/ExpCompile.h"
#include "./Function/VariableFun.h"
class CVariableFun;
class CExpCompileMgr
{
public:
    CExpCompileMgr();
    virtual ~CExpCompileMgr();
    bool compile(const QString& exp);
    void setVariableValue(const QString& varName, const QVariant& varValue);
    void repaceVariableName(const QString& varName,const QString& newVarName);
    QList<QString> referencedVarList();
    QVariant excute(bool* isok = NULL);
private:
    CFunctionBase* _fun;
    QString _exp;
    QMap<QString,QList<CVariableFun*>> _varList;

    void referencedVarHead(CFunctionBase* fun);
    void referencedVarList(CFunctionBase* fun);
    void insertVar(const QString& varName,CVariableFun* fun);
};

#endif // EXPCOMPILEMGR_H
