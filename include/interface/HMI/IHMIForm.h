/*!
\author aGRw 2014.10.27
\section 图形窗体接口
*/
#ifndef IHMIFORM_H
#define IHMIFORM_H

#include <QMouseEvent>
#include "IEnvironment.h"
#include "IStudio.h"
#include "IRun.h"
#include "IHMIFramework.h"

class IHMIForm
{
public:
    virtual ~IHMIForm(){}
/*!
\section property
*/
public:
    //!公共环境
    virtual IEnvironment* common() = 0;
    //!编辑环境
    virtual IStudio* studio() = 0;
    //!运行环境
    virtual IRun* run() = 0;
    //!图形框架
    virtual IHMIFramework* framework() = 0;
/*!
\section function
*/
public:
    //virtual void save() = 0;
    //!设置保存标记
    virtual void setModify(bool value) = 0;
//////////////////////////////////////////////////////////////////////////
};

#endif // IHMIFORM_H


