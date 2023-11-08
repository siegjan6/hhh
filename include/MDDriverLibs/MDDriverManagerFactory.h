#ifndef MDDRIVERMANAGERFACTORY_H
#define MDDRIVERMANAGERFACTORY_H


#include "MDDriverLibs_global.h"
#include "IMDDriverManager.h"
#include "ICommunicationComponent.h"
class MDDRIVERLIBSSHARED_EXPORT CMDDriverManagerFactory
{
public:
    CMDDriverManagerFactory();
    ~CMDDriverManagerFactory();
public:
    static IMDDriverManager *createMDDriverManager();
};

#endif // MDDRIVERMANAGERFACTORY_H
