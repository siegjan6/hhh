/*!
\author aGRw 2015.06.04
\section 自定义属性类
*/
#ifndef MDCUSTOMPROPERTY_H
#define MDCUSTOMPROPERTY_H

#if defined(MDDRAWOBJ_LIBRARY)
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QString>

class MDDRAWOBJSHARED_EXPORT CMDCustomProperty
{
public:
    CMDCustomProperty(const QString& name, const QString& expression);
//////////////////////////////////////////////////////////////////////////
public:
    void setName(const QString& value){m_name = value;}
    QString name(){return m_name;}
    void setExpression(const QString& value){m_expression = value;}
    QString expression(){return m_expression;}
private:
    QString m_name;//!名称
    QString m_expression;//!表达式
};

#endif // MDCUSTOMPROPERTY_H
