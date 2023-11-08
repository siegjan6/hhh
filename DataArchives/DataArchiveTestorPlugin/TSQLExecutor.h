#ifndef TSQLEXECUTOR_H
#define TSQLEXECUTOR_H
#include <QString>
#include "JSDBParameters.h"

class TSQLExecutor
{
public:
    TSQLExecutor();
    int queryCount(const QString & dbSourceName, JSDBParameters * pDBParam, const QString & sqlStr);
};

#endif // TSQLEXECUTOR_H
