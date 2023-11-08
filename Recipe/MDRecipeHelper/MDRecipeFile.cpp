#include "MDRecipeFile.h"
#include "MDRecipeFileData.h"

CMDRecipeFile::CMDRecipeFile()
{
    m_data = new CMDRecipeFileData;
}

CMDRecipeFile::~CMDRecipeFile()
{
    delete m_data;
}

CMDRecipeFile::CMDRecipeFile(const CMDRecipeFile& obj)
{
    copy(&obj);
}

CMDRecipeFile& CMDRecipeFile::operator = (const CMDRecipeFile& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

bool CMDRecipeFile::copy(const CMDRecipeFile* source)
{
    m_data->m_recipe = source->m_data->m_recipe;
    m_data->m_fileName = source->m_data->m_fileName;
    return true;
}

bool CMDRecipeFile::load(const QString& fileName, CMDRecipe& fileRecipe)
{
    return CMDRecipeFileData::load(fileName, fileRecipe);
}

bool CMDRecipeFile::load()
{
    return m_data->load();
}

bool CMDRecipeFile::reload()
{
    return m_data->reload();
}

void CMDRecipeFile::unload()
{
    m_data->unload();
}

void CMDRecipeFile::save()
{
    m_data->save();
}

int CMDRecipeFile::save(const QString& fileName, const QString& content)
{
    return CMDRecipeFileData::save(fileName, content);
}

QString CMDRecipeFile::getContent(const QString& fileName)
{
    return CMDRecipeFileData::getContent(fileName);
}

QString CMDRecipeFile::fileName()
{
    return m_data->m_fileName;
}

void CMDRecipeFile::setFileName(const QString& fileName)
{
    m_data->m_fileName = fileName;
}

CMDRecipe* CMDRecipeFile::recipe()
{
    return &m_data->m_recipe;
}
