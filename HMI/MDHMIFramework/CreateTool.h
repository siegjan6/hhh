/*!
\author aGRw 2016.08.19
\section 控件创建工具类
*/

#include "MDDrawTool.h"

#ifndef CREATETOOL_H
#define CREATETOOL_H

class CCreateTool: public ICreateTool
{
public:
    virtual ~CCreateTool(){}
public:
    CMDDrawObj* createDrawObj(DrawType type);
};

#endif // CREATETOOL_H
