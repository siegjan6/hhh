/*!
\author aGRw 2015.02.04
\section 控件名称管理类,名称比较时忽略大小写
*/
#ifndef IOBJNAME_H
#define IOBJNAME_H

#include <QString>


class IObjName
{
public:
    virtual ~IObjName(){}
/*!
\section function
*/
public:
    //控件名称是否包含在字典中
    virtual bool containsName(const QString& name) = 0;
    //将控件名称添加到字典中
    virtual void addName(const QString& name) = 0;
    //检查控件名称;如果不存在,从字典中删除
    virtual void dynamicContainsName(const QString& name) = 0;

};

#endif // IOBJNAME_H



