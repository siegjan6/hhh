/*!
\author aGRw 2014.10.27
\section 编辑环境接口
*/
#ifndef ISTUDIO_H
#define ISTUDIO_H

#include <QPainter>
#include "QUndoStack"
#include "IObjName.h"

class CMDDrawObj;

class IStudio
{
public:
    virtual ~IStudio(){}
/*!
\section property
*/
public:
    //!窗体缩放比例
    virtual void setFormScale(const QPointF& value) = 0;
    //!正交模式
    virtual void setIsOrtho(bool value) = 0;
    virtual bool isOrtho() = 0;
    //!属性框
    virtual void* property() = 0;
    //!选中的控件列表
    virtual QList<CMDDrawObj*>* selectObjs() = 0;
    //!设置属性框显示对象
    virtual void selectPropetyObject(CMDDrawObj* obj) = 0;
    //!undo管理器
    virtual QUndoStack* undo() = 0;
/*!
\section function
*/
public:
    //!状态栏数据改变事件
    virtual void statusLabelChanged() = 0;
    //!选择控件改变事件
    virtual void selectedObjChanged() = 0;
    //!选择控件
    virtual void itemSelectObject(QList<CMDDrawObj*>* objs) = 0;
    //!粘帖控件
    virtual void pasteObject(CMDDrawObj *obj) = 0;
    //!选择的子控件
    virtual void setLastSelectedObject(CMDDrawObj *obj) = 0;
    //!绘制预览窗体
    virtual void drawPreview(QPainter* p) = 0;
    //批量操作后，修改控制点
    virtual void invalidateBatchControlPoint() = 0;



};

#endif // ISTUDIO_H
