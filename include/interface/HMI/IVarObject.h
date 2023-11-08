/*!
\author aGRw 2015.12.21
\section 变量控件接口
*/
#ifndef IVAROBJECT_H
#define IVAROBJECT_H

class CMDExpInfo;

class IVarObject
{
public:
    virtual ~IVarObject(){}
/*!
\section function
*/
public:
    virtual void loadDataChangedEvent(CMDExpInfo *expression) = 0;

};
#endif // IVAROBJECT_H



