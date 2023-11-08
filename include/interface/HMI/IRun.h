/*!
\author aGRw 2015.01.13
\section 运行环境接口
*/
#ifndef IRUN_H
#define IRUN_H

#include <QVariant>
#include "MDActionHelper/MDActionConstant.h"
#include "MDDrawObj/DrawObjDefine.h"
class CMDDrawObj;

class IRun
{
public:
    virtual ~IRun(){}
/*!
\section function
*/
public:
    //变量变化
    virtual void onDataChanged(const QString& name, const QVariant& value) = 0;

    //鼠标进入控件事件
    virtual void loadWidgetEnterEvent(CMDDrawObj* obj) = 0;
    //鼠标移出控件事件
    virtual void loadWidgetLeaveEvent(CMDDrawObj* obj) = 0;
/*!
\section script
*/
public:
    virtual void fireEvent(const QString& objName, const QString& eventName, QList<QVariant>& paras) = 0;
    virtual void callJsFunc(const QString funcName, QList<QVariant>& paras) = 0;
    virtual bool runDynamically(const QString& scriptCode) = 0;
/*!
\section fullScreen
*/
public:
    //全屏
    virtual void enterFullScreen() = 0;
    //退出全屏
    virtual void exitFullScreen() = 0;
//////////////////////////////////////////////////////////////////////////
    //安全策略
    virtual bool checkSecurity(MDActionType type) = 0;
    //通知
    virtual void notify(NotifyType type) = 0;
};

#endif // IRUN_H
