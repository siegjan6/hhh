/*!
\author aGRw 2015.05.27
\section 新控件管理类.添加新控件代码,统一放于此类中
*/
//todo add control
#ifndef DRAWCREATOR_H
#define DRAWCREATOR_H

#include <QList>

#include "MDDrawObj.h"
#include "MDDrawTextWrapper.h"

class CMDObjectWrapper;

class MDHMIFORMSHARED_EXPORT CDrawCreator
{
public:
    //!是否是节点类型
    static bool isDrawNode(DrawType type);
    //!控件的正交编辑模式
    static OrthoMode drawOrthoMode(DrawType type);
//////////////////////////////////////////////////////////////////////////
public:
    //!控件名称, 需要和枚举DrawType保持一致
    static QList<QString>* names();
private:
    static QList<QString> m_names;
//////////////////////////////////////////////////////////////////////////
public:
    //!创建控件
    static CMDDrawObj* createDrawObj(DrawType type);
    //!创建脚本包装类
    static CMDObjectWrapper *createObjectWrapper(CMDDrawObj* obj);
//////////////////////////////////////////////////////////////////////////
public:
    //!设置表达式对象索引
    static void initExpression();
};

#endif // DRAWCREATOR_H



