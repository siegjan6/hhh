/*!
\author aGRw 2015.05.19
\section 单个变量接口
*/
#ifndef MDPARAMETER_H
#define MDPARAMETER_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWOBJ_LIBRARY)
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QVariant>

class CMDExpInfo;

class CMDParameter
{
public:
    CMDParameter(const QString& name);
/*!
\section property
*/
public:
    QString name(){return m_name;}
    QList<CMDExpInfo*>* list(){return &m_list;}
    void setValue(const QVariant& value){m_value = value;}
    QVariant value(){return m_value;}
private:
    QString m_name;
    QList<CMDExpInfo*> m_list;
    QVariant m_value;
};

#endif // MDPARAMETER_H

