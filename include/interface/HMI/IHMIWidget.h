/*!
\author aGRw 2015.08.27
\section 控件接口
*/
#ifndef IHMIWIDGET_H
#define IHMIWIDGET_H

#include "IHMIFramework.h"

class IHMIWidget
{
public:
    virtual ~IHMIWidget(){}
/*!
\section function
*/
public:
    //!确定
    virtual bool onOk() = 0;
    //!初始化
    virtual void init(IHMIFramework* framework) = 0;


};

#endif // IHMIWIDGET_H
