#ifndef MDDBQUERY_ENGINE_2016_11_01_H
#define MDDBQUERY_ENGINE_2016_11_01_H


#include "IMDDBQueryEngine.h"

#if defined(DBQUERYENGINE_LIBRARY)
#  define DBQUERYENGINE_EXPORT Q_DECL_EXPORT
#else
#  define DBQUERYENGINE_EXPORT Q_DECL_IMPORT
#endif

class DBQUERYENGINE_EXPORT CMDDBQueryEngineFactory
{
public:
    CMDDBQueryEngineFactory();
    virtual ~CMDDBQueryEngineFactory();

    /*!
    \section 静态创建派生类
    */
public:
    static IMDDBQueryEngine *createDBQueryEngine(); //! 创建查询服务
};

#endif // MDDBQUERY_ENGINE_2016_11_01_H
