/*!
\author hdp 2015.04.20
\section 变量管理器接口
*/
#ifndef IEXPRESSIONMANAGER_H
#define IEXPRESSIONMANAGER_H

#include <QList>
#include <QMap>
#include <IPropertyExpression.h>


class IExpressionManager
{
public:
    virtual ~IExpressionManager(){}
//////////////////////////////////////////////////////////////////////////
public:
    virtual QList<IPropertyExpression*>* list() = 0;
//////////////////////////////////////////////////////////////////////////
public:
    virtual void addParameter(const QString& name, const QString& expression = "", int index = 0) = 0;
    virtual void generateIndex(QMap<QString, int> *dict) = 0;
    virtual int expressionCount() = 0;
    virtual void serialize(QDataStream& stream, bool isOrigVersion = false) = 0;
    virtual void deserialize(QDataStream& stream) = 0;
//////////////////////////////////////////////////////////////////////////
};

#endif // IEXPRESSIONMANAGER_H


