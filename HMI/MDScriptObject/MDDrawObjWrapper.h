/*!
\author aGRw 2015.05.21
\section 控件基类，脚本封装类
*/

#ifndef MDDRAWOBJWRAPPER_H
#define MDDRAWOBJWRAPPER_H

#if defined(MDHMIFORM_LIBRARY)
#  define MDHMIFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDHMIFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "MDObjectWrapper.h"

class CMDDrawObj;
class CMDDrawCurve;

class MDHMIFORMSHARED_EXPORT CMDDrawObjWrapper : public CMDObjectWrapper
{
    Q_OBJECT

    Q_PROPERTY(qreal left READ left WRITE setLeft)
    Q_PROPERTY(qreal top READ top WRITE setTop)
    Q_PROPERTY(qreal width READ width WRITE setWidth)
    Q_PROPERTY(qreal height READ height WRITE setHeight)
    Q_PROPERTY(bool visible READ visible WRITE setVisible)
public:
    CMDDrawObjWrapper(void* object, const QString& name);
    virtual ~CMDDrawObjWrapper(){}
//////////////////////////////////////////////////////////////////////////
//property
public Q_SLOTS:
    void setLeft(qreal value);
    qreal left();
    void setTop(qreal value);
    qreal top();
    void setWidth(qreal value);
    qreal width();
    void setHeight(qreal value);
    qreal height();
    void setVisible(bool value);
    bool visible();
public Q_SLOTS:
    //动态设置表达式
    void setExpression(const QString& name, const QString& expression);
public:
    CMDDrawObj* object();

};


#endif // MDDRAWOBJWRAPPER_H

