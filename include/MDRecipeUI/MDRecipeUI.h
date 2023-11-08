#ifndef MDRECIPEUI_H
#define MDRECIPEUI_H

#include "MDRecipeUI_Global.h"
#include "IStudioModule.h"
#include "IStudioInterface.h"

class CMDRecipeExpert;
class CMDRecipeUIImp;
class MDRECIPEUISHARED_EXPORT CMDRecipeUI : public IStudioModule
{
public:
    CMDRecipeUI();
    ~CMDRecipeUI();

/*!
\section property
*/
public:
    //根节点名称
    virtual QString rootName();
    //根节点图标
    virtual QIcon* rootIcon();
    //根节点
    virtual QTreeWidgetItem* rootItem();
    virtual void setRootItem(QTreeWidgetItem* value);
    //设置Studio接口
    virtual void setStudioInterface(IStudioInterface* value);
/*!
\section function
*/
public:
    //加载
    virtual void load();
    //卸载
    virtual void unload();

    //多语言
    virtual void changeLanguage();
    virtual void collectWords();
public:
    void setRecipeExpert(CMDRecipeExpert* recipeExpert);
    //open runtime
    void showRuntime();
    //hide runtime
    void hideRuntime();
protected:
    CMDRecipeUIImp* m_imp;
};

#endif // MDRECIPEUI_H
