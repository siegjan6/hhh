#include "MDRecipeExpertImp.h"
#include "MDRecipeContacter.h"
#include "MDRecipeConfiger.h"
#include "MDRecipeFile.h"

CMDRecipeExpertImp::CMDRecipeExpertImp()
{
    m_pid = "";
    m_contater = NULL;
    m_configer = NULL;
}

CMDRecipeExpertImp::~CMDRecipeExpertImp()
{

}

void CMDRecipeExpertImp::setParaBeforeLoad(const QString& pid, CMDRecipeConfiger* configer, const QString& projectPath, CMDRecipeContacter* contacter)
{
    m_pid = pid;
    m_projectPath = projectPath;
    m_configer = configer;

    m_contater = contacter;

    m_downloadExpert.setParaBeforeLoad(this, m_contater);
}

void CMDRecipeExpertImp::load()
{
    m_downloadExpert.load();
    createSwitchs(true);
}

void CMDRecipeExpertImp::unload()
{
    m_downloadExpert.unload();
    createSwitchs(false);
}

bool CMDRecipeExpertImp::download(const QString& RecipeName, int showProcessDialog/* = 1*/)
{
    return (AddRecipeResult::success == m_downloadExpert.addNewTask(RecipeName, 0, showProcessDialog));
}

bool CMDRecipeExpertImp::upload(const QString& RecipeName)
{
    if(!m_contater)
        return false;

    if(!m_configer->exists(RecipeName))
        return false;

    CMDRecipeFile dFile;
    dFile.setFileName(m_configer->toFileName(RecipeName));
    dFile.load();

    QStringList itemIDList;
    dFile.recipe()->configProperty()->items()->enumIds(itemIDList);
    foreach (QString itemID, itemIDList)
    {
        QVariant data = m_contater->varValue(itemID);
        dFile.recipe()->configProperty()->items()->setItemValue(itemID, data.toString());
    }
    dFile.save();
    return true;
}

void CMDRecipeExpertImp::createSwitchs(bool create)
{
    if(create)
    {
        QString subFileDir = "ProjectExpert";
//        m_lsDownloadQueue = CMDLogSwitchFactory::createSwitchWithSubFileDir(m_pid, "RecipeExpert.DownloadQueue", "配方下载队列", subFileDir);
//        m_lsDownloadQueue->turnOn();
    }
    else
    {
//        m_lsDownloadQueue = NULL;
    }
}
