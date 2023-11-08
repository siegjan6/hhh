#ifndef MDDBQUERYSERVICEFACTORY_H
#define MDDBQUERYSERVICEFACTORY_H

/*!

\author bHVvY2hhbw==

\section 功能
1.查询模块相关工厂方法。

*/

#include "IMDDBQueryService.h"

#if defined(DBQUERYSERVICE_LIBRARY)
#  define DBQUERYSERVICESHARED_EXPORT Q_DECL_EXPORT
#else
#  define DBQUERYSERVICESHARED_EXPORT Q_DECL_IMPORT
#endif

class DBQUERYSERVICESHARED_EXPORT CMDDBQueryServiceFactory
{
public:
    CMDDBQueryServiceFactory();
    virtual ~CMDDBQueryServiceFactory();

    /*!
    \section 静态创建派生类
    */
public:
    static IMDDBQueryService *createDBQueryService(); //! 创建查询服务
    static IMDDBQueryParam *createDBQueryParam(); //! 创建查询参数
    static IMDConditionItem *createConditionItem(); //! 创建查询条件项
    static IMDCondition *createCondition(); //! 创建查询条件管理器
};

#endif // MDDBQUERYSERVICEFACTORY_H
