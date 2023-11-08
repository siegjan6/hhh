/*!
\author aGRw 2015.07.29
\section 图库接口
*/
#ifndef IGRAPHICSSTORAGE_H
#define IGRAPHICSSTORAGE_H

#include <QList>
#include "IHMIForm.h"
class CMDDrawObj;


class IGraphicsStorage
{
public:
    virtual ~IGraphicsStorage(){}
public:
    //!打开对话框
    virtual void showDialog(IHMIForm* container) = 0;
    //!保存控件
    virtual void saveObjDialog(QList<CMDDrawObj*>* objs) = 0;
    //!保存控件到默认目录
    virtual void saveObjToPath(QList<CMDDrawObj*>* objs) = 0;
};

#endif // IGRAPHICSSTORAGE_H



