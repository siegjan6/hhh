/*!

\author dGFuZ3p3

\section 功能

模块与外部联系的类，目的是降低模块与外部的耦合关系。

\section 用法

模块的使用者派生一个类，并将派生类的实例设置给模块，参考 CMDScriptExpert::init。
模块的使用者必须实现所有的重载接口。

*/

#ifndef MDSCRIPTCONTACTER_H
#define MDSCRIPTCONTACTER_H

#include <QString>
#include "MDScriptExpert_Global.h"
#include "../include/MDScriptEngine/MDObjectWrapper.h"

class MDSCRIPTEXPERTSHARED_EXPORT CMDScriptContacter
{
public:
    CMDScriptContacter();
    virtual ~CMDScriptContacter();
public:
    virtual bool objectWrappers(QList<CMDObjectWrapper*>& wrapperList);
};

#endif // MDSCRIPTCONTACTER_H
