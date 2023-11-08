/*!
\author aGRw 2016.08.19
\section 控件工具类
*/
#ifndef MDDRAWTOOL_H
#define MDDRAWTOOL_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWOBJ_LIBRARY)
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "DrawObjDefine.h"

class CMDDrawObj;

/*!
*控件创建接口
*/
class ICreateTool
{
public:
    virtual ~ICreateTool(){}
public:
    //!创建控件
    virtual CMDDrawObj* createDrawObj(DrawType type) = 0;
};

class MDDRAWOBJSHARED_EXPORT CMDDrawTool
{
public:
    CMDDrawTool(){}
    ~CMDDrawTool();
public:
    static void init(ICreateTool* create);
    static void release();
    static CMDDrawTool* instance();
private:
    //静态对象，由主框架new&delete，在控件模块中使用
    static CMDDrawTool* m_drawTool;
public:
    ICreateTool* create(){return m_create;}
private:
    ICreateTool* m_create;
};
#endif // MDDRAWTOOL_H
