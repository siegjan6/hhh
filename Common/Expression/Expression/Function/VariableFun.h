#ifndef VARIABLEFUN_H
#define VARIABLEFUN_H
#include "./Core/FunctionBase.h"

class CVariableFun : public CFunctionBase
{
public:
    CVariableFun();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    QString getVarName();
    void setVarValue(const QVariant& varValue);

private:
    QVariant _varValue;
};
#endif // VARIABLEFUN_H
