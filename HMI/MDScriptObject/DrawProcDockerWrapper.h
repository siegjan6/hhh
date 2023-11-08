//by luyj 2022.05.25
#ifndef MDDRAWPROCDOCKERWRAPPER_H
#define MDDRAWPROCDOCKERWRAPPER_H

#if defined(MDHMIFORM_LIBRARY)
#  define MDHMIFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDHMIFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "MDObjectWrapper.h"

#include <QString>

class CMDDrawProcDocker;


class CDrawProcDockerWrapper : public CMDObjectWrapper
{
    Q_OBJECT
public:
    CDrawProcDockerWrapper(void* object, const QString& name);
    virtual ~CDrawProcDockerWrapper(){}
//////////////////////////////////////////////////////////////////////////
public  Q_SLOTS:
    void setCmd(const QString& cmd);
    QString getCmd();
    void run(const QString& cmd);
//////////////////////////////////////////////////////////////////////////
};

#endif // MDDRAWPROCDOCKERWRAPPER_H

