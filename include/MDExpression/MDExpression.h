#ifndef CEXPRESSION_H
#define CEXPRESSION_H
#include <QtCore/qglobal.h>
#include <QVariant>
#if defined(MDEXPRESSION_LIBRARY)
#  define EXPRESSIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EXPRESSIONSHARED_EXPORT Q_DECL_IMPORT
#endif
class CExpCompileMgr;

/***********************************************************/
//先编译（初始化一次，内部只会执行一次缓存起来，除非表达式发生变化）
//再设变量值 （可以重复设）
//最后执行获取结果
/***********************************************************/
class EXPRESSIONSHARED_EXPORT CMDExpression
{
public:
    CMDExpression();
    CMDExpression(const QString& exp);
    virtual ~CMDExpression();

    bool compile(const QString& exp);
    QVariant excute(bool* isok = NULL);

    void setVariableValue(const QString& varName,QVariant value);
    QList<QString> referencedVarList();
    void repaceVariableName(const QString& varName,const QString& newVarName);
private:
    CExpCompileMgr* _engine;
};

#endif // CEXPRESSION_H
