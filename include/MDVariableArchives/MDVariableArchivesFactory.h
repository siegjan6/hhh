#ifndef MDVARIABLEARCHIVESFACTORY_H
#define MDVARIABLEARCHIVESFACTORY_H

#include "mdvariablearchives_global.h"
#include "IMDVariableArchivesManager.h"


class MDVARIABLEARCHIVESSHARED_EXPORT   CMDVariableArchivesFactory
{
public:
    CMDVariableArchivesFactory();
    ~CMDVariableArchivesFactory();
public:
    static IMDVariableArchivesManager *createVariableArchivesManager();
};
#endif // MDVARIABLEARCHIVESFACTORY_H
