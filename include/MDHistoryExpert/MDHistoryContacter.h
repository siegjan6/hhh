/*!

\author dGFuZ3p3

\section 功能

模块与外部联系的类，目的是降低模块与外部的耦合关系。

\section 用法

模块的使用者派生一个类，并将派生类的实例设置给模块，参考 CMDHistoryExpert::init。
模块的使用者必须实现所有的重载接口。

*/

#ifndef MDHISTORYCONTACTER_H
#define MDHISTORYCONTACTER_H

#include <QVariant>
#include "MDHistoryExpert_Global.h"

class IMDVariable;
class MDHISTORYEXPERTSHARED_EXPORT CMDHistoryContacter
{
public:
    CMDHistoryContacter();
    virtual ~CMDHistoryContacter();
public:
    virtual bool variable(const QString& varName, IMDVariable* var); //! 获取变量信息
    virtual void onQueryResult(const QString& queryID, int result);
};

#endif // MDHISTORYCONTACTER_H
