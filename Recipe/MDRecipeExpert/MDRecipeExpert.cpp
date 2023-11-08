#include "MDRecipeExpert.h"
#include "MDRecipeExpertImp.h"
#include "MDRecipeContacter.h"

CMDRecipeExpert::CMDRecipeExpert()
{
    m_imp = new CMDRecipeExpertImp;
}

CMDRecipeExpert::~CMDRecipeExpert()
{
    if(m_imp)
        delete m_imp;
}

/*!
\section 初始化与释放
*/
void CMDRecipeExpert::setParaBeforeLoad(const QString& pid, CMDRecipeConfiger* configer, const QString& projectPath, CMDRecipeContacter* contacter)
{
    if(m_imp)
        m_imp->setParaBeforeLoad(pid, configer, projectPath, contacter);
}

void CMDRecipeExpert::setDownloadProcessContacter(CMDRecipeContacter* contacter)
{
    m_imp->m_downloadExpert.setDownloadProcessContacter(contacter);
}

void CMDRecipeExpert::load()
{
    if(m_imp)
        m_imp->load();
}

void CMDRecipeExpert::unload()
{
    if(m_imp)
        m_imp->unload();
}

CMDRecipeConfiger* CMDRecipeExpert::configer()
{
    return m_imp->m_configer;
}

bool CMDRecipeExpert::download(const QString& RecipeName, int showProcessDialog/* = 1*/)
{
    return m_imp->download(RecipeName, showProcessDialog);
}

bool CMDRecipeExpert::upload(const QString& RecipeName)
{
    return m_imp->upload(RecipeName);
}

void CMDRecipeExpert::taskNameList(QStringList& nameList)
{
    m_imp->m_downloadExpert.m_taskQueue.taskNameList(nameList);
}

int CMDRecipeExpert::taskCount()
{
    return m_imp->m_downloadExpert.m_taskQueue.taskCount();
}

void CMDRecipeExpert::deleteTask(const QString& name)
{
    m_imp->m_downloadExpert.m_taskQueue.deleteTask(name);
}

void CMDRecipeExpert::deleteSomeTask(const QStringList& nameList)
{
    m_imp->m_downloadExpert.m_taskQueue.deleteSomeTask(nameList);
}

void CMDRecipeExpert::deleteAllTask()
{
    m_imp->m_downloadExpert.m_taskQueue.deleteAllTask();
}

void CMDRecipeExpert::downloadingTask(CMDRecipe& recipe)
{
    recipe = m_imp->m_downloadExpert.m_downloadingTask.m_itemsRemained;
}

QVariant CMDRecipeExpert::itemVarValue(const QString& itemID)
{
    return m_imp->m_contater->varValue(itemID);
}
