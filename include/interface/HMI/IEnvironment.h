/*!
\author aGRw 2014.10.27
\section 环境接口
*/
#ifndef IENVIRONMENT_H
#define IENVIRONMENT_H


#include <QList>
#include <QVector>
#include <QPointF>

#include "IObjName.h"
#include "../../MDDrawObj/DrawObjDefine.h"
#include "../../MDHMIForm/MDFormDefine.h"

class CMDDrawObj;

class IEnvironment
{
public:
    virtual ~IEnvironment(){}
/*!
\section property
*/
public:
    //!控件名称管理类
    virtual IObjName* nameManager() = 0;
    //!控件列表
    virtual QList<CMDDrawObj*>* objs() = 0;
    //!当前层
    virtual void setDefaultLayer(int value) = 0;
    virtual int defaultLayer() = 0;
    //!层的可见信息
    virtual QVector<bool>* visibleLayers() = 0;
    //!层的锁定信息
    virtual QVector<bool>* lockedLayers() = 0;
    //!窗体全路径名称
    virtual void setFullName(const QString& value) = 0;
    virtual QString fullName() = 0;
    virtual QPointF formScale() = 0;
    //!工程路径
    virtual QString projectPath() = 0;
/*!
\section function
*/
public:
    //!初始化
    virtual void initalization() = 0;
    /// 窗体根据Style属性显示，为normal、modal、topmost
    //void ShowStyle();
    //!控件图层改变
    virtual void objectCanSelectChanged(CMDDrawObj* obj) = 0;
/*!
\section refresh
*/
public:
    //!控件刷新
    virtual void invalidateObject(CMDDrawObj* obj) = 0;
    //!整体刷新
    virtual void timeInvalidate() = 0;
//////////////////////////////////////////////////////////////////////////
public:
    //!窗体类型
    virtual FormType formType() = 0;
};

#endif // IENVIRONMENT_H
