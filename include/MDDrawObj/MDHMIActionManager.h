/*!
\author aGRw 2015.07.09
\section 动作管理类
*/
#ifndef MDHMIACTION_H
#define MDHMIACTION_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWOBJ_LIBRARY)
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QString>
#include <QJsonObject>
#include "IHMIForm.h"


class MDDRAWOBJSHARED_EXPORT CMDHMIActionManager
{
public:
    CMDHMIActionManager();
//////////////////////////////////////////////////////////////////////////
public:
    QList<QString>* list(){return &m_list;}
    void setContainer(IHMIForm* value){m_container = value;}
private:
    QList<QString> m_list;
    IHMIForm* m_container;
//////////////////////////////////////////////////////////////////////////
public:
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
    void operator=(const CMDHMIActionManager& other);
public:
    void execute();
};

#endif // MDHMIACTION_H
