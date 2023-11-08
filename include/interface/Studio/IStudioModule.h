/*!
\author aGRw 2016.11.03
\section Studio子模块接口
*/
#ifndef ISTUDIOMODULE_H
#define ISTUDIOMODULE_H

#include <QIcon>
#include <QTreeWidgetItem>
#include "IStudioInterface.h"

class IStudioModule
{
public:
    virtual ~IStudioModule(){}
/*!
\section property
*/
public:
    //根节点名称
    virtual QString rootName() = 0;
    //根节点图标
    virtual QIcon* rootIcon() = 0;
    //根节点
    virtual QTreeWidgetItem* rootItem() = 0;
    virtual void setRootItem(QTreeWidgetItem* value) = 0;
    //设置Studio接口
    virtual void setStudioInterface(IStudioInterface* value) = 0;
/*!
\section function
*/
public:
    //加载
    virtual void load() = 0;
    //卸载
    virtual void unload() = 0;

    //多语言
    virtual void changeLanguage() = 0;
    virtual void collectWords() = 0;
};

#endif // ISTUDIOMODULE_H
