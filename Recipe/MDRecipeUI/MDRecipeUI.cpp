#include "MDRecipeUI.h"
#include "MDRecipeUIImp.h"
#include "MDRecipeExpert.h"

CMDRecipeUI::CMDRecipeUI()
{
    m_imp = new CMDRecipeUIImp;
}

CMDRecipeUI::~CMDRecipeUI()
{
    if(m_imp)
        delete m_imp;
}

void CMDRecipeUI::setRecipeExpert(CMDRecipeExpert* recipeExpert)
{
    m_imp->m_recipeExpert = recipeExpert;
    m_imp->m_recipeExpert->setDownloadProcessContacter(&m_imp->m_contacter);
    m_imp->m_downloadDialog.m_recipeExpert = recipeExpert;
}

/*!
\section property
*/
//根节点名称
QString CMDRecipeUI::rootName()
{
    return "配方";
}

//根节点图标
QIcon* CMDRecipeUI::rootIcon()
{
    return NULL;
}

//根节点
QTreeWidgetItem* CMDRecipeUI::rootItem()
{
    return m_imp->m_rootInStudio;
}

void CMDRecipeUI::setRootItem(QTreeWidgetItem* value)
{
    m_imp->m_rootInStudio = value;
}

//设置Studio接口
void CMDRecipeUI::setStudioInterface(IStudioInterface* value)
{
    m_imp->m_studioInterface = value;
}
/*!
\section function
*/
//加载
void CMDRecipeUI::load()
{
    m_imp->load();
}

//卸载
void CMDRecipeUI::unload()
{
    m_imp->unload();
}

//open runtime
void CMDRecipeUI::showRuntime()
{
    m_imp->m_downloadDialog.show();
}

//hide runtime
void CMDRecipeUI::hideRuntime()
{
    m_imp->m_downloadDialog.hide();
}

/*!
\section 多语言
*/
void CMDRecipeUI::changeLanguage()
{
}

void CMDRecipeUI::collectWords()
{
}
