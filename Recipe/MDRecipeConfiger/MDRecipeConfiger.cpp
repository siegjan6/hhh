#include "MDRecipeConfiger.h"
#include "MDRecipeConfigerImp.h"
#include "MDRecipeFile.h"

CMDRecipeConfiger::CMDRecipeConfiger()
{
    m_imp = new CMDRecipeConfigerImp;
}

CMDRecipeConfiger::~CMDRecipeConfiger()
{
    if(m_imp)
        delete m_imp;
}

void CMDRecipeConfiger::load(const QString& projectPath)
{
    if(m_imp)
        m_imp->load(projectPath);
}

void CMDRecipeConfiger::unload()
{
    if(m_imp)
        m_imp->unload();
}

QString CMDRecipeConfiger::recipePath()
{
    return m_imp->recipePath();
}

bool CMDRecipeConfiger::beginEdit(const QString& RecipeName)
{
    return m_imp->beginEdit(RecipeName);
}

bool CMDRecipeConfiger::endEdit(const QString& RecipeName, bool save)
{
    return m_imp->endEdit(RecipeName, save);
}

CMDRecipe* CMDRecipeConfiger::findEdit(const QString& RecipeName)
{
    CMDRecipeFile* rFile = m_imp->findEdit(RecipeName);
    return (rFile ? rFile->recipe() : NULL);
}

bool CMDRecipeConfiger::saveEdit(const QString& RecipeName)
{
    return m_imp->saveEdit(RecipeName);
}

bool CMDRecipeConfiger::create(const QString& RecipeName)
{
    return m_imp->create(RecipeName);
}

bool CMDRecipeConfiger::create(const QString& srcRecipeName, const QString& destRecipeName)
{
    return m_imp->create(srcRecipeName, destRecipeName);
}

bool CMDRecipeConfiger::remove(const QString& RecipeName)
{
    return m_imp->remove(RecipeName);
}

bool CMDRecipeConfiger::exists(const QString& RecipeName)
{
    return m_imp->exists(RecipeName);
}

int CMDRecipeConfiger::save(const QString& recipeName, const QString& content)
{
    return m_imp->save(recipeName, content);
}

QString CMDRecipeConfiger::getContent(const QString& recipeName)
{
    return m_imp->getContent(recipeName);
}

QString CMDRecipeConfiger::toFileName(const QString& RecipeName)
{
    return m_imp->toFileName(RecipeName);
}

int CMDRecipeConfiger::getRecipeCount(bool onlyFile)
{
    return m_imp->getRecipeCount(onlyFile);
}

QString CMDRecipeConfiger::recipeName(int index)
{
    return m_imp->recipeName(index);
}

QList<QString> CMDRecipeConfiger::enumRecipe(bool onlyFile)
{
    return m_imp->enumRecipe(onlyFile);
}
